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
}

void MiniMap::Draw()
{
	// 맵 그리기
	map->DrawGrid(startX, startY, cellSize, Color::White);

	// 플레이어 그리기
	map->DrawCell(startX, startY, cellSize, player->GetPosition().x, player->GetPosition().y, 'P', Color::Green);
	// 몬스터 그리기
	map->DrawCell(startX, startY, cellSize, monster->GetPosition().x, monster->GetPosition().y, 'M', Color::Red);

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
