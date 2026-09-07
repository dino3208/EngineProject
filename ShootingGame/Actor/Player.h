#pragma once
#include <Util/Util.h>
#include "Actor/Actor.h"
#include "Actor/Map.h"
#include "UI/TextBox.h"
#include "UI/SelectMenu.h"

using namespace Craft;
using namespace Util;
class Player:public Actor
{
public:
	Player();
	~Player();

	void SetSpawnPosition(float x, float y);

	int viewWidth = 0;
	int viewHeight = 0;

	void TryPickUpItem();
	void TryExit();

	// 충돌 함수.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// Player 클래스는 이제 Map타입의 변수를 갖는 선언
	// 이 객체를 만들면 자동으로 Map의 기본상태(빈 mapData)로 시작
	Map* map = nullptr;

	// Player가 TextBox에 메세지 출력을 요청할 수 있도록 주소 저장
	TextBox* textBox = nullptr;

protected:
	// 캐릭터 좌표
	float playerX = 9.0f;
	float playerY = 1.0f;

	// 캐릭터 시야방향
	float playerAngle = Util::DegToRad(180.0f);

	// 충돌 관련 변수.
	int hitCount = 0; // 몇 번 충돌.
	int totalDamage = 0; // 누적 데미지.
	float invulnerableTimer = 0.0f; // 연속 히트 방지 쿨다운.

	// 비트 기반
	const float bpm = 120.0f;
	const float secondsPerBeat = 60.0f / bpm;
	float beatTimer = 0.0f;

	// 열쇠 보유 유무
	bool hasKey = false;
	bool hasWon = false;
	float winScreenTimer = 2.0f;
};

