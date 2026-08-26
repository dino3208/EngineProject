#include <Engine/Engine.h>
#include "Level/DungeonLevel.h"

int main()
{
	Craft::Engine engine;
	engine.AddNewLevel<DungeonLevel>();
	engine.Run();
}