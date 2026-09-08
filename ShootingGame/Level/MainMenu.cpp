#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Input/Input.h>

#include "Level/MainMenu.h"
#include "Level/DungeonLevel.h"
#include "UI/ScreenArt.h"


using namespace Craft;

void MainMenu::OnInitialized()
{
	Level::OnInitialized();
	Engine::Get().PlayBackGroundMusic("MainMenu.wav");
}

void MainMenu::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	for (int key = 0; key < 256; ++key)
	{
		if (Input::Get().GetKeyDown(key)) // 아무 키나 누르면.
		{
		Engine::Get().AddNewLevel<DungeonLevel>(); // 게임레벨 생성.
		break;
		}
	}
}

void MainMenu::Draw()
{
	Level::Draw();

	ScreenArt screenArt;
	std::vector<std::string> art = screenArt.GetArt(ScreenArt::ScreenType::MainScreen);
	Renderer::Get().Submit(art, Vector2(155, 60), Color::White);
}
