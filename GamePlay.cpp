#include "GamePlay.h"

GamePlay::GamePlay() {}

GamePlay::~GamePlay() {
	delete stage_;
	delete player_;
	delete beam_;
	delete enemy_;
}

void GamePlay::Initalize(ViewProjection viewProjection) {
	viewProjection_ = viewProjection;

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

	//BGM
	audio_ = Audio::GetInstance();
	soundDateHandleBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDateHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDateHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	// その他
	damageTimer_ = 0;
	gameScore_ = 0;
}

int GamePlay::Update_() {
	stage_->Update();  // ステージ
	player_->Update(); // プレイヤー
	beam_->Update();   // ビーム
	enemy_->Update();  // エネミー
	Collision_();      // 衝突判定

	if (player_->GetLife() <= 0) {
		//現在のBGMを停止
		audio_->StopWave(voiceHandleBGM_);

		//ゲームオーバーへ移行
		return 2;
	} else {
		return 0;
	}
}

void GamePlay::Drow3D_() {
	stage_->Droe3D();
	enemy_->Drow3D();
	beam_->Drow3D();
	if (damageTimer_ % 15 <= 7) {
		player_->Drow3D();
	}
}

void GamePlay::Drow2DBack_() {
	stage_->Drow2DFar(); // 背景
}

void GamePlay::Drow2DNear_() {
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", player_->GetLife());
	debugText_->Print(str, 400, 10, 2);

	debugText_->Print("AAA", 10, 10, 2);
	debugText_->DrawAll();
}

//--------------------------------------------------
// 衝突判定
//--------------------------------------------------

// 衝突判定
void GamePlay::Collision_() {
	// 衝突判定(プレイヤーとエネミー)
	CollisionPlayerEnemy_();

	// 衝突判定(とエネミービーム)
	CollisionBeamEnemy_();
}

// 衝突判定(プレイヤーとエネミー)
void GamePlay::CollisionPlayerEnemy_() {
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
				audio_->PlayWave(soundDateHandlePlayerHitSE_);
			}
		}
	}
}

// 衝突判定(とエネミービーム)
void GamePlay::CollisionBeamEnemy_() {

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
					audio_->PlayWave(soundDateHandleEnemyHitSE_);
				}
			}
		}
	}
}

void GamePlay::Start_() {
//BGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleBGM_, true);
}
