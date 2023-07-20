#include "stage.h"

Stage::Stage() {}

Stage::~Stage() {
	delete spriteBG_;
	delete modelStage_;
}

void Stage::Initalize(ViewProjection viewProjection) {
	// メンバ変数に代入
	viewProjection_ = viewProjection;

	// BG(2dスプライト)
	{
		textureHandleBG_ = TextureManager::Load("bg.jpg");
		spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});
	}

	// ステージ
	{
		textureHandleStage_ = TextureManager::Load("stage2.jpg");
		modelStage_ = Model::Create();
		for (int s = 0; s < 20; s++) {
			worldTrandformStage_[s].Initialize();
			// ステージの位置を変更
			worldTrandformStage_[s].translation_ = {0, -1.5f, 2.0f * s - 5};
			worldTrandformStage_[s].scale_ = {4.5f, 1, 1};
			// 変換行列を更新
			worldTrandformStage_[s].matWorld_ = MakeAffineMatrix(
			    worldTrandformStage_[s].scale_, worldTrandformStage_[s].rotation_,
			    worldTrandformStage_[s].translation_);
			// 変換行列をバッファに転送
			worldTrandformStage_[s].TransferMatrix();
		}
	}
}

void Stage::Update() {
	// 各ステージでループ
	for (int s = 0; s < 20; s++) {
		// 手前に移動
		worldTrandformStage_[s].translation_.z -= 0.3f;
		// 端まで来たら奥へ戻る
		if (worldTrandformStage_[s].translation_.z < -5) {
			worldTrandformStage_[s].translation_.z += 40;
		}
		// 変換行列を更新
		worldTrandformStage_[s].matWorld_ = MakeAffineMatrix(
		    worldTrandformStage_[s].scale_, worldTrandformStage_[s].rotation_,
		    worldTrandformStage_[s].translation_);
		// 変換行列をバッファに転送
		worldTrandformStage_[s].TransferMatrix();
	}
}

void Stage::Drow2DFar() {
	// 背景
	spriteBG_->Draw();
}

void Stage::Droe3D() {
	// ステージ
	for (int s = 0; s < 20; s++) {
		modelStage_->Draw(worldTrandformStage_[s], viewProjection_, textureHandleStage_);
	}
}
