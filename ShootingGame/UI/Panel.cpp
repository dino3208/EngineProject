#include <Engine/Engine.h>
#include <Render/Renderer.h>

#include "Panel.h"


void Panel::BeginPlay()
{
	screenHeight = Craft::Engine::Get().GetHeight();
}

void Panel::Draw()
{
	for(int x=0;x<panelWidth;++x)
	{
		// 위/아래 테두리
		Renderer::Get().Submit("-", Vector2(startX + x, 0), Color::White);
		Renderer::Get().Submit("-", Vector2(startX + x, screenHeight - 1), Color::White);
	}
	// 좌/우 테두리
	for (int y = 0;y < screenHeight;++y)
	{
		Renderer::Get().Submit("|", Vector2(startX, y), Color::White);
		Renderer::Get().Submit("|", Vector2(startX + panelWidth - 1, y), Color::White);
	}

	// 네 모서리
	Renderer::Get().Submit("+", Vector2(startX, 0), Color::White);
	Renderer::Get().Submit("+", Vector2(startX + panelWidth - 1, 0), Color::White);
	Renderer::Get().Submit("+", Vector2(startX, screenHeight - 1), Color::White);
	Renderer::Get().Submit("+", Vector2(startX + panelWidth - 1, screenHeight - 1), Color::White);
}
