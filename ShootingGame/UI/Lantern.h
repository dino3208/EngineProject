#pragma once

#include "Actor/Actor.h"

using namespace Craft;


class Lantern : public Actor
{
public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	

protected:
	int screenHeight = 0;

	int lanternFrame = 0;
	float lanternFrameTimer = 0.0f;
	const float lanternFrameDuration = 0.15f;
};

