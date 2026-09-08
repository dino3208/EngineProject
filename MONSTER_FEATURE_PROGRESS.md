# 몬스터 추격 기능 — 진행 상황

`ShootingGame`(레이캐스팅 던전 게임)에 "항상 플레이어를 추적하는 몬스터"를 추가하는 작업 기록입니다. 다른 컴퓨터에서 이어서 작업할 때 여기서부터 다시 상황을 파악하면 됩니다.

## 작업 방식

- **코드는 사용자가 직접 작성합니다.** AI(Claude)는 개념 설명·코드 리뷰·방향 제시만 하고, 구현은 대신 해주지 않습니다. 그래서 진행 속도가 느리게 보일 수 있지만 의도된 것입니다.
- 이 과제는 A*(경로탐색)와 Quadtree(공간분할) 알고리즘을 직접 구현해서 활용하는 것이 목표입니다 (둘 다 배운 적은 있고, 복습하면서 구현 중).

## 기능 설계 (확정)

- 몬스터는 **1마리만**, **항상** 플레이어를 추적한다 (감지 범위 없음).
- 몬스터는 **화면에 렌더링하지 않는다** — 공포게임 컨셉으로, 플레이어는 몬스터를 볼 수 없고 부딪혔을 때만 알게 된다.
- 몬스터와 플레이어가 **겹치면 즉시 게임오버** (`CollisionSystem`을 통한 충돌 판정 사용).
- (나중에) 발소리를 거리별 3단계 볼륨으로 재생 예정 — 몬스터가 한 칸 이동할 때마다 발소리 1회. **아직 미착수.**
- 맵은 A*/Quadtree가 의미 있게 동작하도록 기존 11×9보다 크게 만들 예정. 손으로 직접 그림 (자동 생성 알고리즘은 안 씀 — 시간 대비 효율 문제로 보류).

## 현재 완료된 것

1. **[Player.cpp](ShootingGame/Actor/Player.cpp) — 위치를 `Actor`의 `position`에 동기화**
   `Tick()` 끝부분에 `SetPosition(Vector2(static_cast<int>(playerX), static_cast<int>(playerY)));` 추가. 이걸로 `player->GetPosition()`을 통해 외부(나중에 몬스터, 미니맵 등)에서 플레이어의 현재 정수 좌표를 알 수 있음.

2. **[Player.h](ShootingGame/Actor/Player.h) / [Player.cpp](ShootingGame/Actor/Player.cpp) — Player에 생성자 추가 (1×1 히트박스)**
   원래 `Player`는 생성자가 없어서 `Actor`의 기본 생성자(`image=""`)로 초기화되고 있었고, 그 결과 `width=0, height=0`이라 `CollisionSystem`이 충돌을 판정할 수 없는 상태였음. 이제 `Player::Player() : Actor("P", Vector2::Zero, Color::White) {}`로 명시적으로 초기화해서 `width=1, height=1`이 되도록 고침.

3. **[Player.h](ShootingGame/Actor/Player.h) / [Player.cpp](ShootingGame/Actor/Player.cpp) — `SetSpawnPosition(float x, float y)` 추가**
   `playerX`/`playerY`가 `protected`라 외부에서 직접 못 건드리므로, 스폰 위치를 지정할 수 있는 public 세터 추가. [DungeonLevel.cpp](ShootingGame/Level/DungeonLevel.cpp)에서 `player->SetSpawnPosition(9.0f, 1.0f);`로 호출 중 (현재 11×9 맵 기준 값 — 맵 확장하면 다시 맞춰야 함).

   > 참고: 스폰 위치를 맵 문자열에 `'P'` 마커로 심어서 자동으로 찾는 방식도 논의했었으나, 오늘 목표(몬스터 완성)엔 필수가 아니라고 판단해 **보류**. 지금은 하드코딩된 값을 `SetSpawnPosition`으로 명시적으로 전달하는 방식으로 감.

4. **빌드 확인 완료** — `ShootingGame.vcxproj`만 빌드(엔진 재빌드 없이 기존 `Lib/CraftEngine.lib`+`Bin/CraftEngine.dll` 사용), 경고/오류 0개.

## 다음 순서

1. 맵 확장 — [DungeonLevel.cpp](ShootingGame/Level/DungeonLevel.cpp)의 `map.mapData` 리터럴을 더 크게 새로 그리기. 확장 후 `SetSpawnPosition` 좌표도 새 맵에 맞게 조정.
2. `Monster` 액터 뼈대 만들기 — `Actor` 상속, `Map*`/`Player*` 보유, 아직 AI 없음. `Player.h`에 했던 것처럼 1×1 히트박스 생성자 필요.
3. `Player::OnCollision()` 오버라이드 — `Cast<Monster>(other)`로 몬스터인지 확인하고, 맞으면 게임오버 처리 (`TextBox`로 메시지 표시 + `QuitGame()`).
   → 여기까지 하면 "몬스터가 (아직 안 움직여도) 플레이어랑 겹치면 게임오버"가 동작하는지 먼저 확인 가능.
