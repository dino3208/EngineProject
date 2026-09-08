#include <Engine/Engine.h>

#include "Actor/Monster.h"
#include <Render/Renderer.h>

using namespace Craft;

Monster::Monster()
	:Actor("M", Vector2::Zero, Color::Red)
{
}

Monster::~Monster()
{
}

void Monster::SetSpawnPosition(float x, float y)
{
	monsterX = x;
	monsterY = y;
	SetPosition(Vector2(static_cast<int>(x), static_cast<int>(y)));
	SavePreviousState();
}

void Monster::Stun(float duration)
{
	stunTimer = duration;
}

void Monster::BeginPlay()
{

}

// 플레이어와의 거리에 따라 이동 주기 결정 -> 멀면 빠르게, 가까우면 느리게
float GetMoveInterval(float distance)
{
	if (distance > 7.0f) { return 0.5f; }
	if (distance > 4.0f) { return 1.5f; }
	return 3.0f;
}

void Monster::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (stunTimer > 0.0f)
	{
		stunTimer -= deltaTime;
		return; // 이동 로직 전부 건너뛰기.
	}

	float dx = player->GetPosition().x - monsterX;
	float dy = player->GetPosition().y - monsterY;
	float distance = sqrtf(dx * dx + dy * dy); // sqrtf-> 제곱근을 구하는 함수.

	// 거리 안에 들어오면 소리 재생.
	if (distance <= 3.0f)
	{
		if (!isNearby)
		{
		Engine::Get().PlayOneShot("Roar.wav");
		isNearby = true;
		}
	}
	else
	{
		isNearby = false;
	}

	moveTimer += deltaTime;
	if (moveTimer >= GetMoveInterval(distance))
	{
		moveTimer -= GetMoveInterval(distance);

		Position Start((int)monsterX, (int)monsterY);
		Position Goal(player->GetPosition().x, player->GetPosition().y); // 플레이어의 현재 위치.

		currentPath = pathFinder.FindPath(Start, Goal, *map);

		if (currentPath.size() >= 2)
		{
			monsterX = (float)currentPath[1].x;
			monsterY = (float)currentPath[1].y;
		}
	}

	SetPosition(Vector2(static_cast<int>(monsterX), static_cast<int>(monsterY)));

}