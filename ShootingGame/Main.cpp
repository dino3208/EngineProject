#include <Engine/Engine.h>

// 색깔 표현 헤더
#include <Windows.h>
#include <Math/Color.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

const int screenHeight = 100;
const int screenWidth = 200;



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


const float FOV = DegToRad(60.0f);

float distances[screenWidth];

//void CastAllRays(float playerX, float playerY, float playerAngle)
//{
//	float startAngle = playerAngle - FOV / 2.0f;
//	float angleStep = FOV / screenWidth;
//
//	for (int i = 0; i < screenWidth; ++i)
//	{
//		float rayAngle = startAngle + angleStep * i;
//		distances[i] = CastRay(playerX, playerY, rayAngle);
//	}
//}

void CastAllRays(float playerX, float playerY, float playerAngle)
{
	float angleStep = FOV / screenWidth;
	
	for (int i = 0;i < screenWidth;++i)
	{
		float offset = angleStep * i - FOV / 2.0f;
		float rayAngle = playerAngle + offset;

		float rawDistance = CastRay(playerX, playerY, rayAngle);
		distances[i] = rawDistance * cosf(offset);
	}
}

// Todo: 음영표현
// █░▒▓

//if (distance < 1.0f)
//{
//	return (char)219;
//}
//if (distance < 5.0f)
//{
//	return (char)178;
//}
//if (distance < 8.0f)
//{
//	return (char)176;
//}
//return ' ';

char GetWallChar(float distance)
{
	if (distance < 3.0f)
	{
	return '#';
	}
	if (distance < 6.0f)
	{
		return ':';
	}
	if (distance < 10.0f)
	{
		return '.';
	}
	return ' ';
}

void SetTextColor(Craft::Color color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, static_cast<WORD>(color));
}

Craft::Color GetWallColor(float distance)
{
	if (distance < 3.0f)
	{
		return Craft::Color::BrightWhite;
	}
	if (distance < 6.0f)
	{
		return Craft::Color::White;
	}

	return Craft::Color::Red;
}


void DrawScreen()
{
	for (int y = 0;y < screenHeight;++y)
	{
		for (int x = 0;x < screenWidth;++x)
		{
			int wallHeight = (int)(screenHeight / distances[x]);


			// 너무 가까워서 화면보다 커지는 경우 방지
			if (wallHeight > screenHeight)
			{
				wallHeight = screenHeight;
			}

			int wallTop = (screenHeight / 2) - (wallHeight / 2);
			int wallBottom = (screenHeight / 2) + (wallHeight / 2);

			if (y >= wallTop && y <= wallBottom)
			{
				// 색깔 표현
				//SetTextColor(GetWallColor(distances[x]));
				//std::cout << GetWallChar(distances[x]);
				std::cout << GetWallChar(distances[x]);
			}
			else
			{
				std::cout << ' ';
			}
		}
		std::cout << std::endl;

	}
	std::cout << std::endl;
	// 색깔 표현
	//SetTextColor(Craft::Color::White);
}






int main()
{
	//SetConsoleOutputCP(437); // 음영 표현 문자 1바이트 숫자코드로 표현 

	float playerX = 1.0f;
	float playerY = 1.0f;
	float playerAngle = DegToRad(0.0f);

	CastAllRays(playerX, playerY, playerAngle);
	DrawScreen();
}