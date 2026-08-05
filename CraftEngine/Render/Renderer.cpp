#include "Renderer.h"
#include <cassert>
#include <iostream>
#include <Windows.h>


namespace Craft
{
	// -------------------- Frame ------------------------//

	Renderer::Frame::Frame(int bufferCount)
	{
		// 2차원 배열 생성
		charInfoArray = std::make_unique<CHAR_INFO[]>(bufferCount);
		sortingOrderArray = std::make_unique<int[]>(bufferCount);
	}
	Renderer::Frame::~Frame()
	{

	}

	// 프레임 초기화 함수
	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		// 이중 루프를 순회하면서 값 초기화
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// 1차원 배열을 2차원 배열로 사용할 때
				// 필요한 인덱스 좌표 변환
				const int index = (y * width) + x;

				// 글자 항목 초기화
				CHAR_INFO& info = charInfoArray[index];
				// 빈문자 설정 - 기존의 설정된 값 지우기
				info.Char.AsciiChar = ' ';
				// 색상 표기 안함
				info.Attributes = 0;
				
				// 그리기 순서 배열 항목 초기화
				sortingOrderArray[index] = -1;
			}
		}
	}

	// -------------------- Frame ------------------------//
	// static 변수 초기화
	Renderer* Renderer::instance = nullptr;
	Renderer::Renderer()
	{
		// 어서트
		assert(!instance && "instnace should be null");
		instance = this;

		// 콘솔 커서 안 보이게 설정
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		// 보이기 옵션을 false로 설정
		info.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}
	Renderer::~Renderer()
	{
		instance = nullptr;

		// 콘솔 커서 다시 보이게 설정 (복구)
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		// 보이기 옵션을 true로 설정
		info.bVisible = TRUE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}
	void Renderer::Submit(const std::string& image, const Vector2& position, Color color, int sortingOrder)
	{
		// 렌더 명령 생성 및 값 설정
		RenderCommand command; 
		command.image = image;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		// 렌더큐에 명령 추가
		renderQueue.emplace_back(command);
	}
	void Renderer::Draw()
	{
		// 화면(이미지/프레임) 지우기
		Clear();

		// 프레임 그리기
		DrawRenderQueue();

		// 화면(이밎/프레임) 표시
		Present();
	}
	Renderer& Renderer::Get()
	{
		// 어서트
		assert(instance && "instance should not be null");
		return *instance;
	}
	void Renderer::Clear()
	{
		// TODO; 시스템 clear 함수 사용
		system("cls");
	}
	void Renderer::DrawRenderQueue()
	{
		// 렌더큐를 순회하면서 그리기 명령 실행
		for (const RenderCommand& command : renderQueue)
		{
			// 윈도우 콘솔 핸들
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

			// 그릴 위치로 이동 (콘솔 좌표 이동)
			SetConsoleCursorPosition(handle, command.position);

			// 글자 색상 설정
			SetConsoleTextAttribute(handle, static_cast<WORD>(command.color));

			// TODO: 그리기
			std::cout << command.image;

			// 콘솔 색상 복원
			SetConsoleTextAttribute(handle, static_cast<WORD>(Color::White));
		}

		// 렌더큐 비우기
		renderQueue.clear();
	}
	void Renderer::Present()
	{
		// TODO: 이중 버퍼링 구현할 때 내용 채우기
	}
}