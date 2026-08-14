#include "Enemy.h"
#include <Util/Util.h>
#include <Engine/Engine.h>
#include <Actor/EnemyBullet.h>
#include <Level/Level.h>
#include <Level/GameLevel.h>
#include <Actor/PlayerBullet.h>
#include <Actor/DestroyEffect.h>
#include <cassert>
#include <Game/Game.h>
namespace ShootingGame

{
	using namespace Craft;

	// 생성할 적 이미지 타입 배열
	static std::string enemyImages[] =
	{
		";:^:;",
		"zZwZz",
		"oO@Oo",
		"<-=->",
		")qOp(",
	};

	// 보스 이미지
	static std::vector<std::string> bossShape =
	{
			"<-=->)qOp()qOp()qOp()qOp()qOp()qOp()qOp(<-=->",
			";:^:;)qOp()qOp()qOp()qOp()qOp()qOp()qOp(;:^:;",
			"<-=->)qOp()qOp()qOp()qOp()qOp()qOp()qOp(<-=->",
			";:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;",
			";:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;;:^:;",
			"oO@Oo     oO@Oo     oO@Oo     oO@Oo     oO@Oo",
			"oO@Oo     oO@Oo     oO@Oo     oO@Oo     oO@Oo",
			"oO@Oo     oO@Oo     oO@Oo     oO@Oo     oO@Oo",
			"oO@Oo;:^:;oO@Oo;:^:;oO@Oo;:^:;oO@Oo;:^:;oO@Oo",
	};

	// 일반 적 생성자
	Enemy::Enemy(EnemyType enemyType, int yPosition)
		: Actor(enemyImages[static_cast<int>(enemyType)])
	{
		// 랜덤 (오른쪽 또는 왼쪽으로 이동할 지 결정)
		int random = Util::RandomRange(1, 10);

		// 랜덤으로 선택된 수가 짝수/홀수 여부에 따라 방향 결정
		if (random % 2 == 0)
		{
			// 화면 오른쪽에 생성(이동은 왼쪽 방향)
			direction = MoveDirection::Left;
			xPosition = static_cast<float>(Engine::Get().GetWidth() - width - 1);
		}
		else
		{
			// 화면 왼쪽에 생성 (이동은 오른쪽 방향)
			direction = MoveDirection::Right;
			xPosition = 0.0f;
		}

		// 위치 설정
		SetPosition(Vector2(static_cast<int>(xPosition), yPosition));

		// 발사 타이머 시간 설정 (1초에서 3초 사이의 시간을 랜덤으로)
		timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));

		// TODO 나중에 점수 분화
		scoreValue = (static_cast<int>(enemyType) + 1) * 500;

		hp = 1;
	}

	// 보스 생성자
	Enemy::Enemy(EnemyType enemyType, int xPosition, int yPosition)
		: Actor(bossShape)
	{
		// 랜덤 (오른쪽 또는 왼쪽으로 이동할 지 결정)
		int random = Util::RandomRange(1, 10);

		// 랜덤으로 선택된 수가 짝수/홀수 여부에 따라 방향 결정
		if (random % 2 == 0)
		{
			// 화면 오른쪽에 생성(이동은 왼쪽 방향)
			direction = MoveDirection::Left;
		}
		else
		{
			// 화면 왼쪽에 생성 (이동은 오른쪽 방향)
			direction = MoveDirection::Right;
		}

		// 위치 설정
		SetPosition(Vector2(static_cast<int>(xPosition), yPosition));

		// 발사 타이머 시간 설정 (1초에서 3초 사이의 시간을 랜덤으로)
		timer.SetTargetTime(Util::RandomRange(0.5f, 1.0f));

		scoreValue = 1000;

		// HP 설정
		hp = 100;

	}

	// 적 타입 갯수 확인 함수
	int Enemy::GetEnemyTypeCount()
	{
		return sizeof(enemyImages) / sizeof(enemyImages[0]);
	}

	void Enemy::TakeDamage(int damage)
 	{
		hp -= damage;
	}


	void Enemy::Move(float deltaTime)
	{
		// 이동
		float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;
		xPosition += dir * moveSpeed * deltaTime;

		// 좌표 검사
		// 왼쪽으로 벗어나는 경우 오른쪽 방향전환 실행
		if (xPosition <= 0)
		{
			direction = MoveDirection::Right;
		}

		// 오른쪽으로 벗어나는 경우 왼쪽 방향 전환
		if (xPosition + width >= Engine::Get().GetWidth() - 1)
		{
			direction = MoveDirection::Left;
		}

		// 위치 설정
		SetPosition(Vector2(static_cast<int>(xPosition), GetPosition().y));
	}


	void Enemy::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		// 이동 함수 호출
		Move(deltaTime);

		// 발사

		// 타이머 시간 업데이트
		timer.Tick(deltaTime);
		if (!timer.IsTimeOut())
		{
			return;
		}

		// 발사 가능하다면 타이머 리셋
		timer.Reset();

		// 탄약 생성
		// 생성 위치 설정
		if (hp == 1)
		{
			Vector2 bulletPosition(GetPosition().x + (width / 2), GetPosition().y);

			// 액터 생성
			std::shared_ptr<Level> owner = GetOwner();
			if (owner)
			{
				owner->SpawnActor<EnemyBullet>(
					bulletPosition, Util::RandomRange(10.0f, 20.0f)
				);
			}
		}
		else
		{
			int randomX =
				Util::RandomRange(GetPosition().x, GetPosition().x + width);
			int randomY =
				Util::RandomRange(GetPosition().y, GetPosition().y + height);
			Vector2 bulletPosition(randomX, randomY);

			std::shared_ptr<Level> owner = GetOwner();
			if (owner)
			{
				owner->SpawnActor<EnemyBullet>(
					bulletPosition, Util::RandomRange(1.0f, 50.f)
				);
			}
		}
	}

	void Enemy::OnCollision(const std::shared_ptr<Actor>& other)
	{
		super::OnCollision(other);

		// 충돌한 다른 액터가 플레이어 탄약이면 삭제
		// 커스텀 타입 활용
		if (other->IsTypeOf<PlayerBullet>())
		{
			// 플레이어 탄약 제거
			other->Destroy();

			// 적 HP 감소
			TakeDamage(1);

			//TODO: 소코반 넘어갈 때
			if (hp == 25)
			{
				Game::Get().ChangeGame(State::Sokoban);
			}

			// 적 HP = 0 확인
			if (hp == 0)
			{
				// 적 액터 제거
				Destroy();

				// 적 파괴 이펙트 생성
				if (GetOwner())
				{
					// 적 처치시 점수 추가
					auto gameLevel = std::dynamic_pointer_cast<GameLevel>(GetOwner());

					assert(gameLevel != nullptr && "Owner must be a GameLevel!");

					if (gameLevel)
					{
						gameLevel->AddScore(scoreValue);
					}

					if (height > 1)
					{
						for (int i = 0; i < 20; ++i)
						{
							int randomX =
								Util::RandomRange(GetPosition().x, GetPosition().x + width);
							int randomY =
								Util::RandomRange(GetPosition().y, GetPosition().y + height);
							Vector2 randomPosition(randomX, randomY);
							GetOwner()->SpawnActor<DestroyEffect>(randomPosition);
						}
					}
					else
					{
						GetOwner()->SpawnActor<DestroyEffect>(GetPosition());
					}

				}
			}

		}

	}
}