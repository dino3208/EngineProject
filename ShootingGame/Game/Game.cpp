#include <Level/MenuLevel.h>
#include <Game/Game.h>
#include <Level/GameLevel.h>

#include <Input/Input.h>
#include <Sokoban/Level/SGameLevel.h>
#include <cassert>

namespace ShootingGame
{
	class MenuLevel;
	Game::Game()
	{
		instance = this;

		// 두 레벨 생성 및 배열에 추가
		levelList.emplace_back(std::make_shared<GameLevel>());
		levelList.emplace_back(std::make_shared<MenuLevel>());
		
		// TODO 소코반 넘어갈 때 
		levelList.emplace_back(std::make_shared<Sokoban::GameLevel>());

		// 시작 상태 설정
		state = State::GamePlay;

		// 게임 시작시 활성화할 레벨 설정
		mainLevel = levelList[(int)state];

	}

	void Game::ToggleMenu()
	{
		int stateIndex = static_cast<int>(state);

		// 인덱스를 1->0, 0->1로 토글하는 공식
		int nextState = 1 - stateIndex;

		// 레벨 설정 및 상태값 업데이트
		mainLevel = levelList[nextState];
		state = static_cast<State>(nextState);
	}

	// TODO 소코반 넘어갈 때
	void Game::ChangeGame(State newState)
	{
		mainLevel = levelList[(int)newState];
		state = newState;
	}



	void Game::RestartGame()
	{
		std::shared_ptr<GameLevel> gameLevel = std::dynamic_pointer_cast<GameLevel>(levelList[0]);
		if (!gameLevel)
		{
			return;
		}

		gameLevel->Restart();
		mainLevel = gameLevel;
		state = State::GamePlay;
	}

	// TODO 소코반 넘어갈 때
	Game* Game::instance = nullptr;

	Game& Game::Get()
	{
		// 검증 - assert
		// 무조건 통화해야하는 조건이 있을 때 사용
		// 디버그 모드에서만 동작
		assert(instance && "instance is null");


		return *instance;
	}
}