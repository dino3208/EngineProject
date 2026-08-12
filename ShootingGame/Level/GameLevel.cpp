#include "GameLevel.h"
#include <Actor/Player.h>
#include <Actor/EnemySpawner.h>

void GameLevel::AddScore(int scoreToAdd)
{
	score += scoreToAdd;
}

int GameLevel::GetScore() const
{
	return score;;
}

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	// 플레이어 액터 추가
	SpawnActor<Player>();

	// 적 생성기 액터 추가
	SpawnActor<EnemySpawner>();

	// 보스 생성기 액터 추가
	SpawnActor<BossSpawner>();
	
	score = 0;
}
