#include "Beam.h"

// コンストラクタ
Beam::Beam() {}

// デストラクタ
Beam::~Beam() {}

// 初期化
void Beam::Initalize(ViewProjection viewProjection, Player* player) {
	// メンバ変数に代入
	viewProjection_ = viewProjection;
	player_ = player;

	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	for (int b = 0; b < beamNum_; b++) {
		worldTransformBeam_[b].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformBeam_[b].Initialize();
	}
	beamTimer_ = 0;
	powerBeamFlag_ = 0;
	powerNum_ = 0;
	powerTimer_ = 0;
	speed_ = speedIni_;

	// インプットクラス
	input_ = Input::GetInstance();
}

// 更新
void Beam::Update() {
	// 移動
	BeamMove_();

	// 発生
	BeamBorn_();

	for (int b = 0; b < beamNum_; b++) {
		// 変換行列を更新
		worldTransformBeam_[b].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[b].scale_, worldTransformBeam_[b].rotation_,
		    worldTransformBeam_[b].translation_);
		// 変換行列を定数バッファに転送
		worldTransformBeam_[b].TransferMatrix();
	}
}

// 3D描画
void Beam::Drow3D() {
	for (int b = 0; b < beamNum_; b++) {
		if (shotFlag[b] == 1) {
			modelBeam_->Draw(worldTransformBeam_[b], viewProjection_, textureHandleBeam_);
		}
	}
}

// 移動
void Beam::BeamMove_() {
	for (int b = 0; b < beamNum_; b++) {
		if (shotFlag[b] == 1) {
			worldTransformBeam_[b].translation_.z += speed_;

			// 回転
			worldTransformBeam_[b].rotation_.x += 0.1f;

			if (worldTransformBeam_[b].translation_.z > 40.0f) {
				shotFlag[b] = 0;
			}
		}
	}
}

// 発生
void Beam::BeamBorn_() {
	beamTimer_++;

	if (powerBeamFlag_ == 1) {
		powerTimer_--;

		if (powerTimer_ <= 0) {
			powerBeamFlag_ = 0;
			speed_ = speedIni_;
		}
	}

	if (powerNum_ > 0 && input_->PushKey(DIK_X)) {
		powerBeamFlag_ = 1;
		powerTimer_ = powerTimeMax_;
		speed_ = powerSpeed_;
		powerNum_--;
	}

	if (beamTimer_ >= beamTimeMax_ || powerBeamFlag_ == 1) {
		for (int b = 0; b < beamNum_; b++) {
			if (shotFlag[b] == 0) {
				if (input_->PushKey(DIK_SPACE)) {
					worldTransformBeam_[b].translation_.x = player_->GetX();
					worldTransformBeam_[b].translation_.z = player_->GetZ();
					shotFlag[b] = 1;
					beamTimer_ = 0;
					break;
				}
			}
		}
	}
}

void Beam::BeamSet(int num) {
	if (num % 40 == 0 && num > 0) {
		powerNum_++;
	}
}
