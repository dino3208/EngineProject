#include <iostream>
#include "EnemySpawner.h"
#include <Util/Util.h>
#include <Actor/Enemy.h>
#include <Level/Level.h>
#include <Level/GameLevel.h>

using namespace Craft;

EnemySpawner::EnemySpawner()
{
	// 적 생성 타이머 설정
	timer.SetTargetTime(Util::RandomRange(0.5f, 5.0f));
}


void EnemySpawner::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 타이머 초기화
	timer.Reset();

	// 적 생성
	SpawnEnemy();
}

void EnemySpawner::SpawnEnemy()
{
	// 적 생성 처리

	// 적 타입 갯수 확인 함수 호출
	int count = Enemy::GetEnemyTypeCount();

	// 랜덤 인덱스
	const int randomIndex = Util::RandomRange(0, count - 1);

	// 선택된 인덱스를 EnemyType으로 변환
	Enemy::EnemyType selectedType = static_cast<Enemy::EnemyType>(randomIndex);

	// 생성 y 위치 (랜덤)
	int yPosition = Util::RandomRange(1, 10);

	// 적 액터 생성
	std::shared_ptr<Level> owner = GetOwner();
	if (owner)
	{
		owner->SpawnActor<Enemy>(selectedType, yPosition);
	}
}



BossSpawner::BossSpawner()
{

}

void BossSpawner::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 게임레벨 포인터
	auto gameLevel = std::dynamic_pointer_cast<GameLevel>(GetOwner());

	// 보스 출현 조건
	if (gameLevel && !bossSpawned && gameLevel->GetScore() >= 1000)
	{
		// 보스 생성
		SpawnBoss();
		bossSpawned = true;
	}
}

void BossSpawner::SpawnBoss()
{
	// 보스 생성 처리


	// 보스 타입 지정
	Enemy::EnemyType selectedBoss = Enemy::EnemyType::Boss;

	// 생성 위치
	int xPosition = 60;
	int yPosition = 5;

	// 적 액터 생성
	std::shared_ptr<Level> owner = GetOwner();
	if (owner)
	{
		owner->SpawnActor<Enemy>(selectedBoss, xPosition, yPosition);
	}
}
