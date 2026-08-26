#pragma once
#include "Actor/Actor.h"


using namespace Craft;
class Player:public Actor
{
public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

protected:
	float playerX = 1.0f;
	float playerY = 1.0f;
	float playerAngle = 0.0f;

	bool hasKey = false;
};

