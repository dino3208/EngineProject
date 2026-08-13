#include "Target.h"

namespace Sokoban
{
	using namespace Craft;
	Target::Target(const Vector2& position)
		: Actor("T", position, Color::Blue)
	{
		// 우선 순위 설정
		// 바닥보다는 크고 박스 및 플레이어 보다는 작은 값
		sortingOrder = 3;
	}
}
