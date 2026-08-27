#pragma once
#include <string>
#include <vector>

class Map
{
public:
	bool LoadFile(const std::string& filepath);

	std::vector<std::string> mapData; // 배열
};

