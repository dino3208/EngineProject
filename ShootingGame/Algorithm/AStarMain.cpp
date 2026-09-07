#include <iostream>
#include <vector>
#include <Windows.h>

#include "Algorithm/AStar.h"


// 맵에서 시작/목표 위치 찾는 함수
bool FindStartAndGoalPosition(
	const std::vector<std::vector<int>>& grid,
	Position& outStartPosition,
	Position& outGoalPosition);

int main()
{
	// 애니메이션 출력 중 콘솔 커서를 감춘다.
	CONSOLE_CURSOR_INFO cursorInfo = { };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	// 0은 빈 공간, 1은 장애물, 2는 시작점, 3은 목표점이다.
	std::vector<std::vector<int>> grid =
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 3, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	// 그리드에서 시작 위치와 목표 위치를 찾는다.
	Position startPosition;
	Position goalPosition;

	if (!FindStartAndGoalPosition(grid, startPosition, goalPosition))
	{
		std::cout << "시작 위치 또는 목표 위치가 올바르지 않습니다.\n";

		cursorInfo.bVisible = TRUE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

		std::cin.get();
		return 0;
	}

	// A* 객체를 생성하고 경로를 탐색한다.
	AStar aStar;
	std::vector<Position> path = aStar.FindPath(startPosition, goalPosition, grid);

	if (!path.empty())
	{
		// 탐색한 좌표와 최종 경로를 출력한다.
		std::cout << "\n경로를 찾았습니다.\n최단 경로:\n";

		for (const Position& position : path)
		{
			std::cout << "(" << position.x << ", " << position.y << ") -> ";
		}

		std::cout << "목표 도착\n";
		std::cout << "경로를 맵에 표시한 결과:\n";
		aStar.DisplayGridWithPath(grid, path);
	}
	else
	{
		std::cout << "경로를 찾지 못했습니다.\n";
	}

	// 같은 AStar 객체로 FindPath를 다시 호출할 수 있다.
	// 다음 탐색을 시작하면 이전 탐색 상태가 자동으로 정리된다.

	// 프로그램을 종료하기 전에 콘솔 커서를 다시 표시한다.
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	COORD consolePosition = { 0, 30 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), consolePosition);

	std::cin.get();
	return 0;
}

bool FindStartAndGoalPosition(
	const std::vector<std::vector<int>>& grid,
	Position& outStartPosition,
	Position& outGoalPosition)
{
	// 빈 그리드에서는 시작점과 목표점을 찾을 수 없다.
	if (grid.empty() || grid[0].empty())
	{
		return false;
	}

	// 시작점과 목표점의 중복 여부를 확인한다.
	bool foundStart = false;
	bool foundGoal = false;

	for (int y = 0; y < static_cast<int>(grid.size()); ++y)
	{
		for (int x = 0; x < static_cast<int>(grid[y].size()); ++x)
		{
			if (grid[y][x] == 2)
			{
				// 시작 위치는 정확히 하나만 허용한다.
				if (foundStart)
				{
					return false;
				}

				outStartPosition = Position(x, y);
				foundStart = true;
			}
			else if (grid[y][x] == 3)
			{
				// 목표 위치도 정확히 하나만 허용한다.
				if (foundGoal)
				{
					return false;
				}

				outGoalPosition = Position(x, y);
				foundGoal = true;
			}
		}
	}

	return foundStart && foundGoal;
}