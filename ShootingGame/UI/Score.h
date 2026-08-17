#pragma once
#include <Actor/Actor.h>
#include <Level/GameLevel.h>

namespace ShootingGame
{
	class CountdownTimer : public Craft::Actor
	{
	public:
		CountdownTimer();
		~CountdownTimer();

		virtual void Tick(float deltaTime) override;


	private:
		int score = 0;
	};
}
