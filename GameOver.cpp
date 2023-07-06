#include "GameOver.h"

GameOver::GameOver() {}

GameOver::~GameOver() {}

void GameOver::Initalize() {
	// ゲームオーバー
	{
		textureHandleGameover_ = TextureManager::Load("gameover.png");
		spriteGameover_ = Sprite::Create(textureHandleGameover_, {0, 0});

		textureHandleEnter_ = TextureManager::Load("enter.png");
		spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 500});
		timer = 0;
	}

	// インプットクラス
	input_ = Input::GetInstance();
}

int GameOver::Update_() {
	timer++;
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをタイトルへ変更
		return 1;

		// BGM切り替え
		// audio_->StopWave(voiceHandleBGM_);                                  // 現在のBGMを停止
		// voiceHandleBGM_ = audio_->PlayWave(soundDateHandleTitleBGM_, true); // タイトルBGMを再生
	} else {
		return 2;
	}
}

void GameOver::Drow2Dnear_() {
	spriteGameover_->Draw();
	if (timer % 40 >= 20) {
		spriteEnter_->Draw();
	}
}
