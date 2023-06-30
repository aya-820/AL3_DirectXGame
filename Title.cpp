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
	}
}

// タイトル更新
void Title::TitleUpdate_() { timer++; }

// タイトル表示
void Title::TitleDrow2Dnear_() {
	spriteTitle_->Draw();
	if (timer % 40 >= 20) {
		spriteEnter_->Draw();
	}
}