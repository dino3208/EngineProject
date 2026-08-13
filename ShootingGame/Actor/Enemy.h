#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

namespace ShootingGame
{
	class Enemy : public Craft::Actor
	{
		// 커스텀 타입 설정
		TYPE_DECLARATIONS(Enemy, Actor)

			// 이동 방향 열거형
			enum class MoveDirection
		{
			None = -1,
			Left,
			Right,
			Down
		};
	public:

		// 적 타입 종류
		enum class EnemyType
		{
			Type1,
			Type2,
			Type3,
			Type4,
			Type5,
			Boss,
		};

		// 적 생성자
		Enemy(EnemyType enemyType, int yPosition);

		// 보스 생성자
		Enemy(EnemyType enemyType, int xPosition, int yPosition);

		// 적 타입 갯수 반환 함수
		static int GetEnemyTypeCount();

		// 데미지 입는 함수
		void TakeDamage(int damage);


	private:
		// 이벤트 함수 오버라이드
		virtual void Tick(float deltaTime) override;

		// 충돌 처리 함수 오버라이드
		virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	private:
		// 이동 방향 열거형 변수
		MoveDirection direction = MoveDirection::None;

		// 좌우 이동 처리를 위한 변수
		float xPosition = 0.0f;
		float yPosition = 0.0f;
		float moveSpeed = 5.0f;

		// 이동 함수
		void Move(float deltaTime);

		// 발사 타이머
		Timer timer;

		// 적 처치 점수
		int scoreValue = 0;

		// 적 HP
		int hp = 0;
	};
}