4. `Quadtree` 구현 — 벽(장애물) 좌표를 저장하고 범위/좌표 질의하는 자료구조.
5. `A*` 구현 — 그리드 기반, 4방향 이동, 우선순위 큐 기반 경로탐색. 벽 체크는 Quadtree 질의로.
6. `Monster::Tick()` — 주기적으로 A*로 경로 재계산, 경로 따라 이동, 매 프레임 `SetPosition()`으로 `Actor::position` 동기화 (Player와 동일한 패턴).
7. 최종 빌드 + 로직 검증.

## 알려진 이슈 (보류 중)

- **[ScreenBuffer.cpp:30](CraftEngine/Render/ScreenBuffer.cpp:30) 콘솔 크래시**: `SetConsoleWindowInfo`가 `SetConsoleScreenBufferSize`보다 먼저 호출되는 순서 버그. 콘솔 창을 미리 손으로 키워둔 적 있는 컴퓨터에서만 우연히 동작함. **의도적으로 아직 안 고침** — 지금은 로직 완성이 우선이라, 실제 실행/시각적 확인 없이 코드 레벨에서만 진행 중. 나중에 두 함수 호출 순서를 바꾸면 해결됨.
- **마우스 클릭(LMB/RMB/MMB)이 가끔 인식 안 됨 (2026-09-07 발견 → 시도 후 원인 미확정으로 보류)**: `AStarVisualizer`에서 빠르게 클릭하면 여러 번 중 한 번만 반영되는 현상. 이론상 원인(추정): `Engine::Run()`의 `while(true)` 루프에서 `ProcessInput()`은 매 반복마다 무조건 실행되지만 `Tick()`/`SavePreviousInputStates()`는 목표 프레임 시간이 지났을 때만 실행됨([Engine.cpp:61-135](CraftEngine/Engine/Engine.cpp)) — 그래서 `Tick()` 사이의 "대기 시간" 동안 빠른 클릭의 눌림+뗌 이벤트가 둘 다 처리돼버리면 `isKeyDown`이 이미 `false`로 돌아와 있어서 놓칠 수 있다는 가설. **`Input::KeyState`에 `pressedThisFrame` 스티키 플래그를 추가하는 수정을 실제로 적용하고 테스트까지 해봤지만, 체감상 개선이 확인되지 않아서 다시 되돌림(revert)함.** 즉 이 가설이 틀렸거나, 다른 원인(예: `CraftEngine` 프로젝트가 실제로 재빌드 안 됐을 가능성 — 확인 안 됨)이 있을 수 있음. **원인 미확정 상태로 보류** — 클릭이 완전히 안 되는 건 아니고 "가끔 몇 번 더 눌러야 하는" 수준이라 발표엔 지장 없다고 판단, 시간 되면 나중에 처음부터 다시 진단할 것 (다음엔 `CraftEngine` 재빌드 여부부터 확실히 확인할 것).

---

## 2026-09-07 진행 상황 — 대규모 업데이트

**발표일 정정: 2026-09-09 (모레). 처음엔 내일(09-08)로 착각하고 있었음.**

주말(09-05/06)에 다른 컴퓨터에서 혼자 작업 + 09-07 당일 세션으로 위 "다음 순서" 1~6번을 대부분 완료함. 아래는 최종 확정된 내용 기준으로 다시 정리.

### 설계 변경 (기존 계획 대비)

- **Quadtree는 최종적으로 사용 안 함.** 맵이 이미 촘촘한 2D 그리드([Map.h](ShootingGame/Actor/Map.h)의 `mapData`)라서, A*의 벽 체크는 `Map::IsWall()`을 직접 호출하는 걸로 충분 — 공간분할 자료구조가 풀어줄 문제 자체가 없다고 판단해서 뺌.
- **즉사 게임오버 → 단계별 데미지로 변경.** 몬스터와 충돌 시 1회차 10, 2회차 20, 3회차 30, 4회차 40 데미지 (누적 10→30→60→100), 100 도달 시 게임오버. 충돌 후 3초 스턴을 줘서 플레이어에게 도망갈 틈을 줌.
- **발소리, 랜턴 시야 색깔 구분** — 발표 일정 때문에 스코프에서 제외.

### 완료된 것

