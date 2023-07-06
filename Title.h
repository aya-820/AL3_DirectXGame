#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "WorldTransform.h"

class Title {
public:
	Title();
	~Title();

	void Initalize();
	int Update_();     // タイトル更新
	void Drow2Dnear_(); // タイトル2D

	private:
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// タイトルエンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int timer = 0;

	// インプットクラス
	Input* input_ = nullptr;
};
