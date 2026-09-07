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

};

