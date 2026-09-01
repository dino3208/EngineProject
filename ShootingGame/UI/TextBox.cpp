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
}
