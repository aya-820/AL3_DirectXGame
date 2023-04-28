#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"

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

	// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTrandformStage_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	void PlayerUpdate(); // プレイヤー更新

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	void BeamUpdate();
	void BeamMove();
	void BeamBorn();
	int BeamFlag_ = 0;

	// エネミー
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	void EnemyUpdate();
	void EnemyMove();
	void EnemyBorn();
	int enemyFlag_ = 0;

	//衝突判定
	void Collision(); //衝突判定
	void CollisionPlayerEnemy(); //衝突判定(プレイヤーとエネミー)
	void CollisionBeamEnemy(); // 衝突判定(ビームとエネミー)

	//デバッグテキスト
	DebugText* debugText_ = nullptr;
	int gameScore_ = 0 /* ゲームスコア*/, playerLife = 3; /*プレイヤーライフ*/

	//プログラム整理用関数
	void GamePlayUpdate();		//ゲームプレイ更新
	void GamePlayDrow3D();		//ゲームプレイ3D表示
	void GamePlayDrow2DBack();	//ゲームプレイ背景2D表示
	void GamePlayDrow2DNear();	//ゲームプレイ近景2D表示

	//シート切り替え
	int sceneMode_ = 0;
};
