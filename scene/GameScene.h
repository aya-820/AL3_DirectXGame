#pragma once
#include "Clear.h"
#include "GameOver.h"
#include "GamePlay.h"
#include "Title.h"

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

	GamePlay* gamePlay_ = nullptr; // ゲームプレイ
	Title* title_ = nullptr;       // タイトル
	GameOver* gameOver_ = nullptr; // ゲームオーバー
	Clear* clear_ = nullptr;       // クリア

	enum scene {
		gamePlay,
		title,
		gameOver,
		clear,
	};

	int sceneMode_ = title;
};
