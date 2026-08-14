#pragma once

#include <Actor/Actor.h>
#include <Sokoban/Actor/SDestroyEffect.h>

namespace Sokoban
{
	// 소코반 게임에서 박스를 대표하는 액터 클래스
	class Box : public Craft::Actor
	{
		TYPE_DECLARATIONS(Box, Actor)

	public:
		Box(const Craft::Vector2& position);

		// 박스 이동시 카운트 증가 함수
		void OnPushed();


	private:
		// 현재 count 값을 숫자 문자열로 바꿔주는 함수
		std::string GetBoxSymbol() const;

		int count = 1;

		void Explode();

	};

}
