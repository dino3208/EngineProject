#include "Player.h"
#include <Engine/Engine.h>
#include <Input/Input.h>

using namespace Craft;
Player::Player()
	: Actor("<=A=>", Vector2::Zero, Color::Green)
{
	// 생성 위치 설정
	int x = (Engine::Get().GetWidth() / 2) - (width / 2);
	int y = (Engine::Get().GetHeight() - 2);
	SetPosition(Vector2(x, y));

	// x 위치 저장
	xPosition = static_cast<float>(x);
}


void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// ESC 키 종료 처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	// 방향키 입력에 따른 이동 방향 설정
	// 오른쪽 방향: 1 | 왼쪽 방향: -1
	float direction = 0.0f;
	if (Input::Get().GetKey(VK_RIGHT))
	{
		direction = 1.0f;
	}
	if (Input::Get().GetKey(VK_LEFT))
	{
		direction = -1.0f;
	}

	// 이동 함수 호출
	Move(direction, deltaTime);
}

void Player::Move(float direction, float deltaTime)
{
	// x위치 업데이트
	// 이동 처리 -> 이동 방향과 빠르기를 적용해서 새로운 위치를 구하는 것
	// 이동 방향(directino) / 빠르기(moveSpeed) | 시간
	// 등속도 운동: 이동 거리 = 기존의 위치 + 이동방향 X 빠르기 X 시간
	xPosition += direction * moveSpeed * deltaTime;

	// 화면 왼쪽 벗어나지 않도록 처리
	if (xPosition < 0)
	{
		xPosition = 0.0f;
	}

	// 화면 오른쪽 벗어나지 않도록 처리
	if (xPosition + width >= Engine::Get().GetWidth())
	{
		xPosition = static_cast<float>(Engine::Get().GetWidth() - width);
	}

	// 위치 업데이트
	Vector2 newPosition = GetPosition();

	// float 값을 int로 형변환할 때 소숫점 값은 버림 처리된다는 점 주의
	newPosition.x = static_cast<int>(xPosition);
	SetPosition(newPosition);
}
