#define NOMINMAX
#include "Actor/Map.h"
#include "Algorithm/AStar.h"
#include <algorithm>
#include <iostream>

#include <Windows.h>


std::vector<std::vector<int>> AStar::ConvertMapToGrid(const Map& map)
{
    std::vector<std::vector<int>> grid;

    for (const std::string& row : map.mapData) // 맵의 한 줄(문자열)씩 순회.
    {
        std::vector<int> gridRow; // 이번 줄을 숫자로 바꿔서 담을 벡터.

        for (char tile : row)  // 그 줄의 글자를 한 글자씩 순회.
        {
            if (tile == '#')
            {
                gridRow.emplace_back((int)AStar::TileType::Wall);
            }
            else
            {
                gridRow.emplace_back((int)AStar::TileType::Ground);
            }
        }
        grid.emplace_back(gridRow);
    }
    return grid;
}

AStar::AStar()
{
}

AStar::~AStar()
{
    // 초기화.
    Clear();
}

// 최적화.
std::vector<Position> AStar::FindPath(
    const Position& startPosition, 
    const Position& goalPosition, 
    const Map& map)
{
    // 이전에 탐색한 결과 초기화.
    Clear();

    // 그리드에 입력할 값을 맵데이터 변환 값으로 저장.
    std::vector<std::vector<int>> grid = ConvertMapToGrid(map);

    // 예외 처리.
    if (!IsValidGrid(grid))
    {
        // 유효하지 않으면 빈 배열 반환.
        return { };
    }

    // 시작위치/목표위치가 grid 기준에서 문제 없는 위치값인지 확인.
    if (!IsInRange(startPosition.x, startPosition.y, grid)
        || !IsInRange(goalPosition.x, goalPosition.y, grid))
    {
        return { };
    }
    // 시작위치/목표위치가 이동 불가하면 종료.
    if (grid[startPosition.y][startPosition.x] == (int)TileType::Wall
        || grid[goalPosition.y][goalPosition.x] == (int)TileType::Wall)
    {
        return { };
    }

    // 이전 탐색 과정의 시각화 제거(기존에 방문 처리한 값이 있으면 제거).
    ClearVisualization(grid);

    // 탐색 시작,
    // 시작/목표 노드 생성.
    startNode = CreateNode(startPosition);
    goalNode = CreateNode(goalPosition);

    // 시작 노드의 비용 계산 및 openList에 추가해 탐색 시작.
    startNode->gCost = 0.0f;
    startNode->hCost = CalculateHeuristic(startPosition, goalPosition);
    startNode->fCost = startNode->gCost + startNode->hCost;

    // 편의를 위해 사전 비용 설정.
    const float diagonalCost = 1.14121f;
    const std::vector<Direction> directions =
    {
        //Direction { 0, -1, 1.0f }, 
        //Direction(0, -1, 1.0f)
        { 0, -1, 1.0f }, { 0, 1, 1.0f },    // 상하.
        { -1, 0, 1.0f }, { 1, 0, 1.0f },    // 좌우.
        { -1, -1, diagonalCost },           // 좌상단 방향.
        {  1, -1, diagonalCost },           // 우상단 방향.
        { -1,  1, diagonalCost },           // 좌하단 방향.
        {  1,  1, diagonalCost },           // 우하단 방향.
    };

    openList.emplace_back(startNode);

    // openList가 빌 때까지 탐색 반복.
    while (!openList.empty())
    {
        // openList에서 fCost가 가장 작은 노드를 선택.
        // 이진힙을 사용하면 최적화 가능. // Todo:
        Node* currentNode = openList[0];
        for (Node* node : openList)
        {
            // 더 작은 비용의 노드 검색.
            if(node->fCost < currentNode->fCost
                || (node->fCost == currentNode->fCost && node->hCost < currentNode->hCost))
            {
                currentNode = node;
            }
        }
        // 목표 노드인지 확인.
        if (IsDestination(currentNode))
        {
            // 이동 경로 제작 후 반환.
            return ConstructPath(currentNode);
        }

        // 현재 노드를 openList에서 제거.
        // 방문 처리를 위해.
        auto iterator = std::find(openList.begin(), openList.end(), currentNode);

        // 검색에 성공했는지 확인.
        if (iterator != openList.end())
        {
            // openList에서 제거.
            openList.erase(iterator);
        }

        // 탐색을 마친 노드를 closedList에 추가.
        closedList.emplace_back(currentNode);

        // 현재 위치를 기준으로 주변(8방향)의 이웃노드를 탐색.
        for (const Direction& direction : directions)
        {
            // 현재 노드를 기준으로 인접한 노드의 좌표 계산.
            // 새로운 좌표(위치) = 현재 위치 + 이동 방향.
            int newX = currentNode->position.x + direction.x;
            int newY = currentNode->position.y + direction.y;

            // 예외 처리.
            if (!IsInRange(newX, newY, grid))
            {
                continue;
            }

            // 새로운 위치가 장애물인지 확인.
            if (grid[newY][newX] == (int)TileType::Wall)
            {
                continue;
            }

            // 대각선 이동시 장애물을 통과하는지 확인.
            if (IsDiagonalBlocked(currentNode->position, direction, grid))
            {
                continue;
            }
            
            // 이미 방문한 곳이라면 건너뛰기.
            if (IsInClosedList(newX, newY))
            {
                continue;
            }

            // 현재 노드를 거쳐서 새로운 위치로 가는데 드는 비용 계산.
            float newGCost = currentNode->gCost + direction.cost;

            // 이미 openList에 있는데 비용면에서 더 나은지 확인.
            Node* openNode = FindOpenNode(newX, newY);
            if (openNode)
            {
                // 비용 비교.
                if (newGCost < openNode->gCost)
                {
                    openNode->gCost = newGCost;
                    openNode->fCost = openNode->gCost + openNode->hCost;
                    openNode->parent = currentNode;
                }
                continue;
            }

            // 이웃 노드 생성 및 openList에 추가.
            Node* neighborNode = CreateNode(Position(newX, newY), currentNode);

            // 새로운 노드의 비용 계산.
            neighborNode->gCost = newGCost;
            neighborNode->hCost = CalculateHeuristic(
                neighborNode->position, goalNode->position);
            neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

            // 새로운 노드를 openList에 추가.
            openList.emplace_back(neighborNode);

            // 옵션: 시각화를 위한 처리.
            if (grid[newY][newX] == (int)TileType::Ground)
            {
                grid[newY][newX] = (int)TileType::Visited;
            }
            // grid 그리기.
            //DisplayGrid(grid);

            // 스레드 재우기 (애니메이션처럼 단순하게 프레임을 만들기 위해).
            DWORD delay = static_cast<DWORD>(0.1f * 1000);
            //Sleep(delay);
        }
    }
    // 빈 경로 반환 (탐색 실패).
    return { };
}