1. **A\* 구현** ([Algorithm/Node.h](ShootingGame/Algorithm/Node.h), [Algorithm/AStar.h](ShootingGame/Algorithm/AStar.h)/[.cpp](ShootingGame/Algorithm/AStar.cpp)) — `Map`을 그리드로 변환해서 경로탐색. 이 과정에서 발견한 큰 버그: `FindPath()`가 시작 노드를 `openList`에 넣는 걸 빠뜨려서 경로탐색이 단 한 번도 안 돌고 있었음 (`openList.emplace_back(startNode);` 누락) — 이것 때문에 몬스터가 밤새 제자리에 얼어있었던 원인이었음.
2. **`Monster` 액터** ([Actor/Monster.h](ShootingGame/Actor/Monster.h)/[.cpp](ShootingGame/Actor/Monster.cpp)) — `AStar pathFinder` 멤버로 0.3초마다 플레이어까지 경로 재계산 후 한 칸씩 이동. 화면에 렌더링 안 함(공포 컨셉 유지). `Stun(duration)`으로 피격 후 일시정지 가능.
3. **`Player::OnCollision()` + 단계별 데미지 시스템** ([Actor/Player.cpp](ShootingGame/Actor/Player.cpp)) — `Cast<Monster>`로 충돌 대상 확인, `TextBox`에 데미지/누적데미지 아트 동시 출력, 누적 100 도달 시 `QuitGame()`.
4. **맵 확장 (25×21, 루프형)** ([Level/DungeonLevel.cpp](ShootingGame/Level/DungeonLevel.cpp)) — "몬스터가 계속 쫓아와도 막다른 길에 몰리지 않도록" 모든 통로에 우회로가 있게 손으로 그림. PowerShell BFS/막다른길 검사 스크립트로 전체 연결성 확인 완료 (열림칸 282개, 막다른길은 출구 타일 1개뿐 — 의도된 것).
5. **열쇠 + 출구 승리 조건 (진행중)**:
   - `hasKey` 플래그, `TryPickUpItem()` — 열쇠(`'K'`) 위를 그냥 지나가면 자동으로 습득 (E키 필요 없음)
   - `Player::TryExit()` — E키를 눌렀을 때 상하좌우 인접 칸에 출구(`'G'`)가 있고 `hasKey`면 탈출 시도 (출구 타일 자체가 경계벽 안에 뚫려있어서 "출구 앞"에서 E를 누르는 방식으로 감)
   - 승리 시 플레이어 시야 전체를 덮는 대형 "CLEAR" 아스키아트 출력용으로 [UI/ScreenArt.h](ShootingGame/UI/ScreenArt.h)/[.cpp](ShootingGame/UI/ScreenArt.cpp) 신설 (`TextBox`와 별개 — 텍스트박스는 28줄짜리라 84줄짜리 대형 아트가 안 들어가서 분리함). `GameOver`/`MainScreen` 타입도 미리 만들어놓음 (내용은 아직 빈 자리).
   - **완료** — `TryExit()`는 최종적으로 상하좌우 4방향 검사가 아니라, 예전에 삭제된 `TryOpenDoor()`의 방식(`roundf(playerX + cosf(playerAngle))`으로 "바라보는 방향 한 칸 앞"만 계산)을 재사용하는 걸로 감. 빌드/실행 테스트 완료 (열쇠 습득 → 출구 앞에서 E → CLEAR 화면 확인).

### 다음 순서 (남은 것)

1. ~~`Player::TryExit()` 마무리~~ — **완료**
2. [Monster.cpp](ShootingGame/Actor/Monster.cpp)의 디버그용 좌표 출력 줄(`Renderer::Get().Submit("Monster: "...)`) 최종 빌드 전에 제거 — 아직 안 함
3. **실제 과제 요구사항인 마우스 기반 A* 시각화/디버그 모드** — 진행 중, 아래 참고.

### A* 시각화 모드 (`AStarVisualizer`) — 진행 상황

새 액터 [UI/AStarVisualizer.h](ShootingGame/UI/AStarVisualizer.h)/[.cpp](ShootingGame/UI/AStarVisualizer.cpp) 신설. `Craft::Actor`의 `isPauseImmune`/`Level::SetPaused()`를 활용 — 이 액터만 `isPauseImmune = true`로 해서, 시각화 모드 켤 때 `GetOwner()->SetPaused(true)`로 플레이어/몬스터는 얼리고 이 액터만 계속 동작.

**완료:**
1. `P` 키 토글 — `GetKeyDown('P')`로 `visualizer` bool 뒤집고 그 값으로 레벨 `SetPaused()` 호출
2. `Draw()` — 켜져 있을 때 `map->mapData`를 왼쪽 위(0,0) 기준 1:1 크기로 그리고, 벽/바닥 색 구분, `player`/`monster`의 실제 위치(`GetPosition()` — `Actor`의 public 함수라 `Monster`용 별도 getter 불필요)를 `P`/`M` 마커로 표시
3. `DungeonLevel.cpp`에 스폰 + `map`/`player`/`monster` 포인터 연결 완료
4. **빌드/실행 테스트 완료** — P로 토글, 격자·마커 정상 표시, 토글 중 플레이어/몬스터 정지, 재토글 시 정상 재개까지 확인.

