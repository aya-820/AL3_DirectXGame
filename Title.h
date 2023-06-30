#pragma once
#include "Beam.h"
#include "DebugText.h"
#include "Enemy.h"
#include "Player.h"
#include "ViewProjection.h"
#include "stage.h"

class Title {
public:
	Title();
	~Title();

	void Initalize();
	void TitleUpdate_();     // タイトル更新
	void TitleDrow2Dnear_(); // タイトル2D

	private:
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// タイトルエンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int timer = 0;
};
