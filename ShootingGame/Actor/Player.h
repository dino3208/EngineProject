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
	// 캐릭터 좌표
	float playerX = 1.0f;
	float playerY = 1.0f;

	// 캐릭터 시야방향
	float playerAngle = 0.0f;

	// 비트 기반
	const float bpm = 120.0f;
	const float secondsPerBeat = 60.0f / bpm;
	float beatTimer = 0.0f;

	// 열쇠 보유 유무
	bool hasKey = false;
};

