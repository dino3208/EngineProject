#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

namespace ShootingGame
{// 잡몹 클래스
	class EnemySpawner : public Craft::Actor
	{
		TYPE_DECLARATIONS(EnemySpawner, Actor)

	public:
		EnemySpawner();

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;

		// 적 생성 함수
		void SpawnEnemy();

	private:
		// 타이머
		Timer timer;
	};

	// 보스 클래스
	class BossSpawner : public Craft::Actor
	{
		TYPE_DECLARATIONS(BossSpawner, Actor)

	public:
		BossSpawner();

	private:
		virtual void Tick(float deltaTime) override;


		// 보스 생성 상태

		bool bossSpawned = false;

		// 보스 생성 함수
		void SpawnBoss();
	};
}