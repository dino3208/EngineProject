#include "Player.h"
#include <Input/Input.h>
#include <Render/Renderer.h>

// 맵 데이터 Todo: 나중에 파일로 수정하는 형식으로 변환
std::vector<std::string> mapData =
{
	"########",
	"#..D...#",
	"#..##..#",
	"#....K.#",
	"########"
};

// 화면 크기 *주의사항* Config\Setting.txt 파일을 수정하고 수정할 것.(일치 필수)
// 나중에 파일 참고 형식으로 변환
const int screenHeight = 80;
const int screenWidth = 240;

// 각도->라디안 변환 함수
const float PI = 3.14159265f;
float DegToRad(float degree)
{
	return degree * (PI / 180.0f);
}

// 시야각
const float FOV = DegToRad(60.0f);

// 벽인지 확인
bool IsWall(int x, int y)
{
	if (y < 0 || y >= (int)mapData.size())
	{
		return true;
	}
	if (x < 0 || x >= (int)mapData[y].length())
	{
		return true;
	}

	return mapData[y][x] == '#' || mapData[y][x] == 'D';
}


// 화면 가로 넓이 갯수만큼 광선 갯수
float distances[screenWidth];

// 광선 함수
float CastRay(float startX, float startY, float angle)
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
		if (IsWall((int)rayX, (int)rayY))
		{
			return distance;
		}
	}

	return maxDistance;
}




//void CastAllRays(float playerX, float playerY, float playerAngle)
//{
//	float startAngle = playerAngle - FOV / 2.0f;
//	float angleStep = FOV / screenWidth;
//
//	for (int i = 0; i < screenWidth; ++i)
//	{
//		float rayAngle = startAngle + angleStep * i;
//		distances[i] = CastRay(playerX, playerY, rayAngle);
//	}
//}

// 광선 여러개 쏘는 함수
void CastAllRays(float playerX, float playerY, float playerAngle)
{
	float angleStep = FOV / screenWidth;

	for (int i = 0;i < screenWidth;++i)
	{
		// 비스듬히 쏘는 광선에 의해 외곡되는 광선 보정 (어안렌즈 현상 수정)
		float offset = angleStep * i - FOV / 2.0f;
		float rayAngle = playerAngle + offset;

		float rawDistance = CastRay(playerX, playerY, rayAngle);
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
	if (distance < 3.0f)
	{
		return '#';
	}
	if (distance < 6.0f)
	{
		return ':';
	}
	if (distance < 10.0f)
	{
		return '.';
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

// 아이템 줍기 시도 함수
void TryPickUpItem(float playerX, float playerY)
{
	int x = (int)playerX;
	int y = (int)playerY;

	// 화면 밖으로 나가는 것 방지
	if (y < 0 || y >= (int)mapData.size()) { return; }
	if (x < 0 || x >= (int)mapData[y].length()) { return; }

	// 열쇠인 K 획득시. 열쇠 보유로 전환 + K를 바닥으로 전환
	if (mapData[y][x] == 'K')
	{
		hasKey = true;
		mapData[y][x] = '.';
	}
}

// 열쇠 메커니즘
bool hasKey = false;
// 문 열기 시도 함수
void TryOpenDoor(float playerX, float playerY, float playerAngle)
{
	if (!hasKey)
	{
		return;
	}
	int frontX = (int)(playerX + cosf(playerAngle));
	int frontY = (int)(playerY + sinf(playerAngle));

	// 화면 밖으로 나가는 경우 방지
	if (frontY < 0 || frontY >= (int)mapData.size()) { return; }
	if (frontX < 0 || frontX >= (int)mapData[frontY].length()) { return; }

	// 열쇠가 있을 경우 문인 D를 바닥인 .으로 변환
	if (mapData[frontY][frontX] == 'D')
	{
		mapData[frontY][frontX] = '.';
	}
}

//---------------------------------------------------------------------------------------//

using namespace Craft;

void Player::BeginPlay()
{
}

void Player::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	const float rotSpeed = DegToRad(90.0f);
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

	if (!IsWall((int)nextX, (int)playerY)) {playerX = nextX;}
	if (!IsWall((int)playerX, (int)nextY)) {playerY = nextY;}

	if (Input::Get().GetKeyDown('E'))
	{
		TryPickUpItem(playerX, playerY);
		TryOpenDoor(playerX, playerY, playerAngle);
	}

	CastAllRays(playerX, playerY, playerAngle);
	
}

void Player::Draw()
{
	// 거리에 따른 벽 위아래 그리기로 거리감 표현
	for (int x = 0;x < screenWidth;++x)
	{
		int wallHeight = (int)(screenHeight / distances[x]);
		if (wallHeight > screenHeight)
		{
			wallHeight = screenHeight;
		}

		// 벽 그리기 -> 콘솔의 Y좌표는 반대임에 유의
		int wallTop = (screenHeight / 2) - (wallHeight / 2);
		int wallBottom = (screenHeight / 2) + (wallHeight / 2);

		for (int y = 0;y < screenHeight;++y)
		{
			if (y >= wallTop && y <= wallBottom)
			{
				std::string ch(1, GetWallChar(distances[x]));
				Renderer::Get().Submit(ch, Vector2(x, y), GetWallColor(distances[x]));
			}
		}
	}
}


