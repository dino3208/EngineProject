#include "GameLevel.h"

#include <Actor/Ground.h>
#include <Actor/Wall.h>

#include <Actor/Player.h>
#include <Actor/Box.h>
#include <Actor/Target.h>

#include <iostream>
#include <cassert>

using namespace Craft;
void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	// 파일을 읽어서 맵 로드
	LoadMap("Map.txt");
}

void GameLevel::Draw()
{
	Level::Draw();
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

	// Todo: 읽은 데이터를 기반으로 로직 제작
	// 1. 화면에 액터를 그리기
	
	// 문자열에 저장된 값을 접근할 때 사용할 인덱스
	int index = 0;

	// 액터 생성에 사용할 위치 값
	Vector2 position;
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
			position.x = 0;
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
