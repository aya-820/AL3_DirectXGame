#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() { delete gamePlay_; }

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	{
		viewProjection_.translation_.y = 1;
		viewProjection_.translation_.z = -6;
		viewProjection_.Initialize();
	}

	// ゲームプレイ
	gamePlay_ = new GamePlay;
	gamePlay_->Initalize(viewProjection_);

	// タイトル
	title_ = new Title;
	title_->Initalize();

	// ゲームオーバー
	gameOver_ = new GameOver;
	gameOver_->Initalize();

	//クリア
	clear_ = new Clear;
	clear_->Initalize();
}

// 更新
void GameScene::Update() {
	int oldSceneMode = sceneMode_;

	switch (sceneMode_) {
	case gamePlay:
		sceneMode_ = gamePlay_->Update_(); // ゲームプレイ
		break;
	case title:
		sceneMode_ = title_->Update_(); // タイトル
		break;
	case gameOver:
		sceneMode_ = gameOver_->Update_();
		break;
	case clear:
		sceneMode_ = clear_->Update_();
		break;
	}

	if (oldSceneMode != sceneMode_) {
		switch (sceneMode_) {
		case gamePlay:
			gamePlay_->Initalize(viewProjection_);
			gamePlay_->Start_();
			break;
		case title:
			title_->Start_(); // タイトル開始
			break;
		case gameOver:
			gameOver_->Start_(); // ゲームオーバー開始
			break;
		case clear:
			clear_->Start_();
			break;
		}
	}
}

// 描画
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	gamePlay_->Drow2DBack_(); // ゲームプレイ

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	gamePlay_->Drow3D_(); // ゲームプレイ

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	switch (sceneMode_) {
	case gamePlay:
		gamePlay_->Drow2DNear_();
		break;
	case title:
		title_->Drow2Dnear_();
		break;
	case gameOver:
		gameOver_->Drow2Dnear_();
		break;
	case clear:
		clear_->Drow2Dnear_();
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}