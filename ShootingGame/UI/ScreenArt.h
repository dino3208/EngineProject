#pragma once
#include <vector>
#include <string>

class ScreenArt
{
public:
	enum class ScreenType
	{
		MainScreen,
		Win,
		GameOver,
	};

	std::vector<std::string> GetArt(ScreenType type) const;
};