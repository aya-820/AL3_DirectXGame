#pragma once
#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "WorldTransform.h"

class GameOver {
public:
	GameOver();
	~GameOver();

	void Initalize();
	int Update_();
	void Drow2Dnear_();

private:
	uint32_t textureHandleGameover_ = 0;
	Sprite* spriteGameover_ = nullptr;

	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int timer = 0;

	// インプットクラス
	Input* input_ = nullptr;
};
