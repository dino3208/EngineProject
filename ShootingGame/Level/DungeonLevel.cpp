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

	// 시야 박스 크기
	int viewWidth = 313;
	int viewHeight = 106;
	
	// UI패널 너비
	int uiPanelWidth = 156;
	// 텍스트박스 높이
	int textBoxHeight = 28;

	// 플레이어 초기 위치 탐색
	for()

	// 플레이어 시야 담당 액터 생성 + 크기 설정
	std::shared_ptr<Player> player = SpawnActor<Player>();
	// 플레이어한테 레벨이 가진 맵의 주소 저장 명령
	player->map = &map; 
	player->viewWidth = viewWidth;
	player->viewHeight = viewHeight;

	// UI 패널 액터 생성 + 크기/위치 설정
	std::shared_ptr<Panel> panel = SpawnActor<Panel>();
	panel->startX = viewWidth;
	panel->panelWidth = uiPanelWidth;
	panel->panelHeight = viewHeight;

	// UI에 랜턴 배치
	std::shared_ptr<Lantern> lantern = SpawnActor<Lantern>();
	lantern->SetPosition(Vector2(viewWidth + 1, viewHeight - 11));
	std::shared_ptr<Lantern> lantern2 = SpawnActor<Lantern>();
	lantern2->SetPosition(Vector2(viewWidth + 135, viewHeight - 11));


	// 하단 메세지 출력용 텍스트박스 생성 + 크기/위치 설정
	std::shared_ptr<TextBox> textBox = SpawnActor<TextBox>();
	textBox->startY = viewHeight;
	textBox->boxWidth = viewWidth + uiPanelWidth;
	textBox->boxHeight = textBoxHeight;
	player->textBox = textBox.get();

}