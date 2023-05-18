#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
	delete spriteTitle_;
	delete spriteEnter_;
}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// BG(2dスプライト)
	{
		textureHandleBG_ = TextureManager::Load("bg.jpg");
		spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});
	}

	// ビュープロジェクションの初期化
	{
		viewProjection_.translation_.y = 1;
		viewProjection_.translation_.z = -6;
		viewProjection_.Initialize();
	}

	// ステージ
	{
		textureHandleStage_ = TextureManager::Load("stage2.jpg");
		modelStage_ = Model::Create();
		for (int s = 0; s < 20; s++) {
			worldTrandformStage_[s].Initialize();
			// ステージの位置を変更
			worldTrandformStage_[s].translation_ = {0, -1.5f, 2.0f * s - 5};
			worldTrandformStage_[s].scale_ = {4.5f, 1, 1};
			// 変換行列を更新
			worldTrandformStage_[s].matWorld_ = MakeAffineMatrix(
			    worldTrandformStage_[s].scale_, worldTrandformStage_[s].rotation_,
			    worldTrandformStage_[s].translation_);
			// 変換行列をバッファに転送
			worldTrandformStage_[s].TransferMatrix();
		}
	}

	// プレイヤー
	{
		textureHandlePlayer_ = TextureManager::Load("player.png");
		modelPlayer_ = Model::Create();
		worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformPlayer_.Initialize();
	}

	// ビーム
	{
		textureHandleBeam_ = TextureManager::Load("beam.png");
		modelBeam_ = Model::Create();
		for (int b = 0; b < 10; b++) {
			worldTransformBeam_[b].scale_ = {0.5f, 0.5f, 0.5f};
			worldTransformBeam_[b].Initialize();
		}
		beamTimer_ = 0;
	}

	// エネミー
	{
		textureHandleEnemy_ = TextureManager::Load("enemy.png");
		modelEnemy_ = Model::Create();
		for (int e = 0; e < 10; e++) {
			worldTransformEnemy_[e].scale_ = {0.5f, 0.5f, 0.5f};
			worldTransformEnemy_[e].Initialize();
		}
	}

	// time.h
	srand((unsigned int)time(NULL));

	// デバッグテキスト
	{
		debugText_ = DebugText::GetInstance();
		debugText_->Initialize();
	}

	// タイトル(2Dスプライト)
	{
		textureHandleTitle_ = TextureManager::Load("title.png");
		spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

		textureHandleEnter_ = TextureManager::Load("enter.png");
		spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 500});
		gameTimer_ = 0;
	}

	// ゲームオーバー
	{
		textureHandleGameover_ = TextureManager::Load("gameover.png");
		spriteGameover_ = Sprite::Create(textureHandleGameover_, {0, 0});
	}

	// サウンド
	{
		// サウンドデータ読み込み
		soundDateHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
		soundDateHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
		soundDateHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
		soundDateHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
		soundDateHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

		// タイトルBGMを再生
		voiceHandleBGM_ = audio_->PlayWave(soundDateHandleTitleBGM_, true);
	}
}

// 更新
void GameScene::Update() {
	switch (sceneMode_) {
	case 0:
		GamePlayUpdate_();
		break;
	case 1:
		TitleUpdate_();
		break;
	case 2:
		gameoverUpdate_();
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

	switch (sceneMode_) {
	case 0:
	case 2:
		GamePlayDrow2DBack_();
		break;
	}

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

	switch (sceneMode_) {
	case 0:
	case 2:
		GamePlayDrow3D_();
		break;
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

	switch (sceneMode_) {
	case 0:
		GamePlayDrow2DNear_();
		break;
	case 1:
		TitleDrow2Dnear_();
		break;
	case 2:
		gameoverDrow2Dnear_();
	}
	debugText_->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//--------------------------------------------------
// ステージ
//--------------------------------------------------
void GameScene::StageUpdate_() {
	// 各ステージでループ
	for (int s = 0; s < 20; s++) {
		// 手前に移動
		worldTrandformStage_[s].translation_.z -= 0.3f;
		// 端まで来たら奥へ戻る
		if (worldTrandformStage_[s].translation_.z < -5) {
			worldTrandformStage_[s].translation_.z += 40;
		}
		// 変換行列を更新
		worldTrandformStage_[s].matWorld_ = MakeAffineMatrix(
		    worldTrandformStage_[s].scale_, worldTrandformStage_[s].rotation_,
		    worldTrandformStage_[s].translation_);
		// 変換行列をバッファに転送
		worldTrandformStage_[s].TransferMatrix();
	}
}

//--------------------------------------------------
// プレイヤー
//--------------------------------------------------

// プレイヤー更新
void GameScene::PlayerUpdate_() {
	// 移動

	// 右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}

	// 左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}

	// 移動範囲を制限
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4.0f);
	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4.0f);

	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}

//--------------------------------------------------
// ビーム
//--------------------------------------------------

// ビーム更新
void GameScene::BeamUpdate_() {
	// 移動
	BeamMove_();

	// 発生
	BeamBorn_();

	for (int b = 0; b < 10; b++) {
		// 変換行列を更新
		worldTransformBeam_[b].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[b].scale_, worldTransformBeam_[b].rotation_,
		    worldTransformBeam_[b].translation_);
		// 変換行列を定数バッファに転送
		worldTransformBeam_[b].TransferMatrix();
	}
}

