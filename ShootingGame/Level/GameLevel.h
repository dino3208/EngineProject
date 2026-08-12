#pragma once

#include <Level/Level.h>

class GameLevel : public Craft::Level
{
public: 
	// 적 처치시 점수 증가 함수
	void AddScore(int scoreToAdd);

	// 현재 점수 조회
	int GetScore() const;

private:
	// 초기화 이벤트 함수 오버라이드
	virtual void OnInitialized() override;
	
	// 점수 저장 변수
	int score = 0;
};

