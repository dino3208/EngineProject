#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Engine/Engine.h>
#include <vector>

#include "Player.h"
#include "Actor/Monster.h"
#include "Level/MainMenu.h"
#include "UI/TextBox.h"
#include "UI/ScreenArt.h"


Player::Player()
	:Actor("P", Vector2::Zero, Color::White)
{
}

Player::~Player()
{
}

void Player::SetSpawnPosition(float x, float y)
{
	playerX = x;
	playerY = y;
	SetPosition(Vector2(static_cast<int>(x), static_cast<int>(y)));
	SavePreviousState();// 첫 프레임 충돌검사시 둘 다 기본값이기 때문에 몬스터와 플레이어가 충돌판정이 있었음.
}



// 시야각
const float FOV = DegToRad(60.0f);

// 화면 가로 넓이 갯수만큼 광선 갯수
std::vector<float> distances;
std::vector<int> wallHitX;
std::vector<int> wallHitY;

// 광선 함수
// map 객체를 기준으로 확인할지 알려줘야 하기 때문에 매개변수 map을 추가
float CastRay(const Map& map, float startX, float startY, float angle, int& hitX, int& hitY)
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

		
		if (map.IsWall((int)rayX, (int)rayY)) // 포인터 멤버가 아니라 매개변수
		{
			hitX = static_cast<int>(rayX);
			hitY = static_cast<int>(rayY);
			return distance;
		}
	}

	// 벽에 안 부딪히고 최대거리까지 간 경우.
	hitX = -1; // 맵 좌표는 항상 0 이상이다.
	hitY = -1; // 따라서 -1은 절대 나올 수 없는 값 -> 특별 신호로 약속.

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

		int hitX, hitY;
		float rawDistance = CastRay(map, playerX, playerY, rayAngle, hitX, hitY);
		distances[i] = rawDistance * cosf(offset);
		wallHitX[i] = hitX;
		wallHitY[i] = hitY;
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
		textBox->ShowLines(textBox->GetMessageArt(TextBox::MessageType::KeyPickUp));
		Engine::Get().PlayOneShot("Key.wav");
	}
}

// 탈출 시도 함수.
void Player::TryExit()
{
	int x = static_cast<int>(playerX);
	int y = static_cast<int>(playerY);

	// 화면 밖으로 나가는 것 방지.
	if (y < 0 || y >= static_cast<int>(map->mapData.size())) { return; }
	if (x < 0 || x >= static_cast<int>(map->mapData[y].length())) { return; }

	// 열쇠 획득상태로 골 지점 도달시 엔딩.
	if (hasKey && map->mapData[y][x] == 'G')
	{
		hasWon = true;
		winScreenTimer = 10.0f;
	}
	/*int frontX = roundf(playerX + cosf(playerAngle));
	int frontY = roundf(playerY + sinf(playerAngle));

	if (frontY < 0
		|| frontY >= static_cast<int>(map->mapData.size()))
	{
		return;
	}
	if( frontX < 0
		|| frontX >= static_cast<int>(map->mapData[frontY].length())) 
	{
		return;
	}

	if (map->mapData[frontY][frontX] == 'G' && hasKey)
	{
		hasWon = true;
		winScreenTimer = 10.0f;
	}*/
}

// 테두리 벽 색 칠하기 함수.
bool TryGetBorderColor(const Map& map, int hitX, int hitY, Color& outColor)
{
	if (hitX < 0 || hitY < 0)
	{
		return false; // 벽에 안 부딪힌 경우.
	}

	int lastY = static_cast<int>(map.mapData.size() - 1);
	int lastX = static_cast<int>(map.mapData[0].length() - 1);

	if (hitY == 0) { outColor = Color::Red; return true; }
	if (hitY == lastY) { outColor = Color::Yellow; return true; }
	if (hitX == 0) { outColor = Color::Green; return true; }
	if (hitX == lastX) { outColor = Color::Blue; return true; }

	return false;
}
//---------------------------------------------------------------------------------------//

using namespace Craft;

