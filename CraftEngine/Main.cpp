#include <iostream>
#include <Engine/Engine.h>
#include <Level/TestLevel.h>

// #include "Engine/Engine.h" 
// <>보다 ""의 검색범위가 더 넓다.

int main()
{
	// 엔진 객체 생성 및 실행
	Craft::Engine engine;
	engine.AddNewLevel<TestLevel>();
	engine.Run();
}