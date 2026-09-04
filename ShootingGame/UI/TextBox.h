#pragma once
#include "Actor/Actor.h"
#include <vector>
#include <string>

using namespace Craft;

class TextBox : public Actor
{
public:
	// 텍스트 박스 그리기 위한 변수
	int startY = 0;
	int boxWidth = 0;
	int boxHeight = 0;

	// 출력할 메세지
	enum class MessageType
	{
		Yes,
		No,
		KeyPickUp,
		Damaged
	};

	virtual void Draw() override;

	void ShowLines(const std::vector<std::string>& lines);

	std::vector<std::string> GetMessageArt(MessageType type, int value = 0) const;

private:
	// 현재 메세지를 저장할 멤버 변수
	std::vector<std::string> currentLines;
};

