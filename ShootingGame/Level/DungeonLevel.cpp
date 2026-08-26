#include "DungeonLevel.h"
#include "Actor/Player.h"

using namespace Craft;

void DungeonLevel::OnInitialized()
{
	Level::OnInitialized();
	SpawnActor<Player>();
}
