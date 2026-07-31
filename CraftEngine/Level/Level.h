#pragma once

namespace Craft
{
	// 게임에 배치된 모든 액터를 관리하는 클래스
	class Level
	{
	public:
		Level();
		virtual ~Level();

		// 초기화 함수
		virtual void OnInitialized();

		// 게임 플레이 이벤트 함수
		virtual void BeiginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// Getter
		inline bool HasInitialized() const { return hasInitialized; }

	protected:
		// 초기화 처리 여부 플래그
		bool hasInitialized = false;
	};
}

