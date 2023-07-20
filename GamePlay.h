#pragma once
#include "Audio.h"
#include "Beam.h"
#include "DebugText.h"
#include "Enemy.h"
#include "Player.h"
#include "ViewProjection.h"
#include "stage.h"
#include "TextureManager.h"

class GamePlay {
public:
	// 関数
	GamePlay();  // コンストラクタ
	~GamePlay(); // デストラクタ

	void Initalize(ViewProjection viewProjection); // 初期化
	int Update_();                                 // ゲームプレイ更新
	void Drow3D_();                                // ゲームプレイ3D表示
	void Drow2DBack_();                            // ゲームプレイ背景2D表示
	void Drow2DNear_();                            // ゲームプレイ近景2D表示

	// 衝突判定
	void Collision_();            // 衝突判定
	void CollisionPlayerEnemy_(); // 衝突判定(プレイヤーとエネミー)
	void CollisionBeamEnemy_();   // 衝突判定(ビームとエネミー)

	void Start_();     // ゲーム開始
	void DrowScore_(); // スコア表示

private:
	// 変数
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// 各クラス
	Stage* stage_ = nullptr;   // ステージ
	Player* player_ = nullptr; // プレイヤー
	Beam* beam_ = nullptr;     // ビーム
	Enemy* enemy_ = nullptr;   // エネミー

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// BGM
	Audio* audio_ = 0;
	uint32_t soundDateHandleBGM_ = 0;         // ゲームプレイBGM
	uint32_t soundDateHandleEnemyHitSE_ = 0;  // 敵ヒットSE
	uint32_t soundDateHandlePlayerHitSE_ = 0; // プレイヤーヒットSE
	uint32_t voiceHandleBGM_;                 // 音声再生ハンドル

	// ゲームプレイUI(スプライト)
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5] = {};
	uint32_t textureHandleScore_ = 0;
	Sprite* spriteScore_ = nullptr;
	uint32_t textureHandleLife_ = 0;
	Sprite* spriteLife_[3] = {};

	// その他
	int gameScore_ = 0;
	int damageTimer_; // プレイヤーダメージクールタイム用変数
};
