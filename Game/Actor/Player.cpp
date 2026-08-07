#include "Player.h"


using namespace Craft;
Player::Player(const Vector2& position)
	: Actor("P", position, Color::Green)
{
	// 우선순위 설정
	// 액터 중에서 가장 높은 값
	sortingOrder = 5;
}