**완료 (2026-09-07 매우 늦은 밤) — 과제 핵심 요구사항 달성**:
1. ~~마우스 좌클릭(LMB)=시작점, 우클릭(RMB)=목표점, 휠클릭(MMB)=장애물 토글~~ — 완료. `startPoint`/`goalPoint`(`Position`, 기본값 `(-1,-1)`="미지정"), `obstacles`(`std::vector<Position>`, 같은 칸 다시 클릭하면 목록에서 제거=토글) 멤버 추가.
2. ~~`AStar::FindPath()` 실행 + 경로 하이라이트~~ — 완료. 시작/목표가 둘 다 지정되면 매 프레임 `Map tempMap = *map;`로 복사본을 만들어 `obstacles` 위치를 `'#'`로 덮어쓴 뒤 그 복사본으로 `FindPath()` 실행 (진짜 맵은 절대 안 건드림) → 결과 경로(`currentPath`)를 파란 점으로 표시. **그리는 순서 버그**: 처음엔 경로가 시작/목표 칸까지 포함해서 그 위에 파란 점을 찍는 바람에 `S`/`E` 마커가 가려짐 → `S`/`E`를 경로보다 나중에(위에) 그리도록 순서만 바꿔서 해결 (`TextBox` 테두리+글자와 같은 "나중에 그린 게 위에 덮임" 원리).
3. **빌드/실행 테스트 완료** — 좌/우/휠클릭 다 동작, 장애물 놓으면 경로가 실시간으로 우회하는 것까지 확인. **과제의 리터럴 요구사항(LMB=시작, RMB=목표, MMB=장애물, A* 경로 시각화)을 모두 충족.**

### 미니맵 추가 결정 (2026-09-07, 계획 재정리)

플레이어 피드백: 열쇠 위치를 모른 채 몬스터 피해다녀야 해서 불편함 → **오른쪽 `Panel` 안에 항상 켜져있는 미니맵** 추가하기로 함 (P 모드는 게임을 멈추는 디버그용이라 실시간 플레이 중엔 못 씀, 서로 다른 용도).

**설계 (중복 코드 방지, DRY)**: 격자 그리는 로직을 `Map::DrawGrid(int startX, int startY, Color color) const`로 뽑아서 `AStarVisualizer`(전체화면, 토글용)와 새 `Minimap`(패널 구석, 항상 켜짐) 둘 다 재사용.

**단순화 결정 (여러 차례 논의 후)**:
- 처음엔 칸마다 벽/바닥 색 다르게 + 확대(`cellSize`) 계획이었는데, `TextBox::Draw()`에서 이미 쓰던 `Renderer::Get().Submit(vector<string>, Vector2, Color)` 오버로드로 `mapData`를 통째로 한 번에 찍는 게 훨씬 간단하다는 걸 확인 → **칸별 색 구분과 확대 둘 다 지금은 보류**, `DrawGrid`는 단일 색상 매개변수만 받는 한 줄짜리 함수로 단순화.
- 확대(줌)는 마우스 클릭 정확도엔 도움 안 됨(`GetMousePosition()`이 이미 콘솔 칸 단위라서) — 순전히 가독성 문제라 우선순위 낮음. 넣게 되면 `mapX = (mouseX - startX) / cellSize;`처럼 나누기 한 단계만 추가하면 됨 (구현 자체는 어렵지 않음, 아직 진행 여부 미정).

**완료 (2026-09-07 늦은 밤)**:
1. ~~`Map::DrawGrid(int startX, int startY, Color color) const` 구현~~ — 완료, `mapData`를 `Submit(vector<string>, Vector2, Color)`로 한 번에 그리는 한 줄짜리 함수로 정리
2. ~~`AStarVisualizer::Draw()` 리팩터링~~ — 완료, `map->DrawGrid(0, 0, Color::White)` 호출로 교체, 기존 수동 반복문 삭제
3. ~~`Minimap` 액터 신설~~ — 완료. `map->DrawGrid(startX, startY, ...)` + P/M 마커(좌표에 `startX`/`startY` offset 적용) + **열쇠(`K`) 위치 강조**(맵을 순회하며 `'K'` 찾아서 노란색으로 덧그림 — 주우면 `TryPickUpItem()`이 `'.'`로 바꿔버려서 자동으로 사라짐)
4. ~~`DungeonLevel.cpp`에 `Minimap` 스폰 + 위치 연결~~ — 완료. `panel->startX + 2`, `2`로 배치 (패널 테두리 안쪽)
5. **빌드/실행 테스트 완료** — 미니맵이 패널에 항상 뜨고, 열쇠 위치가 노란색으로 보임, P 모드도 정상 동작.

**이 과정에서 만난 엔진 버그 하나 고침**: `Map.h`에 `DrawGrid` 선언 위해 `#include <Math/Color.h>` 추가하면서 `Windows.h`가 `AStar.cpp`까지 전파됨 → `Windows.h`가 정의하는 `min`/`max` 매크로가 `AStar.cpp`의 `std::min`/`std::max` 호출(휴리스틱 계산, 오각선 거리)을 깨뜨림. **`AStar.cpp` 맨 위에 `#define NOMINMAX` 추가해서 해결** (처음엔 `CraftEngine/Math/Color.h`에 넣어서 프로젝트 전체에 영구 적용하는 걸 제안했었으나, 실제로는 `AStar.cpp`에만 적용됨 — 이 파일 한정으로는 확실히 고쳐졌지만, 나중에 다른 `.cpp`가 `Windows.h`를 끌어들이면서 `std::min`/`std::max`를 쓰면 같은 문제가 재발할 수 있음. 재발하면 `Color.h`로 옮기는 걸 고려할 것.).

### 확대(zoom) 결국 추가함 (2026-09-07 새벽)

