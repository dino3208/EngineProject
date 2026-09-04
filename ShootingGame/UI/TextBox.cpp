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

// 이미 만들어진 줄들을 그대로 보여준다. (상황에 따라 조합해서 만든 내용)
void TextBox::ShowLines(const std::vector<std::string>& lines)
{
	currentLines = lines;
}




