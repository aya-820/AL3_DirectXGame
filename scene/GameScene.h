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
	WorldTransform worldTransformBeam_[10];
	void BeamUpdate();
	void BeamMove();
	void BeamBorn();
	int BeamFlag_[10] = {};
	int beamTimer_ = 0;

	// エネミー
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];
	void EnemyUpdate();
	void EnemyMove();
	void EnemyBorn();
	int enemyFlag_[10] = {};
	float enemySpeed_[10] = {};

	// 衝突判定
	void Collision();            // 衝突判定
	void CollisionPlayerEnemy(); // 衝突判定(プレイヤーとエネミー)
	void CollisionBeamEnemy();   // 衝突判定(ビームとエネミー)

	// デバッグテキスト
	DebugText* debugText_ = nullptr;
	int gameScore_ = 0 /* ゲームスコア*/, playerLife_ = 3; /*プレイヤーライフ*/

	// プログラム整理用関数
	void GamePlayUpdate();     // ゲームプレイ更新
	void GamePlayDrow3D();     // ゲームプレイ3D表示
	void GamePlayDrow2DBack(); // ゲームプレイ背景2D表示
	void GamePlayDrow2DNear(); // ゲームプレイ近景2D表示

	// シート切り替え
	int sceneMode_ = 1; // シーンモード(0:ゲームプレイ　1:タイトル)

	// タイトル
	void TitleUpdate();     // タイトル更新
	void TitleDrow2Dnear(); // タイトル2D
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	// タイトルエンター
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	int gameTimer_ = 0;

	// ゲームオーバー
	void gameoverUpdate();
	void gameoverDrow2Dnear();
	uint32_t textureHandleGameover_ = 0;
	Sprite* spriteGameover_ = nullptr;

	// ゲームプレイ初期化
	void GamePlayStart();

	//サウンド
	uint32_t soundDateHandleTitleBGM_ = 0;		//タイトルBGM
	uint32_t soundDateHandleGamePlayBGM_ = 0;	//ゲームプレイBGM
	uint32_t soundDateHandleGameOverBGM_ = 0;	//ゲームオーバーBGM
	uint32_t soundDateHandleEnemyHitSE_ = 0;	//敵ヒットSE
	uint32_t soundDateHandlePlayerHitSE_ = 0;	//プレイヤーヒットSE
	uint32_t voiceHandleBGM_;					// 音声再生ハンドル
};
