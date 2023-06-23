#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Beam.h"
#include "Enemy.h"
#include "Player.h"
#include "stage.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// 各クラス
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beam_ = nullptr;     // ビーム
	Enemy* enemy_ = nullptr;   // エネミー

	// 衝突判定
	void Collision_();            // 衝突判定
	void CollisionPlayerEnemy_(); // 衝突判定(プレイヤーとエネミー)
	void CollisionBeamEnemy_();   // 衝突判定(ビームとエネミー)
	int damageTimer_;             // プレイヤーダメージクールタイム用変数

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// その他
	int gameScore_ = 0;
};
