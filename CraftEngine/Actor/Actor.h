#pragma once

#include <Core/Core.h>
#include <memory> // std::weak_ptr 사용을 위해

namespace Craft
{
	// 전방선언
	class Level;

	// 가상 공간에 배치될 모든 액터의 기본 클래스
	class CRAFT_API Actor
	{
	public:
		Actor();
		virtual ~Actor();

		// 게임 플레이 이벤트 함수
		virtual void BeiginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 액터 제거 함수
		void Destroy();

		// 게임 (엔진) 종료 함수
		void QuitGame();

		// Getter(값을 읽는 함수)/Setter(값을 쓰는 함수)
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		inline std::shared_ptr<Level> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그
		bool isActive = true;

		// 삭제 요청 여부 플래그
		bool hasExpired = false;

		// 오너십 - 이 액터를 소유하는 레벨 객체
		// weak_ptr -> 약참조 (상호참조 방지를 위해)
		// 실제 사용 위해서는 해당 위치가 유효한지 확인 필요
		std::weak_ptr<Level> owner;

	};

}

class Actor
{
};
