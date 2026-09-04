#pragma once
#include <vector>
#include <string>

// 선택지 메뉴를 위한 클래스
class SelectMenu
{
public:
	void SetOptions(const std::vector<std::vector<std::string>>& options);
	void MoveNext();
	size_t GetSelectedIndex() const;
	std::vector<std::string> GetCurrentLines(const std::vector<std::string>& currentline) const;

private:
	// 선택지
	std::vector<std::vector<std::string>> options;
	// 선택된 선택지
	size_t selectedIndex = 0;
};

