#include "DungeonLevel.h"
#include "Actor/Player.h"
#include "UI/Panel.h"
#include "UI/Lantern.h"
#include "UI/TextBox.h"

using namespace Craft;

void DungeonLevel::OnInitialized()
{
	Level::OnInitialized();

	// 맵 데이터 Todo: 나중에 파일로 수정하는 형식으로 변환
	map.mapData =
	{
		"###########",
		"#.........#",
		"#.#######.#",
		"#.#.....#.#",
		"#.#.###.#.#",
		"#...#...#.#",
		"#.#.#.###.#",
		"#.#...#...#",
		"###########"
	};

	int viewWidth = 313;
	int uiPanelWidth = 156;

	int viewHeight = 106;
	int textBoxHeight = 28;

	// Player 객체 생성 + 월드에 등록 -> 이 객체를 제어할 수 있는 쉐어드 포인터 반환
	std::shared_ptr<Player> player = SpawnActor<Player>();
	// 플레이어한테 레벨이 가진 맵의 주소 저장 명령
	player->map = &map; 
	player->viewWidth = viewWidth;
	player->viewHeight = viewHeight;

	std::shared_ptr<Panel> panel = SpawnActor<Panel>();
	panel->startX = viewWidth;
	panel->panelWidth = uiPanelWidth;
	panel->panelHeight = viewHeight;

	std::shared_ptr<TextBox> textBox = SpawnActor<TextBox>();
	textBox->startY = viewHeight;
	textBox->boxWidth = viewWidth + uiPanelWidth;
	textBox->boxHeight = textBoxHeight;

	SpawnActor<Lantern>();
}