#pragma once
#include "Actor/Actor.h"
#include "Actor/Map.h"
#include "Algorithm/Node.h"

using namespace Craft;

class Player;
class Monster;

class AStarVisualizer : public Actor
{
public:
	AStarVisualizer();
	~AStarVisualizer();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	Map* map = nullptr;
	Player* player = nullptr;
	Monster* monster = nullptr;

	std::vector<Position> currentPath;

	Position startPoint = Position(-1, -1); // -1,-1은 "아직 지정 안 됨" 표시.
	Position goalPoint = Position(-1, -1);
	std::vector<Position> obstacles;
	int cellSize = 6;

private:
	bool visualizer = false;
};

