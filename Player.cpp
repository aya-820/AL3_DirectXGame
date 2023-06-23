#include "Player.h"

// コンストラクタ
Player::Player() {}

// デストラクタ
Player::~Player() {}

// 初期化
void Player::Initalize(ViewProjection viewProjection) {
	// メンバ変数に代入
	viewProjection_ = viewProjection;

	// プレイヤー
	{
		textureHandlePlayer_ = TextureManager::Load("player.png");
		modelPlayer_ = Model::Create();
		worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformPlayer_.Initialize();
		playerLife_ = 3;
	}

	// インプットクラス
	input_ = Input::GetInstance();
}

// 更新
void Player::Update() {

	// 移動

	// 右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}

	// 左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	// 移動範囲を制限
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4.0f);
	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4.0f);

	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}

// 3D描画
void Player::Drow3D() {
	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}
