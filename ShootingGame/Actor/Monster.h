#pragma once
#include "Actor/Actor.h"
#include "Actor/Map.h"
#include "Actor/Player.h"
#include "Algorithm/Astar.h"

using namespace Craft;

class Monster : public Actor
{
	TYPE_DECLARATIONS(Monster, Actor)
public:
	Monster();
	~Monster();

	void SetSpawnPosition(float x, float y);
	// 플레이어와 접촉시 잠시 정지.
	void Stun(float duration);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	Map* map = nullptr;

	// 플레이어를 추적해야 하니까 필요.
	Player* player = nullptr;

	// AStar 알고리즘을 사용.
	AStar pathFinder;

protected:
	// 몬스터 좌표
	float monsterX = 0.0f;
	float monsterY = 0.0f;

	// 움직임 관련 시간.
	float moveTimer = 0.0f;
	const float moveInterval = 2.0f;

	// 정지 시간.
	float stunTimer = 0.0f;

	// FindPath가 돌려주는 std::vector<Position>을 저장해 둘 멤버.
	std::vector<Position> currentPath;
	// 경로에서 몇번째 칸으로 가고 있는 지 세는 변수.
	int pathIndex = 0;

};