// ビーム移動
void GameScene::BeamMove_() {
	for (int b = 0; b < 10; b++) {
		if (beamFlag_[b] == 1) {
			worldTransformBeam_[b].translation_.z += 0.7f;

			// 回転
			worldTransformBeam_[b].rotation_.x += 0.1f;

			if (worldTransformBeam_[b].translation_.z > 40.0f) {
				beamFlag_[b] = 0;
			}
		}
	}
}

// ビーム発生
void GameScene::BeamBorn_() {
	beamTimer_++;

	if (beamTimer_ >= 10) {
		for (int b = 0; b < 10; b++) {
			if (beamFlag_[b] == 0) {
				if (input_->PushKey(DIK_SPACE)) {
					worldTransformBeam_[b].translation_.x = worldTransformPlayer_.translation_.x;
					worldTransformBeam_[b].translation_.y = worldTransformPlayer_.translation_.y;
					worldTransformBeam_[b].translation_.z = worldTransformPlayer_.translation_.z;
					beamFlag_[b] = 1;
					beamTimer_ = 0;
					break;
				}
			}
		}
	}
}

//--------------------------------------------------
// エネミー
//--------------------------------------------------

// エネミー更新
void GameScene::EnemyUpdate_() {
	// 移動
	EnemyMove_();
	EnemyJump_();

	// 発生
	EnemyBorn_();

	for (int e = 0; e < 10; e++) {
		if (input_->PushKey(DIK_E) || worldTransformEnemy_[e].translation_.z <= -10.0f) {
			enemyFlag_[e] = 0;
		}

		// 変換行列を更新
		worldTransformEnemy_[e].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[e].scale_, worldTransformEnemy_[e].rotation_,
		    worldTransformEnemy_[e].translation_);
		// 変換行列を定数バッファに転送
		worldTransformEnemy_[e].TransferMatrix();
	}
}

// エネミー移動
void GameScene::EnemyMove_() {
	for (int e = 0; e < 10; e++) {
		if (enemyFlag_[e] == 1) {
			// タイマーにより速度を設定
			worldTransformEnemy_[e].translation_.z -= 0.1f;
			worldTransformEnemy_[e].translation_.z -= gameTimer_ / 3000.0f;

			worldTransformEnemy_[e].translation_.x += enemySpeed_[e];
			// 移動範囲を制限
			if (worldTransformEnemy_[e].translation_.x > 4.0f ||
			    worldTransformEnemy_[e].translation_.x < -4.0f) {
				enemySpeed_[e] *= -1.0f;
			}

			// 回転
			worldTransformEnemy_[e].rotation_.x -= 0.1f;
		}
	}
}

// エネミー発生
void GameScene::EnemyBorn_() {
	if (rand() % 10 == 0) {
		for (int e = 0; e < 10; e++) {
			if (enemyFlag_[e] == 0) {
				enemyFlag_[e] = 1;
				worldTransformEnemy_[e].translation_.z = 40.0f;
				worldTransformEnemy_[e].translation_.y = 0;

				int enemyBornX_ = rand() % 80;
				float enemyBornX2_ = (float)enemyBornX_ / 10 - 4;
				worldTransformEnemy_[e].translation_.x = enemyBornX2_;

				if (rand() % 2 == 0) {
					enemySpeed_[e] = 0.1f;
				} else {
					enemySpeed_[e] = -0.1f;
				}

				break;
			}
		}
	}
}

// エネミージャンプ
void GameScene::EnemyJump_() {
	// 敵でループ
	for (int e = 0; e < 10; e++) {
		// 消滅演出ならば
		if (enemyFlag_[e] == 2) {
			// 移動(Y座標に速度を加える)
			worldTransformEnemy_[e].translation_.y += enemyJumpSpeed_[e];

			// 速度を減らす
			enemyJumpSpeed_[e] -= 0.1f;

			// 斜め移動
			worldTransformEnemy_[e].translation_.x += enemySpeed_[e] * 4;

			// 下へ落ちると消滅
			if (worldTransformEnemy_[e].translation_.y < -3) {
				enemyFlag_[e] = 0;
				enemyJumpSpeed_[e] = 0;
			}
		}
	}
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
	for (int e = 0; e < 10; e++) {
		// エネミーが存在すれば
		if (enemyFlag_[e] == 1) {
			// 差を求める
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[e].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[e].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				enemyFlag_[e] = 2;
				enemyJumpSpeed_[e] = 1.0f;
				playerLife_--;

				// プレイヤーヒットSE
				audio_->PlayWave(soundDateHandlePlayerHitSE_);
			}
		}
	}
}

