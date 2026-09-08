#include <Engine/Engine.h>
#include "Level/DungeonLevel.h"
#include "Level/MainMenu.h"

int main()
{
	Craft::Engine engine;
	engine.AddNewLevel<MainMenu>();
	engine.Run();
}