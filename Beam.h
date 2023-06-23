#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"

class Beam {
public:
	// 関数
	Beam();  // コンストラクタ
	~Beam(); // デクリメント

	void Initalize(ViewProjection viewProjection, Player* player); // 初期化
	void Update();                                                 // 更新
	void Drow3D();                                                 // 3D描画

	void BeamMove_(); // 移動
	void BeamBorn_(); // 発生

private:
	// 変数
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[10];
	int shotFlag[10] = {};
	int beamTimer_ = 0;

	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤー
	Player* player_ = nullptr;

public:
	// X座標の獲得
	float GetX(int num) { return worldTransformBeam_[num].translation_.x; }
	// Z座標の獲得
	float GetZ(int num) { return worldTransformBeam_[num].translation_.z; }
	// 存在フラグの獲得
	int GetFlag(int num) { return shotFlag[num]; }
	// 衝突処理
	void Hit(int num) { shotFlag[num] = 0; }
};
