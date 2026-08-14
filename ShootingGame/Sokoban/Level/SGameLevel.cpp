#include "SGameLevel.h"

#include <Sokoban/Actor/Ground.h>
#include <Sokoban/Actor/Wall.h>

#include <Sokoban/Actor/SPlayer.h>
#include <Sokoban/Actor/Box.h>
#include <Sokoban/Actor/Target.h>

#include <Render/Renderer.h>


#include <Game/Game.h>
#include <iostream>
#include <cassert>

namespace Sokoban
{
	using namespace Craft;
	bool GameLevel::CanMove(
		const Vector2& playerPosition,
		const Vector2& nextPosition)
	{
		// 게임 클리어인 경우 처리 안함.
		if (isGameClear)
		{
			return false;
		}

		// 이동하려는 위치에 어떤 액터가 있는 지를 확인할 때 타입을 활용.
		// 박스가 있을 때 로직이 더 복잡하기 때문에 이 처리를 위한 배열.
		std::vector<std::shared_ptr<Actor>> boxList;

		// 레벨을 순회하면서 박스 타입을 boxList에 저장
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 현재 액터가 박스 타입인지 확인 (커스텀 RTTI 활용)
			if (actor->IsTypeOf<Box>())
			{
				boxList.emplace_back(actor);
				continue;
			}
		}

		// 이동하려는 위치에 박스가 있는지 검증을 위한 변수
		std::shared_ptr<Actor> boxActor = nullptr;

		// 위치 값 비교를 통해 해당 위치에 박스가 있는지 확인
		for (const std::shared_ptr<Actor>& box : boxList)
		{
			if (box->GetPosition() == nextPosition)
			{
				boxActor = box;
				break;
			}
		}

		// #1. 이동하려는 위치에 박스가 있는 경우
		if (boxActor)
		{
			// 박스는 밀 수 있지만, 밀리는 위치가 이동가능해야 함
			// 1. 두 위치 값이 있을 때 이동 방향 구하기(두 벡터 빼기)
			// 플레이어 위치에서 다음 위치로 향하는 방향 구하기
			// (x, y, 1) - (x1, y1, 1) = (x-x1, y-y1, 0)
			Vector2 direction = nextPosition - playerPosition;

			// 박스가 밀리는 위치 구하기
			// 위치(좌표) + 벡터(크기, 방향을 가지는 데이터)
			// 벡터의 정의에 따르면 위치 정보는 없음
			// 하지만 이 덧셈은 수학적으로 정의되지 않음
			// 원래는 안되는데 이게 가능하도록 별도로 정의함
			// 동차좌표계(위치, 벡터) | 아핀 변환
			// (x, y, w): w가 0이면 벡터 | w가 1이면 위치
			// (x, y, z, w): w가 0이면 벡터 | w가 1이면 위치
			// (x1, y1, 1) + (x2, y2, 0) = (x1+x2, y1+y2, 1)
			// (x1, y1, 1) + (x2, y2, 1) = (x1+x2, y1+y2, 2)? 정의가 되어 있지 않으므로 위치와 위치를 더하면 안됨.
			Vector2 newPosition = boxActor->GetPosition() + direction;

			// 박스가 밀리는 위치에 다른 박스가 있는지 확인
			for (const std::shared_ptr<Actor>& otherBox : boxList)
			{
				// 같은 액터를 검사 중이면 건너뛰기
				if (otherBox == boxActor)
				{
					continue;
				}

				// 위치 확인
				if (otherBox->GetPosition() == newPosition)
				{
					// 박스를 이동시킬 위치에 다른 박스가 있으면 이동 불가
					return false;
				}
			}

			// 박스가 밀릴 위치가 이동 가능한지 다시 확인
			for (const std::shared_ptr<Actor>& actor : actorList)
			{
				// 박스가 밀리는 위치의 액터 검색
				if (actor->GetPosition() == newPosition)
				{
					// 벽이면 이동 불가
					if (actor->IsTypeOf<Wall>())
					{
						return false;
					}

					// 땅이거나 목표 지점이면 이동 가능
					if (actor->IsTypeOf<Ground>()
						|| actor->IsTypeOf<Target>())
					{
						// 박스 밀림 처리
						boxActor->SetPosition(newPosition);

						// RTTI로 타입 변환 후 숫자를 올리는 OnPushed 함수 호출
						if (auto box = std::dynamic_pointer_cast<Box>(boxActor))
						{
							box->OnPushed();
						}

						// 점수 확인
						isGameClear = CheckGameClear();

						return true;
					}
				}
			}
		}

