#pragma once
#include "Beam.h"
#include "DebugText.h"
#include "Enemy.h"
#include "Player.h"
#include "ViewProjection.h"
#include "stage.h"

class GamePlay {
public:
	// 関数
	GamePlay();  // コンストラクタ
	~GamePlay(); // デストラクタ

	void Initalize(ViewProjection viewProjection); // 初期化
	void GamePlayUpdate_();                        // ゲームプレイ更新
	void GamePlayDrow3D_();                        // ゲームプレイ3D表示
	void GamePlayDrow2DBack_();                    // ゲームプレイ背景2D表示
	void GamePlayDrow2DNear_();                    // ゲームプレイ近景2D表示

	// 衝突判定
	void Collision_();            // 衝突判定
	void CollisionPlayerEnemy_(); // 衝突判定(プレイヤーとエネミー)
	void CollisionBeamEnemy_();   // 衝突判定(ビームとエネミー)

private:
	// 変数
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// 各クラス
	Stage* stage_ = nullptr;       // ステージ
	Player* player_ = nullptr;     // プレイヤー
	Beam* beam_ = nullptr;         // ビーム
	Enemy* enemy_ = nullptr;       // エネミー

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	// その他
	int gameScore_ = 0;
	int damageTimer_; // プレイヤーダメージクールタイム用変数
};
