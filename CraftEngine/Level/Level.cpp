#include "Level.h"

namespace Craft
{
	Level::Level()
	{
	}
	Level::~Level()
	{
		
	}
	
	void Level::OnInitialized()
	{
		// 초기화 되었다고 설정
		hasInitialized = true; // 이걸 추가하지 않아서 SpawnActor에 계속 추가되어서 프레임이 저하되었음.
	}

	void Level::BeginPlay()
	{
		// 액터 초기화 시 1번 호출되는 이벤트
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			// 검증 - 이미 BeginPlay 처리된 경우 건너뛰기.
			if (actor->HasBeganPlay())
			{
				continue;
			}

			// BeginPlay 이벤트 호출
			actor->BeginPlay();
		}
	}
	
	void Level::Tick(float deltaTime)
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			// 검증 - 활성화되지 않았으면 건너뛰기
			if (!actor->IsActive())
			{
				continue;
			}

			// Tick 이벤트 호출
			actor->Tick(deltaTime);
		}
	}
	void Level::Draw()
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			// 검증 - 활성화되지 않았으면 건너뛰기
			if (!actor->IsActive())
			{
				continue;
			}

			// Tick 이벤트 호출
			actor->Draw();
		}
	}
	void Level::ProcessAddAndDestroyActors()
	{
		// 액터 제거 처리
		// 이터레이터 기반 루프
		for (auto iterator = actorList.begin(); iterator != actorList.end();)
		{
			// 제거 요청된 액터인지 확인
			auto actor = *iterator;
			if (actor->HasExpired())
			{
				iterator = actorList.erase(iterator);
				continue;
			}
			// 다음 순번을 처리하기 위해 이터레이터(반복자, 포인터) 증가 처리
			// 포인터의 덧셈 연산은 메모리점프
			++iterator;
		}

		// 추가 처리
		// 추가 요청된 목록이 없으면 종료
		if (addRequestedActorList.empty())
		{
			return;
		}
		for (const auto& actor : addRequestedActorList)
		{
			actorList.emplace_back(actor);
		}

		// 추가 처리된 목록 정리. 바로 위에서 넣어주었으니까.
		addRequestedActorList.clear();

	}
	void Level::SavePreviousActorStates()
	{
		// 액터 순회하면서 이전 상태 저장 처리
		for (const auto actor : actorList)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			// 상태 저장
			actor->SavePreviousState();
		}
	}
}
