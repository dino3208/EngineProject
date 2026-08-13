#include <Level/GameLevel.h>
#include <Actor/Player.h>
#include <Actor/EnemySpawner.h>

namespace ShootingGame
{
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

	void GameLevel::Restart()
	{
		// 기존 액터 제거.
		if (!actorList.empty())
		{
			for (const auto& actor : actorList)
			{
				actor->Destroy();
			}
		}

		// 액터 목록 초기화.
		actorList.clear();

		// 맵 처음부터 다시 로드.
		OnInitialized();
	}
}