보류했던 확대 기능을 사용자가 "그래도 하고 싶다"고 해서 결국 구현. `AStarVisualizer`/`Minimap` 둘 다 각자의 `cellSize` 멤버(기본값 각각 6, 4)를 갖고, `Map::DrawGrid`/`DrawCell`에 매번 넘겨주는 방식.

- **`Map::DrawGrid(startX, startY, cellSize, color)` 재설계**: 한 칸씩 `Submit` 여러 번 부르는 대신, 확대된 `vector<string>`을 미리 다 만들어놓고 **`Submit`은 딱 한 번만** 호출 (`std::string(cellSize, tile)`로 가로 반복, `emplace_back`으로 세로 반복). 예전에 사용자가 제안했던 "EnlargeGrid를 따로 만들어서 조합하자"는 아이디어를 실제로 구현한 형태.
- **`Map::DrawCell(startX, startY, cellSize, cellX, cellY, ch, color)` 신설**: 마커(P/M/S/E/K/X/경로) 하나를 `cellSize x cellSize` 색칠된 블록으로 그림. `DrawGrid`처럼 `Renderer::Get().Submit(...)`을 매번 새로 계산하지 않고, 한 칸의 시작 화면좌표(`baseX`/`baseY`)만 미리 계산해두고 재사용.
- **마우스 좌표 변환 필요**: 화면이 `cellSize`배 커졌으니, 클릭한 화면 좌표를 다시 맵 칸 좌표로 되돌리려면 `cellSize`로 나눠야 함 (`Position clickedMapPos(mousePos.x / cellSize, mousePos.y / cellSize);`) — `AStarVisualizer::Tick()`에 반영.
- **빌드/실행 테스트 완료** — 지도와 마커 모두 확대되어 표시되고, 마우스 클릭도 정확한 칸에 반영됨.
- **잡은 버그**: MMB 처리 블록 안에서 이미 계산해둔 `clickedMapPos`를 `Position clickedMapPos;`로 **다시 선언**해버려서 항상 `(0,0)`으로 덮어써지던 변수 섀도잉 버그 — 재선언 줄을 삭제해서 해결.
- (칸별 벽/바닥 색 구분은 여전히 미적용 — `DrawGrid`가 단일 색만 받음. 지금은 굳이 필요 없다고 판단, 보류 유지.)

**남은 순서**:
1. 마우스 입력 (LMB/RMB/MMB) — `AStarVisualizer` 전용, 시작/목표/장애물 지정
2. `AStar::FindPath()` 실행 + 경로 하이라이트 — `AStarVisualizer` 전용
3. (선택, 미정) 칸별 색 구분 / 확대(zoom) 다시 검토
4. `Monster.cpp` 디버그 출력 줄 제거

### 승리화면 최종 완성 (2026-09-07 늦은 밤)

빌드/실행 테스트 완료 — 열쇠 습득 → 출구 앞에서 E → 승리화면이 화면 정중앙에 뜨고, 몇 초 뒤 아무 키나 누르면 종료되는 것까지 확인.

**중간에 잡은 버그들 (교훈으로 기록)**:
- **게임 루프 타이밍 버그**: `Engine::Run()`은 `Tick() → 충돌처리 → Draw()` 순서로 도는데, 승리 아트를 `TryExit()`(즉 `Tick()` 도중)에서 바로 `Submit`했더니 같은 프레임의 `Player::Draw()`(3D 벽 그리기)가 그 위를 덮어써버려서 화면에 안 보였음. 해결: `TryExit()`은 `hasWon` 플래그만 켜고, 실제 그리기는 `Player::Draw()`가 맨 위에서 `if (hasWon)`으로 분기해서 담당하도록 분리.
- **`Vector2` 암시적 변환 함정**: `Vector2(int x = 0, int y = 0)` 생성자가 두 매개변수 다 기본값이 있어서, `어떤Vector2 / 2`처럼 정수 하나만 나눗셈에 써도 컴파일러가 `2`를 `Vector2(2, 0)`로 암시적 변환해버림 → `Vector2::operator/`의 `assert(other.x != 0 && other.y != 0)`가 `y=0`이라 실패해서 크래시. 괄호 위치를 "정수 나눗셈 결과를 `Vector2` 생성자에 넣기"로 고쳐서 해결 (`Vector2(0, (a - b) / 2)`처럼 나눗셈이 `Vector2` 생성자 *안에서* 끝나게).
- **즉시 종료 문제**: 승리 즉시 `QuitGame()`을 불러서 화면이 한 프레임만 그려지고 바로 꺼짐 → `hasWon` 플래그 + `winScreenTimer`(유예시간, `stunTimer`와 같은 패턴)를 추가해서, 일정 시간 지난 후 아무 키나 눌러야 `QuitGame()`이 호출되게 변경. 이 과정에서 `TryExit()`에 옛날 `QuitGame()` 호출이 안 지워지고 남아있던 게 원인이었던 적도 있었음 — 리팩터링할 때 옛 코드 삭제 여부를 항상 재확인할 것.
- **아트 변경**: 84줄짜리 큰 CLEAR 아스키아트가 잘 안 나와서, `TextBox`의 `Damaged`/`TotalDamage`처럼 작은(7줄) 블록체 "CLEAR" 텍스트로 교체. 가로/세로 모두 `(전체너비 - 아트너비)/2` 공식으로 화면 정중앙에 배치.

