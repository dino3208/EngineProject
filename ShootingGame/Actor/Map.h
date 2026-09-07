#pragma once
#include <Actor/Actor.h>

#include <string>
#include <vector>

using namespace Craft;

class Map
{
public:
	bool LoadFile(const std::string& filepath);

	std::vector<std::string> mapData; // 배열

	bool IsWall(int x, int y) const;

	void DrawGrid(int startX, int startY, int cellSize, Color color) const;

	void DrawCell(int startX, int startY, int cellSize, int cellX, int cellY, char ch, Color color) const;
};

