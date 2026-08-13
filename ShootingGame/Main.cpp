#include <Engine/Engine.h>
#include <Game/Game.h>
#include <Level/GameLevel.h>
namespace ShootingGame
{
	int main()
	{
		// 엔진 객체 생성 및 실행
		//Craft::Engine engine;
		//engine.AddNewLevel<GameLevel>();
		Game game;
		game.Run();
	}
}