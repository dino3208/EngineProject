#include "Level.h"

namespace Craft
{
	Level::Level()
	{
	}
	Level::~Level()
	{
		// 초기화 되었다고 설정
		hasInitialized = true;
	}
	
	void Level::OnInitialized()
	{
	}
	void Level::BeiginPlay()
	{
	}
	
	void Level::Tick(float deltaTime)
	{
	}
	void Level::Draw()
	{
	}
}