### 향후 아이디어 (계획만, 미구현)

- **몬스터 거리 기반 가변 속도**: 플레이어와 멀 땐 빠르게, 가까워지면 살짝 느리게 — 긴장감 부족 피드백(2026-09-07)에 대한 아이디어. 지금은 시각화 모드가 우선이라 보류.
- ~~**미니맵 부재**~~ — **해결됨**, 위 참고.

---

## 2026-09-08 진행 상황 — 발표 전날, 마무리 작업

**발표는 오늘(09-08)이 아니라 내일(09-09).** 어제 핵심 기능(추격/데미지/승리조건/미니맵/마우스 A* 시각화) 전부 완성 + 커밋(`83bcd0c`)/푸시 완료한 상태에서 이어감.

- ~~**탈출 조건 간소화**~~ — **완료**. E키로 "바라보는 방향 앞" 체크하던 방식(`roundf(playerX + cosf(playerAngle))`)을 버리고, `TryPickUpItem()`처럼 **"내가 서 있는 칸이 G인지"**로 단순화. E키 조건도 없애고 `TryPickUpItem()`처럼 매 프레임 자동 호출로 변경 — 이제 열쇠를 가진 채 출구 칸을 밟기만 하면 바로 승리.
  - **잡은 버그**: 범위체크 두 번째 줄(`x` 확인)에서 첫 번째 줄(`y` 확인)의 `map->mapData.size()`(세로 줄 개수, 21)를 그대로 복사해다 써서, `x`(가로 위치, 최대 24)를 21이랑 비교해버림 → 출구(`x=23`)가 "범위 밖"으로 잘못 판정되어 `return`, `G` 체크 자체를 못 감. `map->mapData[y].length()`(그 줄의 가로 길이, 25)로 고쳐서 해결.
- 발표 준비 방식 확정: 시연 순서 따로 안 짜고, **구동 영상 녹화 → 장면 컷 편집 → GIF 변환 → PPT에 삽입**으로 진행 예정.
- ~~**칸별 벽 색 구분**~~ — 계획 변경 후 **완료**: 원래 벽/바닥 전체를 색 구분하려던 계획 대신, **테두리 4방향(위=빨강, 아래=노랑, 왼쪽=초록, 오른쪽=파랑)만 별도 색**으로 칠하는 걸로 축소. `Map::DrawCell`을 재사용해서 미니맵에 테두리 4줄 덧그림.
  - **3D 플레이어 시야에도 반영**: `CastRay()`가 거리뿐 아니라 **부딪힌 칸의 좌표(`hitX`/`hitY`, 참조 매개변수로 출력)**까지 알려주게 확장 → `CastAllRays()`가 `wallHitX`/`wallHitY` 배열(`distances`와 같은 방식)에 저장 → `Player::Draw()`에서 새 함수 `TryGetBorderColor(map, hitX, hitY, outColor)`(불리언 반환 + 참조 출력 패턴, 테두리면 방향색 채우고 true, 아니면 false)로 분기해서 테두리면 방향색, 아니면 기존 거리 기반 색(`GetWallColor`) 사용.
  - **`Lantern` 시야 제한 로직 제거**: 랜턴 반경 넘으면 안 그리던 `lanternRadius`/`lanternFalloff` 관련 코드 전부 삭제 (UI 장식용 `Lantern` 액터 자체는 유지) — 시야가 너무 좁다는 피드백으로 제거, `GetWallChar`가 이미 자체적으로 먼 거리를 공백 처리해줘서 문제없음.
  - **미해결 이상 현상 (버그 여부 미확정)**: 스폰 지점(왼쪽 테두리 바로 옆)에서 특정 각도로 볼 때, 직관적으로 예상한 색(예: 파랑)이 아니라 다른 테두리색(빨강)이 보이는 경우 발견. 가설: 미로 통로가 그 방향으로 쭉 이어지다가 실제로는 다른 쪽 테두리 모서리에 닿아서 그런 것(미로라서 대칭이 안 맞을 수 있음) — 그러나 `TryGetBorderColor`가 모서리 칸에서 상하 검사를 좌우보다 먼저 하는 순서상 우선순위 문제일 가능성도 배제 못함. 디버그 출력으로 정확한 `hitX`/`hitY` 값 확인하려다 시간 관계상 보류, 발표에 큰 지장 없다고 판단.
