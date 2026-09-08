#include "UI/MiniMap.h"
#include <Render/Renderer.h>


MiniMap::MiniMap()
	:Actor("", Vector2::Zero, Color::White)
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	blipTimer += deltaTime;
	if (blipTimer >= blipInterval)
	{
		blipTimer -= blipInterval; // 인터벌 이상 시간이면 인터벌을 빼 초기화하는 방식.
	}
}

void MiniMap::Draw()
{
	// 맵 그리기
	map->DrawGrid(startX, startY, cellSize, Color::White);

	// 방향별 벽 색깔 그리기
	for (int x = 0; x < static_cast<int>(map->mapData[0].length()); ++x)
	{
		map->DrawCell(startX, startY, cellSize, x, 0, '#', Color::Red);
	}
	int endY = static_cast<int>(map->mapData.size() - 1);
	for (int x = 0; x < static_cast<int>(map->mapData[endY].length()); ++x)
	{
		map->DrawCell(startX, startY, cellSize, x, endY, '#', Color::Yellow);
	}
	for (int y = 0; y < static_cast<int>(map->mapData.size()); ++y)
	{
		map->DrawCell(startX, startY, cellSize, 0, y, '#', Color::Green);
	}
	int lastX = static_cast<int>(map->mapData[0].length() - 1);
	for (int y = 0;y < static_cast<int>(map->mapData.size()); ++y)
	{
		map->DrawCell(startX, startY, cellSize, lastX, y, '#', Color::Blue);
	}
	// 골 지점
	map->DrawCell(startX, startY, cellSize, 23, 20, 'G', Color::Purple);

	// 플레이어 그리기
	map->DrawCell(startX, startY, cellSize, player->GetPosition().x, player->GetPosition().y, 'P', Color::Green);
	// 몬스터 그리기
	if(blipTimer < blipVisibleDuration)
	{
		map->DrawCell(startX, startY, cellSize, monster->GetPosition().x, monster->GetPosition().y, 'M', Color::Red);
	}

	// 키 그리기
	for (int y = 0; y < static_cast<int>(map->mapData.size()); ++y)
	{
		for (int x = 0;x < static_cast<int>(map->mapData[y].length());++x)
		{
			if (map->mapData[y][x] == 'K')
			{
				map->DrawCell(startX, startY, cellSize, x, y, 'K', Color::Yellow);
			}
		}
	}
}
