#pragma once

#include <Engine/Engine.h>
#include <vector>

namespace ShootingGame
{// 레벨 관리에 사용할 상태 열거형
	enum class State
	{
		GamePlay = 0,
		Menu = 1,
		Sokoban = 2,
		Length
	};

	// 메뉴 레벨 및 게임 레벨을 관리하는 객체

	class Game : public Craft::Engine
	{
	public:
		Game();
		~Game() = default;

		// 메뉴/게임 레벨을 전환하는 함수
		void ToggleMenu();

		// 게임을 전환하는 함수
		void ChangeGame(State newState);

		// 게임 재시작 함수.
		void RestartGame();

		// 전역 접근 함수
		static Game& Get();

	private:
		// 메뉴 레벨과 게임 레벨을 관리할 배열
		std::vector<std::shared_ptr<Craft::Level>> levelList;

		// 현재 활성화된 레벨의 상태를 나타내는 변수
		State state = State::GamePlay;

		// 전역 접근이 가능하도록 변수 선언
		static Game* instance;
	};
}