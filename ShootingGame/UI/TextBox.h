#pragma once
#include "Actor/Actor.h"

using namespace Craft;

class TextBox : public Actor
{
public:
	virtual void Draw() override;

	int startY = 0;
	int boxWidth = 0;
	int boxHeight = 0;

private:
};

