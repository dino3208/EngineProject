#include "Wall.h"

namespace Sokoban
{
	using namespace Craft;

	Wall::Wall(const Vector2& position)
		: Actor("#", position, Color::White)
	{
		// 그리기 우선순위 지정
		sortingOrder = 0;
	}
}