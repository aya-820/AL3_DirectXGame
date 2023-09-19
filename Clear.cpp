#include "Clear.h"

Clear::Clear() {}

Clear::~Clear() {}

void Clear::Initalize() {
	// ゲームクリア
	{
		textureHandleClear = TextureManager::Load("clear.png");
		spriteClear = Sprite::Create(textureHandleClear, {0, 0});

		textureHandleEnter_ = TextureManager::Load("enter.png");
		spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 500});
		timer = 0;
	}

	// BGM
	audio_ = Audio::GetInstance();
	soundDateHandleBGM_ = audio_->LoadWave("Audio/clear.wav");

	// インプットクラス
	input_ = Input::GetInstance();
}

int Clear::Update_() {
	timer++;
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		//
		audio_->StopWave(voiceHandleBGM_);

		// タイトルへ移行
		return 1;
	} else {
		return 3;
	}
}

void Clear::Drow2Dnear_() {
	spriteClear->Draw();
	if (timer % 40 >= 20) {
		spriteEnter_->Draw();
	}
}
void Clear::Start_() {
	// BGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleBGM_, true,0.2f);
}
