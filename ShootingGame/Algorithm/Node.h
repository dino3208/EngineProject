#pragma once

// 그리드의 좌표를 저장할 클래스.
class Position
{
public:
	Position(int x = 0, int y = 0)
		:x(x), y(y)
	{
	}

	// 동등 비교 연산자 오버로딩.
	bool operator==(const Position& other) const
	{
		return x == other.x && y == other.y;
	}

	// 가로/세로 위치.
	int x = 0;
	int y = 0;
};

// A* 탐색에 필요한 노드 클래스.
class Node
{
public:
	Node(const Position& position, Node* parent = nullptr)
		: position(position), parent(parent)
	{

	}

	// 노드의 위치 정보.
	Position position;

	// 노드의 비용 정보.
	float gCost = 0.0f;
	float hCost = 0.0f;
	float fCost = 0.0f;

	// 최종 경로를 역추적할 때 사용할 부모 노드.
	Node* parent = nullptr;
};