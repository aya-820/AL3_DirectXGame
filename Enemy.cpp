#include "Enemy.h"

// コンストラクタ
Enemy::Enemy() {}

// デクリメント
Enemy::~Enemy() {}

void Enemy::Initalize(ViewProjection viewProjection) {
	// メンバ変数に代入
	viewProjection_ = viewProjection;

	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int e = 0; e < 10; e++) {
		worldTransformEnemy_[e].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[e].Initialize();
		aliveFlag_[e] = 0;
	}

	gameTimer_ = 0;
}

void Enemy::Update() {
	gameTimer_++;

	// 移動
	EnemyMove();
	// EnemyJump();

	// 発生
	EnemyBorn();

	for (int e = 0; e < 10; e++) {
		if (/*input_->PushKey(DIK_E) ||*/ worldTransformEnemy_[e].translation_.z <= -10.0f) {
		    aliveFlag_[e] = 0;
		}

		// 変換行列を更新
		worldTransformEnemy_[e].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[e].scale_, worldTransformEnemy_[e].rotation_,
		    worldTransformEnemy_[e].translation_);
		// 変換行列を定数バッファに転送
		worldTransformEnemy_[e].TransferMatrix();
	}
}

void Enemy::Drow3D() {
	// エネミー
	for (int e = 0; e < 10; e++) {
		if (aliveFlag_[e] != 0) {
			modelEnemy_->Draw(worldTransformEnemy_[e], viewProjection_, textureHandleEnemy_);
		}
	}
}

void Enemy::EnemyMove() {
	for (int e = 0; e < 10; e++) {
		if (aliveFlag_[e] == 1) {
			// タイマーにより速度を設定
			worldTransformEnemy_[e].translation_.z -= 0.1f;
			worldTransformEnemy_[e].translation_.z -= gameTimer_ / 3000.0f;

			worldTransformEnemy_[e].translation_.x += enemySpeed_[e];
			// 移動範囲を制限
			if (worldTransformEnemy_[e].translation_.x > 4.0f ||
			    worldTransformEnemy_[e].translation_.x < -4.0f) {
				enemySpeed_[e] *= -1.0f;
			}

			// 回転
			worldTransformEnemy_[e].rotation_.x -= 0.1f;
		}
	}
}

void Enemy::EnemyBorn() {
	if (rand() % 10 == 0) {
		for (int e = 0; e < 10; e++) {
			if (aliveFlag_[e] == 0) {
				aliveFlag_[e] = 1;
				worldTransformEnemy_[e].translation_.z = 40.0f;
				worldTransformEnemy_[e].translation_.y = 0;

				int enemyBornX_ = rand() % 80;
				float enemyBornX2_ = (float)enemyBornX_ / 10 - 4;
				worldTransformEnemy_[e].translation_.x = enemyBornX2_;

				if (rand() % 2 == 0) {
					enemySpeed_[e] = 0.1f;
				} else {
					enemySpeed_[e] = -0.1f;
				}

				break;
			}
		}
	}
}

void Enemy::EnemyJump() { // 敵でループ
	for (int e = 0; e < 10; e++) {
		// 消滅演出ならば
		if (aliveFlag_[e] == 2) {
			// 移動(Y座標に速度を加える)
			worldTransformEnemy_[e].translation_.y += enemyJumpSpeed_[e];

			// 速度を減らす
			enemyJumpSpeed_[e] -= 0.1f;

			// 斜め移動
			worldTransformEnemy_[e].translation_.x += enemySpeed_[e] * 4;

			// 下へ落ちると消滅
			if (worldTransformEnemy_[e].translation_.y < -3) {
				aliveFlag_[e] = 0;
				enemyJumpSpeed_[e] = 0;
			}
		}
	}
}
