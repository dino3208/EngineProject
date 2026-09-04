#include "TextBox.h"
#include <Render/Renderer.h>

void TextBox::Draw()
{
	for (int x = 0;x < boxWidth;++x)
	{
		// 위/아래 테두리
		Renderer::Get().Submit("-", Vector2(x, startY), Color::White);
		Renderer::Get().Submit("-", Vector2(x, startY + boxHeight - 1), Color::White);
	}
	// 좌/우 테두리
	for (int y = startY;y < startY + boxHeight;++y)
	{
		Renderer::Get().Submit("|", Vector2(0, y), Color::White);
		Renderer::Get().Submit("|", Vector2(boxWidth - 1, y), Color::White);
	}

	// 네 모서리
	Renderer::Get().Submit("+", Vector2(0, startY), Color::White);
	Renderer::Get().Submit("+", Vector2(boxWidth - 1, startY), Color::White);
	Renderer::Get().Submit("+", Vector2(0, startY + boxHeight - 1), Color::White);
	Renderer::Get().Submit("+", Vector2(boxWidth - 1, startY + boxHeight - 1), Color::White);

	// 메세지 출력
	Renderer::Get().Submit(currentLines, Vector2(2, startY + 1), Color::White);
}

// Getter
std::string TextBox::GetMessageText(MessageType type, int value) const
{
	switch (type)
	{
	case MessageType::Yes:
		return "예";

	case MessageType::No:
		return "아니오";
	
	case MessageType::KeyPickUp:
		return "열쇠를 획득했습니다.";
	case MessageType::DoorPrompt:
		return "문을 열겠습니까?";

	case MessageType::DoorOpened:
		return "문이 열렸습니다.";
	case MessageType::DoorLocked:
		return "문이 잠겨있습니다.";

	case MessageType::Damaged:
		return "피해를 " + std::to_string(value) + "을(를) 입었습니다.";
	}
	return ""; // 안전망
}

std::vector<std::string> TextBox::GetMessageArt(MessageType type, int value) const
{
	switch (type)
	{
	case MessageType::Yes:
		return{
			"#     # #######  #####  ",
			" #   #  #       #     # ",
			"  # #   #       #       ",
			"   #    #####    #####  ",
			"   #    #             # ",
			"   #    #       #     # ",
			"   #    #######  #####  "

		};
	case MessageType::No:
		return{
			"#     # ####### ",
			"##    # #     # ",
			"# #   # #     # ",
			"#  #  # #     # ",
			"#   # # #     # ",
			"#    ## #     # ",
			"#     # ####### "
		};

	case MessageType::KeyPickUp:
		return{
			"######  ###  #####  #    # ####### ######     #     # ######     ####### #     # #######    #    # ####### #     # ",
			"#     #  #  #     # #   #  #       #     #    #     # #     #       #    #     # #          #   #  #        #   #  ",
			"#     #  #  #       #  #   #       #     #    #     # #     #       #    #     # #          #  #   #         # #   ",
			"######   #  #       ###    #####   #     #    #     # ######        #    ####### #####      ###    #####      #    ",
			"#        #  #       #  #   #       #     #    #     # #             #    #     # #          #  #   #          #    ",
			"#        #  #     # #   #  #       #     #    #     # #             #    #     # #          #   #  #          #    ",
			"#       ###  #####  #    # ####### ######      #####  #             #    #     # #######    #    # #######    #    "
		}
			;
	case MessageType::DoorPrompt:
		return{
			"#     #    #    #     # #######    ####### #######    ####### ######  ####### #     #    ####### #     # #######    ######  ####### ####### ######   #####  ",
			"#  #  #   # #   ##    #    #          #    #     #    #     # #     # #       ##    #       #    #     # #          #     # #     # #     # #     # #     # ",
			"#  #  #  #   #  # #   #    #          #    #     #    #     # #     # #       # #   #       #    #     # #          #     # #     # #     # #     #       # ",
			"#  #  # #     # #  #  #    #          #    #     #    #     # ######  #####   #  #  #       #    ####### #####      #     # #     # #     # ######     ###  ",
			"#  #  # ####### #   # #    #          #    #     #    #     # #       #       #   # #       #    #     # #          #     # #     # #     # #   #      #    ",
			"#  #  # #     # #    ##    #          #    #     #    #     # #       #       #    ##       #    #     # #          #     # #     # #     # #    #          ",
			 "## ##  #     # #     #    #          #    #######    ####### #       ####### #     #       #    #     # #######    ######  ####### ####### #     #    #    "
		};

	case MessageType::DoorOpened:
		return{
			"######  ####### ####### ######     ####### ######  ####### #     # ####### ######  ",
			"#     # #     # #     # #     #    #     # #     # #       ##    # #       #     # ",
			"#     # #     # #     # #     #    #     # #     # #       # #   # #       #     # ",
			"#     # #     # #     # ######     #     # ######  #####   #  #  # #####   #     # ",
			"#     # #     # #     # #   #      #     # #       #       #   # # #       #     # ",
			"#     # #     # #     # #    #     #     # #       #       #    ## #       #     # ",
			"######  ####### ####### #     #    ####### #       ####### #     # ####### ######  "
		};
	case MessageType::DoorLocked:
		return{
			"######  ####### ####### ######     #       #######  #####  #    # ####### ######  ",
			"#     # #     # #     # #     #    #       #     # #     # #   #  #       #     # ",
			"#     # #     # #     # #     #    #       #     # #       #  #   #       #     # ",
			"#     # #     # #     # ######     #       #     # #       ###    #####   #     # ",
			"#     # #     # #     # #   #      #       #     # #       #  #   #       #     # ",
			"#     # #     # #     # #    #     #       #     # #     # #   #  #       #     # ",
			"######  ####### ####### #     #    ####### #######  #####  #    # ####### ######  "
		};

	case MessageType::Damaged:
		switch(value)
		{
		case 10:
			return{
			"  #     ###      ######     #    #     #    #     #####  ####### ",
			" ##    #   #     #     #   # #   ##   ##   # #   #     # #       ",
			"# #   #     #    #     #  #   #  # # # #  #   #  #       #       ",
			"  #   #     #    #     # #     # #  #  # #     # #  #### #####   ",
			"  #   #     #    #     # ####### #     # ####### #     # #       ",
			"  #    #   #     #     # #     # #     # #     # #     # #       ",
			"#####   ###      ######  #     # #     # #     #  #####  ####### "

			};
		case 20:
			return{
			" #####    ###      ######     #    #     #    #     #####  ####### ",
			"#     #  #   #     #     #   # #   ##   ##   # #   #     # #       ",
			"      # #     #    #     #  #   #  # # # #  #   #  #       #       ",
			" #####  #     #    #     # #     # #  #  # #     # #  #### #####   ",
			"#       #     #    #     # ####### #     # ####### #     # #       ",
			"#        #   #     #     # #     # #     # #     # #     # #       ",
			"#######   ###      ######  #     # #     # #     #  #####  ####### "

			};
		case 30:
			return{

			};
		case 50:
			return{

			};
		case 70:
			return{

			};
		case 100:
			return{

			};
		}
		break;
	}
	return {};
}

// 정해진 메세지 종류 중 하나를 보여준다. (고정문구)
void TextBox::ShowMessage(MessageType type, int value)
{
	currentLines = { GetMessageText(type, value) };
}

// 이미 만들어진 줄들을 그대로 보여준다. (상황에 따라 조합해서 만든 내용)
void TextBox::ShowLines(const std::vector<std::string>& lines)
{
	currentLines = lines;
}




