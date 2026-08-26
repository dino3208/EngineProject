#include "Player.h"
#include <Input/Input.h>
#include <Render/Renderer.h>

std::vector<std::string> mapData =
{
	"########",
	"#..D...#",
	"#..##..#",
	"#....K.#",
	"########"
};


const int screenHeight = 80;
const int screenWidth = 240;


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

	return mapData[y][x] == '#' || mapData[y][x] == 'D';
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

bool hasKey = false;


void TryOpenDoor(float playerX, float playerY, float playerAngle)
{
	if (!hasKey)
	{
		return;
	}
	int frontX = (int)(playerX + cosf(playerAngle));
	int frontY = (int)(playerY + sinf(playerAngle));

	if (frontY < 0 || frontY >= (int)mapData.size()) { return; }
	if (frontX < 0 || frontX >= (int)mapData[frontY].length()) { return; }

	if (mapData[frontY][frontX] == 'D')
	{
		mapData[frontY][frontX] = '.';
	}
}

void TryPickUpItem(float playerX, float playerY)
{
	int x = (int)playerX;
	int y = (int)playerY;

	if (y < 0 || y >= (int)mapData.size()) { return; }
	if (x < 0 || x >= (int)mapData[y].length()) { return; }

	if (mapData[y][x] == 'K')
	{
		hasKey = true;
		mapData[y][x] = '.';
	}
}



using namespace Craft;

void Player::BeginPlay()
{
}

void Player::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	const float rotSpeed = DegToRad(90.0f);
	const float moveSpeed = 3.0f;

	if (Input::Get().GetKey('A'))
	{
		playerAngle -= rotSpeed * deltaTime;
	}
	if (Input::Get().GetKey('D'))
	{
		playerAngle += rotSpeed * deltaTime;
	}

	float moveX = 0.0f;
	float moveY = 0.0f;

	if (Input::Get().GetKey('W'))
	{
		moveX = cosf(playerAngle) * moveSpeed * deltaTime;
		moveY = sinf(playerAngle) * moveSpeed * deltaTime;
	}
	if (Input::Get().GetKey('S'))
	{
		moveX = -cosf(playerAngle) * moveSpeed * deltaTime;
		moveY = -sinf(playerAngle) * moveSpeed * deltaTime;
	}

	float nextX = playerX + moveX;
	float nextY = playerY + moveY;

	if (!IsWall((int)nextX, (int)playerY)) {playerX = nextX;}
	if (!IsWall((int)playerX, (int)nextY)) {playerY = nextY;}

	if (Input::Get().GetKeyDown('E'))
	{
		TryPickUpItem(playerX, playerY);
		TryOpenDoor(playerX, playerY, playerAngle);
	}

	CastAllRays(playerX, playerY, playerAngle);
	
}

void Player::Draw()
{
	for (int x = 0;x < screenWidth;++x)
	{
		int wallHeight = (int)(screenHeight / distances[x]);
		if (wallHeight > screenHeight)
		{
			wallHeight = screenHeight;
		}

		int wallTop = (screenHeight / 2) - (wallHeight / 2);
		int wallBottom = (screenHeight / 2) + (wallHeight / 2);

		for (int y = 0;y < screenHeight;++y)
		{
			if (y >= wallTop && y <= wallBottom)
			{
				std::string ch(1, GetWallChar(distances[x]));
				Renderer::Get().Submit(ch, Vector2(x, y), GetWallColor(distances[x]));
			}
		}
	}
}



//---------------------------------------------------------------------------------

