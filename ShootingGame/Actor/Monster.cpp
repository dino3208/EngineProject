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
}

void Monster::Stun(float duration)
{
	stunTimer = duration;
}

void Monster::BeginPlay()
{

}

void Monster::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (stunTimer > 0.0f)
	{
		stunTimer -= deltaTime;
		return; // 이동 로직 전부 건너뛰기.
	}
	moveTimer += deltaTime;
	if (moveTimer >= moveInterval)
	{
		moveTimer -= moveInterval;

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