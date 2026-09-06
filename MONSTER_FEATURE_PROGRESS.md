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