// 衝突判定(とエネミービーム)
void GameScene::CollisionBeamEnemy_() {
	for (int e = 0; e < 10; e++) {
		for (int b = 0; b < 10; b++) {
			if (beamFlag_[b] == 1 && enemyFlag_[e] == 1) {
				// 差を求める
				float dx = abs(
				    worldTransformBeam_[b].translation_.x - worldTransformEnemy_[e].translation_.x);
				float dz = abs(
				    worldTransformBeam_[b].translation_.z - worldTransformEnemy_[e].translation_.z);

				// 衝突したら
				if (dx < 1 && dz < 1) {
					// 存在しない
					beamFlag_[b] = 0;
					enemyFlag_[e] = 2;
					enemyJumpSpeed_[e] = 1.0f;
					gameScore_++;

					// エネミーヒットSE
					audio_->PlayWave(soundDateHandleEnemyHitSE_);
				}
			}
		}
	}
}

//--------------------------------------------------
// プログラム整理
//--------------------------------------------------

// ゲームプレイ更新
void GameScene::GamePlayUpdate_() {
	PlayerUpdate_(); // プレイヤー更新
	EnemyUpdate_();  // エネミー更新
	BeamUpdate_();   // ビーム更新
	Collision_();    // 衝突判定
	StageUpdate_();

	gameTimer_++;

	if (playerLife_ <= 0) {
		// モードをゲームオーバーへ変更
		sceneMode_ = 2;

		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_); // 現在のBGMを停止
		voiceHandleBGM_ =
		    audio_->PlayWave(soundDateHandleGameOverBGM_, true); // ゲームオーバーBGMを再生
	}
}

// ゲームプレイ3D表示
void GameScene::GamePlayDrow3D_() {
	// ステージ
	for (int s = 0; s < 20; s++) {
		modelStage_->Draw(worldTrandformStage_[s], viewProjection_, textureHandleStage_);
	}

	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	for (int b = 0; b < 10; b++) {
		if (beamFlag_[b] == 1) {
			modelBeam_->Draw(worldTransformBeam_[b], viewProjection_, textureHandleBeam_);
		}
	}

	// エネミー
	for (int e = 0; e < 10; e++) {
		if (enemyFlag_[e] != 0) {
			modelEnemy_->Draw(worldTransformEnemy_[e], viewProjection_, textureHandleEnemy_);
		}
	}
}

// ゲームプレイ背景2D表示
void GameScene::GamePlayDrow2DBack_() {
	// 背景
	spriteBG_->Draw();
}

// ゲームプレイ近景2D表示
void GameScene::GamePlayDrow2DNear_() {
	// デバッグテキスト
	debugText_->Print("AAA", 10, 10, 2);

	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE:%d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	char str2[100];
	sprintf_s(str2, "LIFE:%d", playerLife_);
	debugText_->Print(str2, 400, 10, 2);
}

//--------------------------------------------------
// タイトル
//--------------------------------------------------

// タイトル更新
void GameScene::TitleUpdate_() {
	gameTimer_++;
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		GamePlayStart_();
		// モードをゲームプレイへ変更
		sceneMode_ = 0;

		gameTimer_ = 0;

		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_); // 現在のBGMを停止
		voiceHandleBGM_ =
		    audio_->PlayWave(soundDateHandleGamePlayBGM_, true); // ゲームプレイBGMを再生
	}
}

// タイトル表示
void GameScene::TitleDrow2Dnear_() {
	spriteTitle_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

//--------------------------------------------------
// ゲームオーバー
//--------------------------------------------------

// ゲームオーバー更新
void GameScene::gameoverUpdate_() {
	gameTimer_++;
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをタイトルへ変更
		sceneMode_ = 1;

		// BGM切り替え
		audio_->StopWave(voiceHandleBGM_);                                  // 現在のBGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDateHandleTitleBGM_, true); // タイトルBGMを再生
	}
}

// ゲームオーバー表示
void GameScene::gameoverDrow2Dnear_() {
	spriteGameover_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

// ゲームプレイ初期化
void GameScene::GamePlayStart_() {
	playerLife_ = 3;
	gameScore_ = 0;
	beamTimer_ = 0;
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_->translation_.z = 40;
		worldTransformEnemy_[i].translation_.y = 0;
		enemyFlag_[i] = 0;
		beamFlag_[i] = 0;
	}
	worldTransformPlayer_.translation_.x = 0.0f;

	// プレイヤー
	{ // 変換行列を更新
		worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
		    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
		    worldTransformPlayer_.translation_);
		// 変換行列を定数バッファに転送
		worldTransformPlayer_.TransferMatrix();
	}
	// ビーム＆エネミー
	for (int i = 0; i < 10; i++) {
		// ビーム
		//   変換行列を更新
		worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		    worldTransformBeam_[i].translation_);
		// 変換行列を定数バッファに転送
		worldTransformBeam_[i].TransferMatrix();

		// エネミー
		//   変換行列を更新
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);
		// 変換行列を定数バッファに転送
		worldTransformEnemy_[i].TransferMatrix();
	}
}
