#pragma once
#include "Actor/Actor.h"
#include "Actor/Map.h"


using namespace Craft;
class Player:public Actor
{
public:
	int viewWidth = 0;
	int viewHeight = 0;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// Player 클래스는 이제 Map타입의 변수를 갖는 선언
	// 이 객체를 만들면 자동으로 Map의 기본상태(빈 mapData)로 시작
	Map* map = nullptr;

protected:
	// 캐릭터 좌표
	float playerX = 9.0f;
	float playerY = 1.0f;

	// 각도->라디안 변환 함수
	const float PI = 3.14159265f;
	float DegToRad(float degree)
	{
		return degree * (PI / 180.0f);
	}

	// 캐릭터 시야방향
	float playerAngle = DegToRad(180.0f);

	// 비트 기반
	const float bpm = 120.0f;
	const float secondsPerBeat = 60.0f / bpm;
	float beatTimer = 0.0f;

	// 열쇠 보유 유무
	bool hasKey = false;
};