- ~~**몬스터 거리 기반 가변 속도**~~ — **완료**. `Monster.cpp`에 `GetMoveInterval(distance)` 함수 추가 (멀면 0.15초, 보통 0.3초, 가까우면 0.5초 간격 — 최종적으로 사용자가 각각 0.1/2.0/2.0으로 재조정). 두 가지 버그를 거쳐 완성:
  - 처음엔 `distance`만 계산해두고 실제로 `if (moveTimer >= moveInterval)`(옛날 고정 상수)를 그대로 써서 아무 효과가 없었음 → `GetMoveInterval(distance)` 호출로 교체.
  - 그다음엔 `if (moveTimer >= GetMoveInterval(distance));`처럼 조건문 뒤에 세미콜론이 잘못 붙어서 그 아래 `{ }` 블록이 조건과 무관하게 매 프레임 무조건 실행되던 버그 → 세미콜론 제거로 해결. (`if(조건);`은 컴파일 에러 없이 조용히 논리를 깨버리는 클래식 실수라 요주의.)
- ~~**미니맵 몬스터 일정 간격만 표시**~~ — **완료**. `MiniMap`에 `blipTimer`/`blipInterval`(3초)/`blipVisibleDuration`(0.5초) 추가, 매 프레임 `blipTimer`를 증가시키다 주기 넘으면 리셋, `blipTimer < blipVisibleDuration`일 때만 M 마커 그림 — 레이더 신호처럼 깜빡이는 효과.
- ~~**메인화면**~~ — **완료**. 새 `Level` 서브클래스 `MainMenu`(`ShootingGame/Level/MainMenu.h`/`.cpp`) 신설 — `Tick()`에서 아무 키나(256개 키 코드 반복 확인, 승리화면 패턴 재사용) 감지되면 `Engine::Get().AddNewLevel<DungeonLevel>()`로 전환, `Draw()`에서 `ScreenArt::ScreenType::MainScreen` 그림. `Main.cpp`가 `DungeonLevel` 대신 `MainMenu`로 시작하게 변경.
  - 제목 아트: 처음엔 유니코드 박스/블록 문자(`█`, `═`, FIGlet "ANSI Shadow" 등) 시도했으나 전부 멀티바이트라 콘솔에서 못 씀 → Claude가 순수 아스키 5x7 블록체로 "DUNGEON" 직접 생성(스크립트로 폭 검증, 41칸 통일 확인) → 이후 사용자가 직접 다른 변환 도구로 13줄/136칸짜리 더 정교한 아트로 교체, `file` 명령으로 순수 ASCII 확인 완료. 화면 전체(469x134, `Config/Setting.txt` 기준) 중앙 좌표 계산: `x=(469-136)/2=166`, `y=(134-13)/2=60`.
  - **잡은 버그(중요)**: `MainMenu::Draw()`에 `Level:;Draw();`라고 오타 — `::`가 아니라 `:;`. 이번엔 컴파일 에러가 안 나고 **스택 오버플로우 크래시**로 이어짐: `Level:`이 goto 레이블로 파싱되고 `Draw();`가 대상 없이 호출되면서 `virtual` 함수라 자기 자신(`MainMenu::Draw()`)을 무한 재귀 호출 → 스택 소진. `Level::Draw();`로 수정해서 해결. (`:;`/`::` 오타가 이 세션 내내 반복됐는데, 이번처럼 "조용히 다른 의미로 컴파일되어 런타임에 크래시"하는 경우도 있다는 걸 기억해둘 것.)
- ~~**승리화면 → 메인화면 복귀**~~ — **완료**. `Player::Tick()`의 `hasWon` 블록에서 `QuitGame()` 대신 `Engine::Get().AddNewLevel<MainMenu>();`로 교체.
- ~~**게임오버 화면**~~ — **완료**. 승리화면(`hasWon`)과 동일한 구조로 `hasLost`/`lossScreenTimer` 플래그 추가 — `OnCollision()`에서 `totalDamage >= 100`이면 플래그만 세우고, 실제 "대기 + 아무 키나 감지 + 메인화면 복귀"는 `Tick()`에서 매 프레임 처리 (`hasWon`과 거의 동일 코드). 처음엔 `OnCollision()` 안에서 직접 아무 키나 검사하려다 실패 — `OnCollision()`은 충돌 난 그 한 프레임에만 호출되니까 매 프레임 도는 `Tick()`과 달리 "대기"가 안 됨, 승리화면 때와 똑같은 종류의 타이밍 실수.

### 사운드 시스템 추가 (2026-09-08, 별도 인강 진도 병행)

`CraftEngine`에 XAudio2 기반 `Sound` 클래스(싱글톤, 별도 `SoundSystem` 프로젝트에서 `.lib`/`.dll`로 미리 빌드됨) 연동. `Engine`이 `PlayOneShot`/`PlayBackGroundMusic`/`StopBackGroundMusic` 래퍼 함수로 감쌈.

