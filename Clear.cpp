#include "Clear.h"

Clear::Clear() {}

Clear::~Clear() {}

void Clear::Initalize() {
	// �Q�[���N���A
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

	// �C���v�b�g�N���X
	input_ = Input::GetInstance();
}

int Clear::Update_() {
	timer++;
	// �G���^�[�L�[���������u��
	if (input_->TriggerKey(DIK_RETURN)) {
		//
		audio_->StopWave(voiceHandleBGM_);

		// �^�C�g���ֈڍs
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
	// BGM���Đ�
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleBGM_, true);
}
