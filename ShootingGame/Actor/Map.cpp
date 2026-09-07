#include "Actor/Map.h"
#include <Render/Renderer.h>

#include <fstream>

using namespace Craft;

bool Map::LoadFile(const std::string& filePath) // 호출한 쪽의 문자열을 복사하지 않고, 그대로 씀.
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		return false; // 파일 못 찾을 경우
	}

	mapData.clear(); // LoadFile을 다시 호출해서 다른 맵 불러올 때 예전 맵 데이터 안 섞이기 위한 초기화

	std::string line;
	while (std::getline(file, line))
	{
		mapData.push_back(line);
	}

	return true;
}

// 벽인지 확인
bool Map::IsWall(int x, int y) const
{
	// 맵 사이즈를 초과하는 것을 방지
	if (y < 0 || y >= (int)mapData.size())
	{
		return true;
	}
	if (x < 0 || x >= (int)mapData[y].length())
	{
		return true;
	}

	// 벽이거나 문이면 true, 아니면 false를 반환
	return mapData[y][x] == '#';
}

void Map::DrawGrid(int startX, int startY, int cellSize, Color color) const
{
	std::vector<std::string> enlargedMap;

	for (int y = 0;y < static_cast<int>(mapData.size());++y)
	{
		std::string enlargedRow;

		for (int x = 0; x < static_cast<int>(mapData[y].length()); ++x)
		{
			enlargedRow += std::string(cellSize, mapData[y][x]); // 한 글자를 cellSize번 반복해서 이어붙이기.
		}

		for (int i = 0; i < cellSize; ++i)
		{
			enlargedMap.emplace_back(enlargedRow); // 이 줄 자체를 세로로도 cellSize번 반복
		}
	}

	Renderer::Get().Submit(enlargedMap, Vector2(startX, startY), color); // 반복문이 다 끝난 뒤 딱 한 번만 호출.
}

void Map::DrawCell(int startX, int startY, int cellSize, int cellX, int cellY, char ch, Color color) const
{
	int baseX = startX + cellX * cellSize;
	int baseY = startY + cellY * cellSize;

	std::string block(cellSize, ch);

	for (int i = 0; i < cellSize; ++i)
	{
		Renderer::Get().Submit(block, Vector2(baseX, baseY + i), color);
	}
}


