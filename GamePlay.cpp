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

	// BGM
	audio_ = Audio::GetInstance();
	soundDateHandleBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDateHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDateHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	// ゲームプレイUI(スプライト)
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {300.0f + i * 26, 0});
		spriteNumber_[i]->SetSize({32, 64});
	}

	textureHandleScore_ = TextureManager::Load("score.png");
	spriteScore_ = Sprite::Create(textureHandleScore_, {160, 0});

	textureHandleLife_ = TextureManager::Load("player.png");
	for (int i = 0; i < 3; i++) {
		spriteLife_[i] = Sprite::Create(textureHandleLife_, {800.0f + i * 60, 0});
		spriteLife_[i]->SetSize({40, 40});
	}

	textureHandleEXBeam_ = TextureManager::Load("go.png");
	spriteEXBeam_ = Sprite::Create(textureHandleEXBeam_, {1000, 0});

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

	beam_->BeamSet(gameScore_);

	if (gameScore_ >= 100) {
		// 現在のBGMを停止
		audio_->StopWave(voiceHandleBGM_);

		// クリアへ移行
		return 3;
	} else if (player_->GetLife() <= 0) {
		// 現在のBGMを停止
		audio_->StopWave(voiceHandleBGM_);

		// ゲームオーバーへ移行
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
	// ゲームプレイUI(スプライト)
	// スコア表示
	DrowScore_();
	spriteScore_->Draw();

	// ライフ表示
	for (int i = 0; i < player_->GetLife(); i++) {
		spriteLife_[i]->Draw();
	}

	if (beam_->GetPower() > 0) {
		spriteEXBeam_->Draw();
	}
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
		for (int b = 0; b < beamNum_; b++) {
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
	// BGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDateHandleBGM_, true);
}

/// --------------------------------------------------
/// ゲームプレイUI(スプライト)
///--------------------------------------------------

void GamePlay::DrowScore_() {
	// 各桁の値を取り出す
	int eachNumber[5] = {};  // 各桁の値
	int number = gameScore_; // 表示する数字

	int keta = 10000; // 最初の桁

	for (int i = 0; i < 5; i++) {
		eachNumber[i] = number / keta; // 今の桁の値を求める
		number %= keta;                // 次の桁以下の値を取り出す
		keta /= 10;                    // 桁を進める

		// 各桁の数値を描画
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}
}