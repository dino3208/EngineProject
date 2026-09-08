#pragma once
#include "Level/Level.h"

using namespace Craft;

class MainMenu : public Level
{
public:
	virtual void OnInitialized() override;

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
};

