#include "Box.h"
#include <Actor/DestroyEffect.h>
#include <Level/GameLevel.h>

namespace Sokoban
{
	using namespace Craft;
	Box::Box(const Vector2& position)
		: Actor("1", position, Color::Red)
	{
		// 박스는 이동이 가능하기 때문에 땅과 겹칠 수 있음.
		// 땅(Ground) 보다 높은 우선순위로 설정
		// 박스는 타겟(목표 지점) 위에 배치될 수 있어야 함
		// 따라서 타겟 보다도 높은 우선순위로 설정
		sortingOrder = 5;
	}


	// 카운트 증가에 맞춰서 숫자 증가
	std::string Box::GetBoxSymbol() const
	{
		return std::to_string(count <= 9 ? count : 9);
	}

	// 상자 카운트 9 초과시 폭발
	void Box::Explode()
	{
		if (GetOwner())
		{
			GetOwner()->SpawnActor<DestroyEffect>(GetPosition());
		}
	}

	// 밀 때마다 숫자 증가
	void Box::OnPushed()
	{
		count++;
		if (count > 9)
		{
			Explode();
		}
		image = GetBoxSymbol();
	}

}
