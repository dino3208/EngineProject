#pragma once
#include "Level/Level.h"
#include "Actor/Map.h"

using namespace Craft;

class DungeonLevel : public Level
{
public:
	virtual void OnInitialized() override;

	Map map; // 객체 추가
};