#include "ScreenBuffer.h"
#include <cassert>

namespace Craft
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		: size(screenSize)
	{
		// 콘솔 버퍼 생성
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		// 값 확인
		assert(buffer != INVALID_HANDLE_VALUE);

		// 화면 창 크기 설정


		// 화면 버퍼 크기 설정


		// 직접 만든 콘솔의 커서 끄기

	}
	ScreenBuffer::~ScreenBuffer()
	{
		// 콘솔 닫기
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}
	void ScreenBuffer::Clear() const
	{
	}
	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{
	}
}