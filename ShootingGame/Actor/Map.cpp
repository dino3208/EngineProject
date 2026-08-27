#include "Actor/Map.h"

#include <fstream>

bool Map::LoadFile(const std::string& filePath) // 호출한 쪽의 문자열을 복사하지 않고, 그대로 씀.
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		return false; // 파일 못 찾을 경우
	}

	mapData.clear(); // LoadFile을 다시 호출해서 다른 맵 불러올 때 예전 맵 데이터 안 섞이기 위한 초기화

	std::string line;
	while (std::getline(file, line))
	{
		mapData.push_back(line);
	}

	return true;
}