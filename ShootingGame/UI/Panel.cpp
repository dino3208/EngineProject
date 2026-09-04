#include <Engine/Engine.h>
#include <Render/Renderer.h>

#include "Panel.h"

void Panel::Draw()
{
	for(int x=0;x<panelWidth;++x)
	{
		// 위/아래 테두리
		Renderer::Get().Submit("-", Vector2(startX + x, 0), Color::White);
		//Renderer::Get().Submit("-", Vector2(startX + x, panelHeight - 1), Color::White);
	}
	// 좌/우 테두리
	for (int y = 0;y < panelHeight;++y)
	{
		Renderer::Get().Submit("|", Vector2(startX, y), Color::White);
		Renderer::Get().Submit("|", Vector2(startX + panelWidth - 1, y), Color::White);
	}

	// 네 모서리
	Renderer::Get().Submit("+", Vector2(startX, 0), Color::White);
	Renderer::Get().Submit("+", Vector2(startX + panelWidth - 1, 0), Color::White);
	Renderer::Get().Submit("+", Vector2(startX, panelHeight - 1), Color::White);
	Renderer::Get().Submit("+", Vector2(startX + panelWidth - 1, panelHeight - 1), Color::White);
}
