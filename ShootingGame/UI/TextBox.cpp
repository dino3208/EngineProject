#include "TextBox.h"
#include <Render/Renderer.h>

void TextBox::Draw()
{
	for (int x = 0;x < boxWidth;++x)
	{
		// 위/아래 테두리
		Renderer::Get().Submit("-", Vector2(x, startY), Color::White);
		Renderer::Get().Submit("-", Vector2(x, startY + boxHeight - 1), Color::White);
	}
	// 좌/우 테두리
	for (int y = startY;y < startY + boxHeight;++y)
	{
		Renderer::Get().Submit("|", Vector2(0, y), Color::White);
		Renderer::Get().Submit("|", Vector2(boxWidth - 1, y), Color::White);
	}

	// 네 모서리
	Renderer::Get().Submit("+", Vector2(0, startY), Color::White);
	Renderer::Get().Submit("+", Vector2(boxWidth - 1, startY), Color::White);
	Renderer::Get().Submit("+", Vector2(0, startY + boxHeight - 1), Color::White);
	Renderer::Get().Submit("+", Vector2(boxWidth - 1, startY + boxHeight - 1), Color::White);

	// 메세지 출력
	Renderer::Get().Submit(currentMessage, Vector2(2, startY + 1), Color::White);
}

// 어떤 메세지를 보여줄 지 정하는 함수
void TextBox::ShowMessage(MessageType type, int value)
{
	switch (type)
	{
	case MessageType::KeyPickUp:
		currentMessage = "열쇠를 획득했습니다.";
		return; // void 함수이기 때문에 break;를 써도 동일한 효과
	case MessageType::DoorLocked:
		currentMessage = "문이 잠겨있습니다.";
		return;
	case MessageType::Damaged:
		currentMessage = "피해를 " + std::to_string(value) + "을(를) 입었습니다.";
		return;
	}
}




