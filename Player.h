#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
public:
	// 関数
	Player();  // コンストラクタ
	~Player(); // デストラクタ

	void Initalize(ViewProjection viewProjection); // 初期化
	void Update();                                 // 更新
	void Drow3D();                                 // 3D描画

private:
	// 変数
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	// インプットクラス
	Input* input_ = nullptr;

public:
	// X座標の獲得
	float GetX() { return worldTransformPlayer_.translation_.x; }
	// Z座標の獲得
	float GetZ() { return worldTransformPlayer_.translation_.z; }
};
