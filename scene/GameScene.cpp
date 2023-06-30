#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() 
{ 
	delete gamePlay_; 
}

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

	//タイトル
	title_ = new Title;
	title_->Initalize();

}

// 更新
void GameScene::Update() {
	switch (sceneMode_) {
	case gamePlay:
		gamePlay_->GamePlayUpdate_();
		break;
	case	title:
		title_->TitleUpdate_();
		break;
	}

	gamePlay_->GamePlayUpdate_(); // ゲームプレイ
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

	gamePlay_->GamePlayDrow2DBack_(); // ゲームプレイ

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
	
	gamePlay_->GamePlayDrow3D_(); // ゲームプレイ

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
		gamePlay_->GamePlayDrow2DNear_();
		break;
	case title:
		title_->TitleDrow2Dnear_();
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}