void AStar::DisplayGridWithPath(
    std::vector<std::vector<int>>& grid, 
    const std::vector<Position>& path)
{
    // 기존에 시각화를 위해 사용했던 값 복구.
    ClearVisualization(grid);

    // 맵 그리기.
    DisplayGrid(grid);
    
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int green = FOREGROUND_GREEN;


    // 이동경로 그리기.
    for (const Position& position : path)
    {
        // 경로 위치의 타일 값 읽기.
        int value = grid[position.y][position.x];

        // 시작 목표 위치는 경로 표시에서 건너뛰기.
        if (value == (int)TileType::Start
            || value == (int)TileType::Goal)
        {
            continue;
        }

        // 콘솔 좌표.
        COORD cursorPosition;
        cursorPosition.X = static_cast<short>(position.x * 2);
        cursorPosition.Y = static_cast<short>(position.y);

        // 커서 이동.
        SetConsoleCursorPosition(handle, cursorPosition);

        // 텍스트 색 지정.
        SetConsoleTextAttribute(handle, green);

        // 글자 출력.
        std::cout << "* ";

        // 스레드 재우기.
        DWORD delay = static_cast<DWORD>(0.1f * 1000);
        Sleep(delay);
    }

}

void AStar::Clear()
{
    // 탐색 과정에서 생성했던 모든 노드 삭제 및 해제.
    for (Node*& node : allocatedNodes)
    {
        delete node;
        node = nullptr;
    }

    allocatedNodes.clear();
    openList.clear();
    closedList.clear();

    startNode = nullptr;
    goalNode = nullptr;
}

Node* AStar::CreateNode(const Position& position, Node* parent)
{   // 노드를 생성하고, allocatedNodes에 추가.
    Node* newNode = new Node(position, parent);
    allocatedNodes.emplace_back(newNode);

    // 생성한 노드 반환.
    return newNode;
}

std::vector<Position> AStar::ConstructPath(Node* destination)
{
    // 목표 노드로부터 부모 노드를 따라 경로 역추적.
    std::vector<Position> path;
    Node* current = destination;
    while (current)
    {
        // 현재 노드는 경로 배열에 추가.
        path.emplace_back(current->position);

        // 부모 노드로 이동해서 경로를 역추적.
        current = current->parent;
    }

    // 루프가 종료되면 path에는 반대 방향의 경로 정보가 저장된다.
    // 따라서 다시 역방향으로 뒤집기가 필요.
    std::reverse(path.begin(), path.end());
    return path;
}

float AStar::CalculateHeuristic(const Position& current, const Position& goal) const
{
    // 옥타일(8방향) 비용 계산법.
    // 대각선 이동 허용시 주의사항.
    // -> 대각선 형태의 장애물을 뚫고 가지 못하게 막아야 한다.

    // 현재 위치와 목표 위치 사이의 차이 계산.
    // std::abs -> 절대값 함수(absolute).
    int diffX = std::abs(current.x - goal.x);
    int diffY = std::abs(current.y - goal.y);

    // 대각선 거리와 남은 직선 거리 분리.
    int diagonalDistance = std::min(diffX, diffY);
    int straightDistance = std::max(diffX, diffY) - diagonalDistance;

    // 대각선 비용.
    const float diagonalCost = 1.1421f;
    const float straightCost = 1.0f;

    return diagonalDistance * diagonalCost + straightDistance * straightCost;
}