		// #2. 플레이어가 이동하려는 곳에 박스가 없는 경우

		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 플레이어가 이동하려는 위치의 액터 검색
			if (actor->GetPosition() == nextPosition)
			{
				// 벽이면 이동 불가
				if (actor->IsTypeOf<Wall>())
				{
					return false;
				}

				// 벽이 아니라면 이동 가능
				// 1. 앞에서 박스는 아니라고 검증함
				// 2. 바로 위에서 벽도 아니라고 검증함
				// 3. 박스도 벽도 아니기 때문에 플레이어/타겟/땅만 남음
				// 4. 플레이어는 검증 필요하지 않음
				return true;
			}
		}

		// 예상치 못한 처리 - 이동 불가
		return false;
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

	void GameLevel::OnInitialized()
	{
		Level::OnInitialized();

		// 게임 클리어 초기화.
		isGameClear = false;

		// 점수 초기화.
		targetScore = 0;

		// 파일을 읽어서 맵 로드
		LoadMap("Test.txt");
	}

	void GameLevel::Draw()
	{
		Level::Draw();

		// 게임을 클리어한 경우 메시지 표시
		if (isGameClear)
		{
			// 렌더러를 사용해서 게임 클리어 표시
			Renderer::Get().Submit(
				"Clear!",
				Vector2(25, 0)
			);
			// TODO 다시 슈팅게임으로 가는 경로
			ShootingGame::Game::Get().ChangeGame(ShootingGame::State::GamePlay);
		}
	}
	void GameLevel::LoadMap(const std::string& filename)
	{
		// 최종 경로 조립
		std::string path = std::string("../Assets/") + filename;

		// 파일 열기 (C-style)
		FILE* file = nullptr;
		fopen_s(&file, path.c_str(), "rt");
		if (!file)
		{
			assert(false && "failed to a sokoban open stage file.");
			return;
		}

		// 파일의 내용을 저장할 버퍼(데이터 저장공간) 확인
		// 파일 길이 확인 -> 파일 위치를 제일 뒤로 이동 시킨 다음, 해당 위치값 읽기
		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		// 파일 제일 끝위치를 구한 다음에는 다시 처음으로 되돌리기
		// fseek(file, 0, SEEK_SET); 동일
		rewind(file);

		// 앞에서 구한 위치를 사용해서 버퍼 생성
		char* buffer = new char[fileSize] {};

		// 데이터 읽기(파일 읽기)
		size_t readSize = fread(buffer, sizeof(char), fileSize, file);

		// 어서트
		assert(readSize > 0 && "No data in the stage file.");

		// 1. 화면에 액터를 그리기

		// 문자열에 저장된 값을 접근할 때 사용할 인덱스
		int index = 0;

		// 액터 생성에 사용할 위치 값
		
		Vector2 startPosition = Vector2(30, 10); // 시작 위치
		Vector2 position = startPosition; // 위치 값 조정
		while (true)
		{
			// 종료 조건 - 내용을 모두 읽었는지 파악
			if (index >= fileSize)
			{
				break;
			}

			// 이번에 확인할 문자 값
			char mapCharacter = buffer[index];

			// 인덱스 증가 처리
			++index;


			// 현재 문자가 개행문자라면 로직은 건너뛰고,
			// 위치 값만 설정
			if (mapCharacter == '\n')
			{
				++position.y;
				position.x = startPosition.x;
				continue;
			}

			// 읽은 문자 별로 처리
			switch (mapCharacter)
			{
				// 벽
			case '#':
				// 벽 액터 생성
				SpawnActor<Wall>(position);
				break;

				// 바닥(땅)
			case '.':
				// 바닥 액터 생성
				SpawnActor<Ground>(position);
				break;

				// 플레이어
			case 'p':
				// 플레이어는 이동이 가능하기 때문에 같은 위치에 땅 생성
				SpawnActor<Ground>(position);

				// 플레이어 액터 생성
				SpawnActor<Player>(position);
				break;

				// 박스
			case 'b':
				// 박스는 이동이 가능하기 때문에 같은 위치에 땅 생성
				SpawnActor<Ground>(position);

				// 박스 액터 생성
				SpawnActor<Box>(position);
				break;

				// 타겟 (목표 위치)
			case 't':
				SpawnActor<Target>(position);

				// 목표 스코어 증가 처리
				++targetScore;
				break;
			}

			// x 위치 업데이트
			++position.x;
		}

		// 모두 사용한 버퍼 해제
		delete[] buffer;
		buffer = nullptr;

		// 파일 닫기
		fclose(file);
		file = nullptr;

	}

	bool GameLevel::CheckGameClear()
	{
		// 점수 확인용 변수
		int currentScore = 0;

		// 하고 싶은 일: 박스가 타겟 위치에 모두 배치됐는지 확인

		// 박스 목록/타겟 목록 저장
		std::vector<std::shared_ptr<Actor>> boxList;
		std::vector<std::shared_ptr<Actor>> targetList;

		// 게임 레벨의 모든 액터를 순회하면서 박스와 타겟 목록에 저장
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 박스인 경우 박스 목록에 추가
			if (actor->IsTypeOf<Box>())
			{
				boxList.emplace_back(actor);
				continue;
			}

			// 타겟인 경우 타겟 목록에 추가
			if (actor->IsTypeOf<Target>())
			{
				targetList.emplace_back(actor);
			}
		}

		// 목표 지점에 배치된 박스 수 확인
		for (const std::shared_ptr<Actor>& box : boxList)
		{
			for (const std::shared_ptr<Actor>& target : targetList)
			{
				// 위치 비교
				if (box->GetPosition() == target->GetPosition())
				{
					currentScore += 1;
				}
			}
		}

		// 목표 지점에 배치된 박스의 수가 타겟 수 (목표점수)와 같은 지 확인
		return currentScore == targetScore;
	}
}