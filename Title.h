#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "WorldTransform.h"	
#include "Audio.h"

class Title {
public:
	Title();
	~Title();

	void Initalize();	//初期化
	int Update_();     // タイトル更新
	void Drow2Dnear_(); // タイトル2D

	void Start_();	//音声再生

	private:
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// タイトルエンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	int timer = 0;
	
	//BGM
	Audio* audio_ = nullptr;
	uint32_t soundDateHandleBGM_ = 0; // タイトルBGM
	uint32_t voiceHandleBGM_ = 0; // 音声再生ハンドル

	// インプットクラス
	Input* input_ = nullptr;

};