bool AStar::IsValidGrid(const std::vector<std::vector<int>>& grid) const
{
    // 그리드가 비어있다면 유효하지 않음.
    if (grid.empty())
    {
        return false;
    }

    // 행마다 길이가 같은지 확인.
    size_t width = grid[0].size();
    for (const std::vector<int>& row : grid)
    {
        // 앞에서 구한 행의 길이와 다른 행이 나타나면 유효하지 않다고 판정한다.
        if (row.size() != width)
        {
            return false;
        }
    }

    // 검사를 통과하면 유효하다.
    return true;
}

bool AStar::IsInRange(
    int x, int y, 
    const std::vector<std::vector<int>>& grid) const
{
    // grid의 가로 크기는 같다고 가정.
    return x >= 0 && x < static_cast<int>(grid[0].size())
        && y >= 0 && y < static_cast<int>(grid.size());
}

bool AStar::IsDiagonalBlocked(
    const Position& current, 
    const Direction& direction, 
    const std::vector<std::vector<int>>& grid) const
{
    // 이동하려는 방향에 장애물이 있는 지 확인한다.
    // 대각선 성분만 판단한다.
    // 대각선 성분이 아니라면 판단할 필요가 없다.
    // 대각선 방향의 x,y 성분은 모두 0이 아니기 때문이다.
    if (direction.x == 0 || direction.y == 0)
    {
        return false;
    }

    // 대각선으로 이동하려는 새로운 위치의 x성분과 y성분을 분해한다.
    int sideX = current.x + direction.x;
    int sideY = current.y + direction.y;

    // 대각선 이동 성분 위치 중 하나라도 장애물(벽)이 있으면 이동 불가하다.
    return 
        grid[current.y][sideX] == (int)TileType::Wall 
        || grid[sideY][current.x] == (int)TileType::Wall;
}

Node* AStar::FindOpenNode(int x, int y) const
{
    // 같은 좌표의 노드를 OpenList에서 찾기.
    for (Node* node : openList)
    {
        // 좌표 비교.
        if (node->position == Position(x, y))
        {
            return node;
        }
    }
    return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
    // 같은 좌표가 ClosedList에 있는 지 확인.
    for (Node* node : closedList)
    {
        // 좌표 비교.
        if (node->position == Position(x, y))
        {
            return true;
        }
    }
    return false;
}

bool AStar::IsDestination(const Node* node) const
{
    // 두 노드 모두 null이 아니고, 두 노드의 위치가 같은 지 비교한다.
    return
        (node != nullptr) 
        && (goalNode != nullptr)
        && node->position == goalNode->position;
}

void AStar::ClearVisualization(std::vector<std::vector<int>>& grid) const
{
    // 탐색 후보 표시해둔 것을 다시 원상복구한다.
    // 탐색 후보로 표시해뒀다는 건 원래 이동 가능한 위치였다는 것을 의미한다.
    for (std::vector<int>& row : grid)
    {
        for(int& value : row)
        {
            if (value == (int)TileType::Visited)
            {
                // 숫자로 0.
                value = (int)TileType::Ground;
            }
        }
    }
}

void AStar::DisplayGrid(std::vector<std::vector<int>>& grid) const
{
    // 커서를 원점(0,0)으로 이동.
    static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static COORD origin = { 0,0 };
    SetConsoleCursorPosition(handle, origin);

    // 글자 색.
    int red = FOREGROUND_RED;
    int green = FOREGROUND_GREEN;
    int white = red | green | FOREGROUND_BLUE;

    // grid를 순회하면서 글자 그리기.
    for (int y = 0;y < (int)grid.size(); ++y)
    {
        for (int x = 0;x < (int)grid[y].size(); ++x)
        {
            // 타일값에 따라 글자 색상 및 글자 지정해서 출력한다.
            // 타일값에 따라 글자 색상 및 글자 지정해서 출력.
            if (grid[y][x] == (int)TileType::Start)
            {
                SetConsoleTextAttribute(handle, red);
                std::cout << "S ";
            }
            else if (grid[y][x] == (int)TileType::Goal)
            {
                SetConsoleTextAttribute(handle, red);
                std::cout << "G ";
            }
            else if (grid[y][x] == (int)TileType::Wall)
            {
                SetConsoleTextAttribute(handle, white);
                std::cout << "1 ";
            }
            else if (grid[y][x] == (int)TileType::Visited)
            {
                SetConsoleTextAttribute(handle, green);
                std::cout << "+ ";
            }
            else
            {
                SetConsoleTextAttribute(handle, white);
                std::cout << "0 ";
            }
        }
        // 한 라인(행) 출력이 마무리되면 개행 출력.
        std::cout << "\n";
    }
}


