#pragma once
#include "Actor/Actor.h"


using namespace Craft;
class Player:public Actor
{
public:
	virtual void Tick(float deltaTime) override;

protected:
	float playerX = 0.0f;
	float playerY = 0.0f;
	float playerAngle = 0.0f;
};

