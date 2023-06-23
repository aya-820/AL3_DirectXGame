#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete stage_;
	delete player_;
	delete beam_;
	delete enemy_;
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

	// ステージ
	stage_ = new Stage();
	stage_->Initalize(viewProjection_);

	// プレイヤー
	player_ = new Player();
	player_->Initalize(viewProjection_);

	// ビーム
	beam_ = new Beam();
	beam_->Initalize(viewProjection_, player_);

	// エネミー
	enemy_ = new Enemy();
	enemy_->Initalize(viewProjection_);

	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	// その他
	damageTimer_ = 0;
	gameScore_ = 0;
}

// 更新
void GameScene::Update() {
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	beam_->Update();   // ビーム
	enemy_->Update();  // エネミー
	Collision_();
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

	stage_->Drow2DFar(); // 背景

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

	stage_->Droe3D();
	enemy_->Drow3D();
	beam_->Drow3D();
	if (damageTimer_%15<=7) {
	player_->Drow3D();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", player_->GetLife());
	debugText_->Print(str, 400, 10, 2);

	debugText_->Print("AAA", 10, 10, 2);
	debugText_->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//--------------------------------------------------
// 衝突判定
//--------------------------------------------------

// 衝突判定
void GameScene::Collision_() {
	// 衝突判定(プレイヤーとエネミー)
	CollisionPlayerEnemy_();

	// 衝突判定(とエネミービーム)
	CollisionBeamEnemy_();
}

// 衝突判定(プレイヤーとエネミー)
void GameScene::CollisionPlayerEnemy_() {
	damageTimer_--;
	damageTimer_ = max(damageTimer_, 0);

	for (int e = 0; e < 10; e++) {
		// エネミーが存在すれば
		if (enemy_->GetFlag(e) == 1 && damageTimer_ == 0) {
			// 差を求める
			float dx = abs(player_->GetX() - enemy_->GetX(e));
			float dz = abs(player_->GetZ() - enemy_->GetZ(e));

			// 衝突したら
			if (dx < 1 && dz < 1) {
				enemy_->Hit(e, 2);
				player_->Hit();
				damageTimer_ = 60;

				// プレイヤーヒットSE
				// audio_->PlayWave(soundDateHandlePlayerHitSE_);
			}
		}
	}
}

// 衝突判定(とエネミービーム)
void GameScene::CollisionBeamEnemy_() {

	for (int e = 0; e < 10; e++) {
		for (int b = 0; b < 10; b++) {
			if (beam_->GetFlag(b) == 1 && enemy_->GetFlag(e) == 1) {
				// 差を求める
				float dx = abs(beam_->GetX(b) - enemy_->GetX(e));
				float dz = abs(beam_->GetZ(b) - enemy_->GetZ(e));

				// 衝突したら
				if (dx < 1 && dz < 1) {
					// 存在しない
					beam_->Hit(b);
					enemy_->Hit(e, 2);
					gameScore_++;

					// エネミーヒットSE
					// audio_->PlayWave(soundDateHandleEnemyHitSE_);
				}
			}
		}
	}
}
