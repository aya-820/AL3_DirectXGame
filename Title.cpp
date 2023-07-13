#include "Title.h"

Title::Title() {}

Title::~Title() {}

void Title::Initalize() {
	// タイトル(2Dスプライト)
	{
		textureHandleTitle_ = TextureManager::Load("title.png");
		spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

		textureHandleEnter_ = TextureManager::Load("enter.png");
		spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 500});
		timer = 0;

		//BGM
		audio_ = Audio::GetInstance();
		soundDateHandleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
		Start_();

		// インプットクラス
		input_ = Input::GetInstance();
	}
}

// タイトル更新
int Title::Update_() { 
	timer++;

	//エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		//現在のBGMを停止
		audio_->StopWave(voiceHandleBGM_);

		//ゲームプレイへ移行
		return 0;
	} else {
		return 1;
	}
}

// タイトル表示
void Title::Drow2Dnear_() {
	spriteTitle_->Draw();
	if (timer % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

void Title::Start_() {
//BGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleBGM_, true);
}
