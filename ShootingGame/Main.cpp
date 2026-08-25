#include <Engine/Engine.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

const std::vector<std::string> mapData =
{
	"########",
	"#......#",
	"#..##..#",
	"#......#",
	"########"
};

bool IsWall(int x, int y)
{
	if (y < 0 || y >= (int)mapData.size())
	{
		return true;
	}
	if (x < 0 || x >= (int)mapData[y].length())
	{
		return true;
	}

	return mapData[y][x] == '#';
}

int main()
{
	int playerX = 3;
	int playerY = 2;

	bool isWall = IsWall(playerX, playerY);

	std::cout << "isWall: " << isWall << std::endl;
}