**연동 과정에서 잡은 버그들 (전부 오타/설정 실수, 로직 자체는 인강 그대로)**:
- `Includes/`, `Library/` 폴더 안에 `SoundSystem` 폴더명이 `SoundSytem`(s 하나 빠짐)으로 오타 + 폴더가 한 번 더 중첩(`SoundSystem/SoundSystem/`) — 프로젝트 설정의 include/lib 디렉토리 철자랑 실제 폴더명이 어긋나서 `LNK1104`(라이브러리 못 엶) 및 헤더 못 찾는 에러 발생. 실제 폴더 정리 + `AdditionalLibraryDirectories` 오타 수정으로 해결.
- `ShootingGame.vcxproj`의 사전 빌드 이벤트에서 `$(OutDir)`를 `&(OutDir)`로 오타(달러 기호 대신 앰퍼샌드) → MSBuild 매크로로 인식 안 돼서 `xcopy`가 엉뚱한 경로로 복사 시도, `MSB3073`(코드 255) 에러. `$`로 수정.
- `SoundSystem.dll`이 `.lib`만으로는 실행 시점에 자동으로 안 따라옴 — `CraftEngine`의 빌드 후 이벤트에 `xcopy ..\Library\SoundSystem\$(Configuration)\SoundSystem.dll $(OutDir) /y` 추가해서 실행 파일 폴더로 복사되게 함.
- `Engine::PlayOneShot()`의 `std::string("..\Assets\Sound")` — 문자열 리터럴 안 백슬래시가 이스케이프 시퀀스(`\A`, `\S`)로 오인식되어 경로가 깨짐 → 슬래시(`/`)로 교체(`"../Assets/Sound/"`), 폴더-파일명 사이 구분자 누락도 같이 발견해서 수정.
- `Sound::LoadSoundEffect()`가 파일을 못 열어도 조용히 `return`(에러 메세지 없음)이라 원인 파악이 어려웠음 — `Engine::PlayOneShot()`에 임시로 `std::ifstream`으로 파일 열기 성공 여부를 화면에 찍어보는 디버그 코드를 넣어서 확인, 문제 확정 후 제거.
- `PlayOneShot("Roar")`처럼 확장자 없이 호출 — `Sound`는 자동으로 `.wav`를 안 붙여줘서 파일을 못 찾음. 호출부에서 `"Roar.wav"`/`"Damage.wav"`처럼 확장자까지 명시하는 걸로 해결.

**최종 적용된 사운드**:
- `Player::OnCollision()` 데미지 판정 시 `Damage.wav` 재생
- `Monster::Tick()`에서 플레이어와의 거리가 일정 범위(최종 3.0f) 이내로 들어온 **최초 순간에만** `Roar.wav` 재생 + 배경음악 정지(`isNearby` 플래그로 가장자리 감지 — 몬스터는 계속 추적하므로 한 번 가까워지면 다시 멀어지지 않는다는 게임 특성상 배경음악은 재개 안 시키기로 결정)
  - **잡은 버그**: `else`가 안쪽 `if(!isNearby)`에 잘못 붙어서, 범위 안에 있는 동안 매 프레임 "재생→리셋→재생→리셋"이 반복되어 소리가 끊임없이 겹쳐 재생됨 → `else`를 바깥쪽 `if(distance <= 범위)`에 붙이도록 중괄호 위치 수정.
- `MainMenu::OnInitialized()`에서 `MainMenu.wav` 배경음악 재생, `DungeonLevel::OnInitialized()`에서 `GameLevel.wav` 배경음악 재생 — `Sound::PlayBackgroundMusic()`이 내부적으로 이전 곡을 자동 정지하고 교체해줘서 레벨 전환 시 별도 처리 불필요.

### 충돌 판정 첫 프레임 오탐 버그 발견/수정

몬스터/플레이어가 스폰 직후 실제로는 9칸 넘게 떨어져 있는데도 게임 시작하자마자 한 번 데미지를 맞는 현상 발견. 원인: `CollisionSystem`이 "이전 프레임 위치~현재 위치"를 모두 포함하는 swept AABB로 충돌을 계산하는데, `Actor::previousPosition`의 기본값이 `(0,0)`이고 `SetSpawnPosition()`은 `playerX`/`monsterX`같은 별도 float 변수만 설정할 뿐 `Actor::position`/`previousPosition`은 그대로 둠 → 스폰 첫 프레임엔 "이전 위치(0,0)~현재 위치(스폰지점)"라는 실제로 존재하지도 않는 거대한 범위로 충돌 계산되어, 플레이어와 몬스터의 두 스윕 범위가 원점 근처에서 우연히 겹쳐 오탐 발생. `Player`/`Monster`의 `SetSpawnPosition()`에 `SetPosition(...)` + `SavePreviousState()` 호출을 추가해서, 스폰 시점에 `position`과 `previousPosition`을 즉시 동일하게 맞춰 해결.

### 튜토리얼 안내 메세지 추가

게임 시작 시 `TextBox`에 "미니맵의 노란 표시 = 열쇠 위치" 안내를 한 번 보여주는 `MessageType::Tutorial` 추가, `DungeonLevel::OnInitialized()`에서 `textBox` 지역변수 만든 직후 바로 `ShowLines`로 출력 (별도 멤버 변수 없이 지역변수 스코프 안에서 처리).

**현재 상태: 계획했던 모든 필수/선택 기능 구현 완료.** 발표는 2026-09-09. 다음 할 일은 전체 빌드/플레이 테스트 → 구동 영상 녹화 → 장면 컷 편집/GIF 변환 → PPT 삽입.
