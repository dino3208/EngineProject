#pragma once
#include "Actor/Actor.h"

using namespace Craft;

class Panel : public Actor
{
public:
	virtual void Draw() override;

	Vector2 GetPosition(int localX, int localY) const
	{
		return Vector2(startX + localX, localY);
	}

	int startX = 0;
	int panelWidth = 0;
	int panelHeight = 0;

protected:

};

