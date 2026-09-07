#pragma once

#include "Actor/Map.h"
#include "Algorithm/Node.h"
#include <vector>


class AStar
{
private:
	// 맵데이터를 그리드로 변환하는 함수. 
	std::vector<std::vector<int>> ConvertMapToGrid(const Map& map);

	// 이동 방향과 해당 방향의 이동 비용.
	struct Direction
	{
		int x = 0;
		int y = 0;
		float cost = 0.0f;
	};

	// 그리드 타일의 종류. (열거형)
	// char 타입 -> 문자 타입 -> 숫자.
	// 문자 타입으로 활용 / 1 byte 크기를 할당하고 싶을 때 많이 활용.
	enum class TileType : char
	{
		Ground=0,
		Wall=1,
		Start=2,
		Goal=3,
		Visited=5,
	};

public:
	AStar();
	~AStar();

	// 메세지 (객체 지향 프로그래밍 용어) - 공개 메소드 (인터페이스).

	// 경로 탐색 요청 함수.
	std::vector<Position> FindPath(
		const Position& startPosition,
		const Position& goalPosition,
		const Map& map);
	// 탐색한 최종 경로를 출력하는 함수.
	void DisplayGridWithPath(
		std::vector<std::vector<int>>& grid,
		const std::vector<Position>& path);
	
private:
	// 메소드: 클래스 또는 구조체가 가지는 함수 / 공개되지 않은 함수.

	// 이전 탐색에 사용한 정보 및 노드를 정리하는 함수.
	void Clear();

	// 노드 생성 전문 함수.
	Node* CreateNode(
		const Position& position, 
		Node* parent = nullptr);
	
	// 부모 노드를 역추적해서 경로를 구성하는 함수.
	std::vector<Position> ConstructPath(
		Node* destination);

	// 휴리스틱(추정비용) 계산 함수.
	float CalculateHeuristic(
		const Position& current,
		const Position& goal) const;

	// 그리드 검증 함수.
	bool IsValidGrid(
		const std::vector<std::vector<int>>& grid) const;

	// 이동하려는 좌표가 유효한 지 확인.
	bool IsInRange(
		int x,
		int y,
		const std::vector<std::vector<int>>& grid) const;

	// 옵션: 대각선 이동이 장애물 모서리를 통과하는 지 확인.
	bool IsDiagonalBlocked(
		const Position& current,
		const Direction& direction,
		const std::vector<std::vector<int>>& grid) const;

	// Open 목록 및 Closed 목록 검사 함수.
	Node* FindOpenNode(int x, int y) const;
	bool IsInClosedList(int x, int y) const;

	// 목적지인지 확인하는 함수.
	bool IsDestination(const Node* node) const;

	// 이전 탐색 표시를 지우고 그리드를 출력하는 함수.
	void ClearVisualization(std::vector<std::vector<int>>& grid) const;
	void DisplayGrid(std::vector<std::vector<int>>& grid) const;

	// 동적 할당된 모든 노드를 소유하는 목록.
	// 메모리 관리를 위해 사용.
	std::vector<Node*> allocatedNodes;

	// 탐색할 노드 목록/탐색을 마친 노드 목록.
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	// 시작 노드/목표 노드.
	Node* startNode = nullptr;
	Node* goalNode = nullptr;
};

