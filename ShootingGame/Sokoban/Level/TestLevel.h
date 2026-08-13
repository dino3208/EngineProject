#pragma once

#include <Level/Level.h>
namespace Sokoban
{
	class TestLevel : public Craft::Level
	{
	public:
		virtual void OnInitialized() override;
	};
}