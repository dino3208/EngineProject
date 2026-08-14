#include "CountdownTimer.h"
#include <Render/Renderer.h>
#include <Engine/Engine.h>


namespace ShootingGame
{
	using namespace Craft;

	ShootingGame::CountdownTimer::CountdownTimer()
	{
		
	}

	ShootingGame::CountdownTimer::~CountdownTimer()
	{
	}

	void ShootingGame::CountdownTimer::Tick(float deltaTime)
	{
		// 생성 위치 설정
		int x = 0;
		int y = Engine::Get().GetHeight()-1;
		remainingTime = 3.0f;
		std::string str = std::to_string(remainingTime);

		SetPosition(Vector2(x, y));
		Renderer::Get().Submit(
			"Score: ",
			Vector2(x, y)
		);
		Renderer::Get().Submit(
			str,
			Vector2(x + 10, y)
		);
	}
}
