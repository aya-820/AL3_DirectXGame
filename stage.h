#pragma once
#include "DirectXCommon.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Stage {
public:
	// 関数
	Stage();  // コンストラクタ
	~Stage(); // デストラクタ

	void Initalize(ViewProjection viewProjection); // 初期化
	void Update();                                 // 更新
	void Drow2DFar();                              // 2D背景描画
	void Droe3D();                                 // 3D描画

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// 変数
	//   BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTrandformStage_[20];
};