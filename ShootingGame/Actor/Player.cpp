#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Engine/Engine.h>

#include "Player.h"


// 시야각
const float FOV = DegToRad(60.0f);

// 화면 가로 넓이 갯수만큼 광선 갯수
std::vector<float> distances;

// 광선 함수
// map 객체를 기준으로 확인할지 알려줘야 하기 때문에 매개변수 map을 추가
float CastRay(const Map& map, float startX, float startY, float angle)
{
	const float step = 0.05f;
	const float maxDistance = 20.0f;

	float rayX = startX;
	float rayY = startY;
	float distance = 0.0f;

	// 움직이는 거리가 최대거리보다 작은 경우
	while (distance < maxDistance)
	{
		// X좌표 광선
		rayX += cosf(angle) * step;
		// Y좌표 광선
		rayY += sinf(angle) * step;
		// 거리에 광선 반영
		distance += step;

		// 
		if (map.IsWall((int)rayX, (int)rayY)) // 포인터 멤버가 아니라 매개변수
		{
			return distance;
		}
	}

	return maxDistance;
}

// 광선 여러개 쏘는 함수
void CastAllRays(const Map& map, float playerX, float playerY, float playerAngle, int viewWidth)
{
	float angleStep = FOV / viewWidth;

	for (int i = 0;i < viewWidth;++i)
	{
		// 비스듬히 쏘는 광선에 의해 외곡되는 광선 보정 (어안렌즈 현상 수정)
		float offset = angleStep * i - FOV / 2.0f;
		float rayAngle = playerAngle + offset;

		float rawDistance = CastRay(map, playerX, playerY, rayAngle);
		distances[i] = rawDistance * cosf(offset);
	}
}

// Todo: 음영표현
// █░▒▓

//if (distance < 1.0f)
//{
//	return (char)219;
//}
//if (distance < 5.0f)
//{
//	return (char)178;
//}
//if (distance < 8.0f)
//{
//	return (char)176;
//}
//return ' ';

// 거리에 따른 벽 표시 글자 변화로 거리감 표현
char GetWallChar(float distance)
{
	if (distance < 2.0f)
	{
		return '#';
	}
	if (distance < 3.5f)
	{
		return '%';
	}
	if (distance < 5.0f)
	{
		return ':';
	}
	if (distance < 7.0f)
	{
		return '.';
	}
	if (distance < 10.0f)
	{
		return ',';
	}
	return ' ';
}

// 거리에 따른 벽 색깔 표시
Craft::Color GetWallColor(float distance)
{
	if (distance < 3.0f)
	{
		return Craft::Color::BrightWhite;
	}
	if (distance < 6.0f)
	{
		return Craft::Color::White;
	}

	return Craft::Color::Red;
}

// 아이템(열쇠) 줍기 시도 함수
void Player::TryPickUpItem()
{
	int x = (int)playerX;
	int y = (int)playerY;

	// 화면 밖으로 나가는 것 방지
	if (y < 0 || y >= (int)map->mapData.size()) { return; }
	if (x < 0 || x >= (int)map->mapData[y].length()) { return; }

	// 열쇠인 K 획득시. 열쇠 보유로 전환 + K를 바닥으로 전환
	if (map->mapData[y][x] == 'K')
	{
		hasKey = true;
		map->mapData[y][x] = '.';
	}
}


// 문 열기 시도 함수
void Player::TryOpenDoor()
{
	if()
	int frontX = (int)(playerX + cosf(playerAngle));
	int frontY = (int)(playerY + sinf(playerAngle));

	// 화면 밖으로 나가는 경우 방지
	if (frontY < 0 || frontY >= (int)map->mapData.size()) { return; }
	if (frontX < 0 || frontX >= (int)map->mapData[frontY].length()) { return; }

	// 열쇠가 있을 경우 문인 D를 바닥인 .으로 변환
	if (map->mapData[frontY][frontX] == 'D')
	{
		map->mapData[frontY][frontX] = '.';
	}
}

// 플레이어 랜턴
const float lanternRadius = 1.0f;
const float lanternFalloff = 2.0f;
//---------------------------------------------------------------------------------------//

using namespace Craft;

void Player::BeginPlay()
{
	distances.resize(viewWidth);
}

void Player::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	const float rotSpeed = Util::DegToRad(90.0f);
	const float moveSpeed = 3.0f;

	if (Input::Get().GetKey('A'))
	{
		playerAngle -= rotSpeed * deltaTime;
	}
	if (Input::Get().GetKey('D'))
	{
		playerAngle += rotSpeed * deltaTime;
	}

	float moveX = 0.0f;
	float moveY = 0.0f;

	if (Input::Get().GetKey('W'))
	{
		moveX = cosf(playerAngle) * moveSpeed * deltaTime;
		moveY = sinf(playerAngle) * moveSpeed * deltaTime;
	}
	if (Input::Get().GetKey('S'))
	{
		moveX = -cosf(playerAngle) * moveSpeed * deltaTime;
		moveY = -sinf(playerAngle) * moveSpeed * deltaTime;
	}

	float nextX = playerX + moveX;
	float nextY = playerY + moveY;

	if (!map->IsWall((int)nextX, (int)playerY)) {playerX = nextX;}
	if (!map->IsWall((int)playerX, (int)nextY)) {playerY = nextY;}

	if (Input::Get().GetKeyDown('E'))
	{
		TryPickUpItem();
		TryOpenDoor();
	}

	CastAllRays(*map, playerX, playerY, playerAngle, viewWidth);
}

void Player::Draw()
{
	// 거리에 따른 벽 위아래 그리기로 거리감 표현
	for (int x = 0;x < viewWidth;++x)
	{
		// 랜턴 범위 밖은 그리지 않는 함수
		if (distances[x] > lanternRadius + lanternFalloff)
		{
			continue;
		}

		int wallHeight = (int)(viewHeight / distances[x]);
		if (wallHeight > viewHeight)
		{
			wallHeight = viewHeight;
		}

		// 벽 그리기 -> 콘솔의 Y좌표는 반대임에 유의
		int wallTop = (viewHeight / 2) - (wallHeight / 2);
		int wallBottom = (viewHeight / 2) + (wallHeight / 2);

		for (int y = 0;y < viewHeight;++y)
		{
			if (y >= wallTop && y <= wallBottom)
			{
				char ch;

				if (distances[x] > lanternRadius)
				{
					ch = '.';
				}
				else
				{
					ch = GetWallChar(distances[x]);
				}
				Renderer::Get().Submit(std::string(1,ch), Vector2(x, y), GetWallColor(distances[x]));
			}
		}
	}
}