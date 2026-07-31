#pragma once

namespace Craft
{
	// 가상 공간에 배치될 모든 액터의 기본 클래스
	class Actor
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

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그
		bool isActive = true;

		// 삭제 요청 여부 플래그
		bool hasExpired = false;

	};

}