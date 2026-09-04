#include "SelectMenu.h"

// 선택지 목록
void SelectMenu::SetOptions(const std::vector<std::vector<std::string>>& options)
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
std::vector<std::string> SelectMenu::GetCurrentLines(const std::vector<std::string>& currentline) const
{
    static const std::vector<std::string> cursorArt =
    {
"        #    ",
"         #   ",
"          #  ",
"           # ",
"          #  ",
"         #   ",
"        #    "
    };

    std::vector<std::string> lines = currentline;
    for (int i = 0; i < static_cast<int>(options.size()); ++i)
    {
        for (int row = 0; row < static_cast<int>(options[i].size()); ++row)
        {
            std::string prefix;
            if (selectedIndex == i)
            {
                prefix = cursorArt[row];
            }
            else
            {
                prefix = std::string(cursorArt[row].length(), ' ');
            }
            lines.push_back(prefix + options[i][row]);
        }
    }
    return lines;
}
