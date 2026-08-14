#pragma once
#include <Actor/Actor.h>

namespace ShootingGame
{
	class CountdownTimer : public Craft::Actor
	{
	public:
		CountdownTimer();
		~CountdownTimer();

		virtual void Tick(float deltaTime) override;


	private:
		float remainingTime = 0.0f;
	};
}
