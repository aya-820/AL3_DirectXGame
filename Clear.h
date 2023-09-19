#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "Sprite.h"
#include "Audio.h"

class Clear {
public:
	Clear();
	~Clear();

	void Initalize();
	int Update_();
	void Drow2Dnear_();

	void Start_();

	private:
	uint32_t textureHandleClear = 0;
	Sprite* spriteClear = nullptr;

	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int timer = 0;

	// BGM
	Audio* audio_ = nullptr;
	uint32_t soundDateHandleBGM_ = 0; // BGM
	uint32_t voiceHandleBGM_ = 0;     // 音声再生ハンドル

	// インプットクラス
	Input* input_ = nullptr;
};