void Player::OnCollision(const std::shared_ptr<Actor>& other)
{
	std::shared_ptr<Monster> monster = Cast<Monster>(other);
	{
		if (!monster)
		{
			return;
		}

		if (invulnerableTimer > 0.0f)
		{
			return;
		}

		const int damageValues[4] = { 10,20,30,40 };
		if (hitCount >= 4)
		{
			return;
		}
		// 소리.
		Engine::Get().PlayOneShot("Damage.wav");

		int damage = damageValues[hitCount];
		hitCount++;
		totalDamage += damage;

		// 두가지 메세지 출력을 위해 변수 만들어 lines에 넣기.
		std::vector<std::string> lines = textBox->GetMessageArt(TextBox::MessageType::Damaged, damage);
		std::vector<std::string> totalDamageArt = textBox->GetMessageArt(TextBox::MessageType::TotalDamage, totalDamage);
		for (const std::string& line : totalDamageArt)
		{
			lines.emplace_back(line);
		}
		textBox->ShowLines(lines);
		
		invulnerableTimer = 1.0f;
		monster->Stun(3.0f);

		if (totalDamage >= 100)
		{
			hasLost = true;
		}
	}
}

void Player::BeginPlay()
{
	distances.resize(viewWidth);
	wallHitX.resize(viewWidth);
	wallHitY.resize(viewWidth);
}

void Player::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (hasWon)
	{
		Engine::Get().PlayOneShot("Ending.wav");
		if (winScreenTimer > 0.0f)
		{
			winScreenTimer -= deltaTime;
		}
		for (int key = 0; key < 256; ++key)
		{
			if (Input::Get().GetKeyDown(key))
			{
				Engine::Get().AddNewLevel<MainMenu>(); // 끝난 화면에서 메인화면으로 복귀.
				break;
			}
		}
		return;
	}
	if (hasLost)
	{
		Engine::Get().PlayBackGroundMusic("GameOver.wav");
		textBox->ShowLines(textBox->GetMessageArt(TextBox::MessageType::GameOver));
		for (int key = 0; key < 256; ++key)
		{
			if (Input::Get().GetKeyDown(key))
			{
				Engine::Get().StopBackGroundMusic();
				Engine::Get().AddNewLevel<MainMenu>(); // 끝난 화면에서 메인화면으로 복귀.
				break;
			}
		}
	}
	// 무적시간 진행
	if(invulnerableTimer> 0.0f)
	{ 
		invulnerableTimer -= deltaTime;
	}

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

	TryPickUpItem();
	TryExit();

	if (!map->IsWall((int)nextX, (int)playerY)) { playerX = nextX; }
	if (!map->IsWall((int)playerX, (int)nextY)) { playerY = nextY; }

	//if (Input::Get().GetKeyDown('E'))
	//{
	//	// 탈출 시도
	//	TryExit();
	//}

	SetPosition(Vector2(static_cast<int>(playerX), static_cast<int>(playerY)));

	CastAllRays(*map, playerX, playerY, playerAngle, viewWidth);
}



void Player::Draw()
{
	if (hasWon)
	{
		ScreenArt screenArt;
		std::vector<std::string> winArt = screenArt.GetArt(ScreenArt::ScreenType::Win);

		int artWidth = static_cast<int>(winArt[0].length());
		int artHeight = static_cast<int>(winArt.size());
		int x = (viewWidth - artWidth) / 2;
		int y = (viewHeight - artHeight) / 2;
		Renderer::Get().Submit(winArt, Vector2(x, y), Color::White);
		return;
	}
	// 거리에 따른 벽 위아래 그리기로 거리감 표현
	for (int x = 0;x < viewWidth;++x)
	{
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
				char ch = GetWallChar(distances[x]);

				Color color;
				if (!TryGetBorderColor(*map, wallHitX[x], wallHitY[x], color))
				{
					color = GetWallColor(distances[x]);
				}
				Renderer::Get().Submit(std::string(1,ch), Vector2(x, y), color);
			}
		}
	}
}