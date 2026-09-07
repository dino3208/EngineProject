#include "AStarVisualizer.h"
#include "Actor/Player.h"
#include "Actor/Monster.h"
#include "Actor/Map.h"
#include <Input/Input.h>
#include <Level/Level.h>
#include <Render/Renderer.h>

AStarVisualizer::AStarVisualizer()
	:Actor("", Vector2::Zero, Color::White)
{
	isPauseImmune = true;
}

AStarVisualizer::~AStarVisualizer()
{
}

void AStarVisualizer::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (Input::Get().GetKeyDown('P'))
	{
		// 토글 방식
		visualizer = !visualizer;
		GetOwner()->SetPaused(visualizer);
	}

	if (!visualizer)
	{
		return; // 꺼져 있으면 마우스 입력 처리 안 함.
	}

	Vector2 mousePos = Input::Get().GetMousePosition();
	Position clickedMapPos(mousePos.x / cellSize, mousePos.y / cellSize);

	if (Input::Get().GetKeyDown(VK_LBUTTON))
	{
		startPoint = clickedMapPos;
	}

	if (Input::Get().GetKeyDown(VK_RBUTTON))
	{
		goalPoint = clickedMapPos;
	}
	// Todo: 이해 필요.
	if (Input::Get().GetKeyDown(VK_MBUTTON))
	{
		bool found = false;

		for (size_t i = 0; i < obstacles.size(); ++i)
		{
			if (obstacles[i] == clickedMapPos)
			{
				obstacles.erase(obstacles.begin() + i); // 벡터에서 i번째 항목 제거.
				found = true;
				break;
			}
		}
		if (!found)
		{
			obstacles.emplace_back(clickedMapPos);
		}
	}

	if (!(startPoint == Position(-1, -1)) && !(goalPoint == Position(-1, -1)))
	{
		Map tempMap = *map; // 진짜 맵은 건드리지 않고 복사본 생성.

		for (const Position& obstacle : obstacles)
		{
			tempMap.mapData[obstacle.y][obstacle.x] = '#';
		}

			AStar pathFinder;
		currentPath = pathFinder.FindPath(startPoint, goalPoint, tempMap);
	}
}

void AStarVisualizer::Draw()
{
	if (!visualizer)
	{
		return;
	}

	map->DrawGrid(0, 0, 6, Color::White);
	

	// 플레이어/몬스터 위치 마커로 표시.
	map->DrawCell(0, 0, cellSize, player->GetPosition().x, player->GetPosition().y, 'P', Color::Green);
	map->DrawCell(0, 0, cellSize, monster->GetPosition().x, monster->GetPosition().y, 'M', Color::Red);

	// 경로.
	for (const Position& p : currentPath)
	{
		map->DrawCell(0, 0, cellSize, p.x, p.y, '.', Color::Blue);
	}

	// 시작점.
	if (!(startPoint == Position(-1,-1)))
	{
		map->DrawCell(0, 0, cellSize, startPoint.x, startPoint.y, 'S', Color::Cyan);
	}

	for (const Position obstacle : obstacles)
	{
		map->DrawCell(0, 0, cellSize, obstacle.x, obstacle.y, 'X', Color::Yellow);
	}

	if (!(goalPoint == Position(-1,-1)))
	{
		map->DrawCell(0, 0, cellSize, goalPoint.x, goalPoint.y, 'E', Color::Purple);
	}
}
