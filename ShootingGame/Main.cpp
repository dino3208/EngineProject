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

const float PI = 3.14159265f;

float DegToRad(float degree)
{
	return degree * (PI / 180.0f);
}

float CastRay(float startX, float startY, float angle)
{
	const float step = 0.05f;
	const float maxDistance = 20.0f;

	float rayX = startX;
	float rayY = startY;
	float distance = 0.0f;

	while (distance < maxDistance)
	{
		rayX += cosf(angle) * step;
		rayY += sinf(angle) * step;
		distance += step;

		if (IsWall((int)rayX, (int)rayY))
		{
			return distance;
		}
	}

	return maxDistance;
}

const int screenWidth = 40;
const float FOV = DegToRad(60.0f);

float distances[screenWidth];

void CastAllRays(float playerX, float playerY, float playerAngle)
{
	float startAngle = playerAngle - FOV / 2.0f;
	float angleStep = FOV / screenWidth;

	for (int i = 0; i < screenWidth; ++i)
	{
		float rayAngle = startAngle + angleStep * i;
		distances[i] = CastRay(playerX, playerY, rayAngle);
	}
}


int main()
{
	float playerX = 1.0f;
	float playerY = 1.0f;
	float playerAngle = DegToRad(0.0f);

	CastAllRays(playerX, playerY, playerAngle);
	
	for (int i = 0;i < screenWidth;++i)
	{
		std::cout << "distances[" << i << "]: " << distances[i] << std::endl;
	}
}