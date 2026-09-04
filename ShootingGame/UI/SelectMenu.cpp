#include "SelectMenu.h"

// 선택지 목록
void SelectMenu::SetOptions(const std::vector<std::string>& options)
{
    // this->를 써야 헤더의 private에 있는 options로 인식
    this->options = options;
}

// 목록 이동
void SelectMenu::MoveNext()
{
    // 나누기를 전체 선택지 목록으로 해야 함에 주의
    selectedIndex = (selectedIndex + 1) % options.size();
}

// 선택된 선택지 인덱스 반환
size_t SelectMenu::GetSelectedIndex() const
{
    return selectedIndex;
}

// 현재 선택 예상 선택지 표시
std::vector<std::string> SelectMenu::GetCurrentLines(const std::string& currentline) const
{
    std::vector<std::string> lines = {currentline};
    for (int i = 0; i < static_cast<int>(options.size()); ++i)
    {
        // 선택한 인덱스와 일치시 마커 출력
        std::string marker = (i == static_cast<int>(selectedIndex)) ? "> " : " ";
        // 문자열에 마커 + 선택지 뒤에 붙여서 출력
        lines.push_back(marker + options[i]); // 문자열의 size()는 세로줄의 갯수. 줄바꿈이 알아서 있다. 한 원소 = 한 줄
    }
    return lines;
}
