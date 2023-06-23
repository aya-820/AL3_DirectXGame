#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
public:
	Enemy();  // コンストラクタ
	~Enemy(); // デクリメント

	void Initalize(ViewProjection viewProjection); // 初期化
	void Update();                                 // 更新
	void Drow3D();                                 // 3D描画

	void EnemyMove(); // 移動
	void EnemyBorn(); // 発生
	void EnemyJump(); // 敵の被弾リアクション

private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];
	int aliveFlag_[10] = {};
	float enemySpeed_[10] = {};
	float enemyJumpSpeed_[10] = {}; // 敵ジャンプの移動速度

	int gameTimer_ = 0;

public:
	// X座標の獲得
	float GetX(int num) { return worldTransformEnemy_[num].translation_.x; }
	// Z座標の獲得
	float GetZ(int num) { return worldTransformEnemy_[num].translation_.z; }
	// 存在フラグの獲得
	int GetFlag(int num) { return aliveFlag_[num]; }
	// 衝突処理
	void Hit(int num, int flag) {
		aliveFlag_[num] = flag;
		enemyJumpSpeed_[num] = 1.0f;
	}
};
