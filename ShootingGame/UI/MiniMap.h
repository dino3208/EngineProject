#pragma once
#include "Actor/Actor.h"
#include "Actor/Map.h"
#include "Actor/Player.h"
#include "Actor/Monster.h"

using namespace Craft;

class MiniMap : public Actor
{
public:
	MiniMap();
	~MiniMap();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	Map* map = nullptr;
	Player* player = nullptr;
	Monster* monster = nullptr;

	int startX = 0;
	int startY = 0;

	int cellSize = 4;

	// 일정 주기로 몬스터만 표시되게 만드는 데 필요한 변수.
	float blipTimer = 0.0f;
	const float blipInterval = 3.0f;
	const float blipVisibleDuration = 0.5f;

};

