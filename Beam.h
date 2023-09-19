#pragma once
#include "Player.h"

	const int beamNum_ = 64;

class Beam {
public:
	// 関数
	Beam();  // コンストラクタ
	~Beam(); // デクリメント

	void Initalize(ViewProjection viewProjection, Player* player); // 初期化
	void Update();                                                 // 更新
	void Drow3D();                                                 // 3D描画

	void BeamMove_(); // 移動
	void BeamBorn_(); // 発生

	void BeamSet(int num); // EXビームセット

private:
	// 変数
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[beamNum_];
	int shotFlag[beamNum_] = {};
	int beamTimer_ = 0;
	const int beamTimeMax_ = 10;
	int powerBeamFlag_ = 0;
	int powerNum_ = 0;
	int powerTimer_ = 0;
	const int powerTimeMax_ = 120;
	float speed_ = 0.7f;
	const float speedIni_ = 0.7f;
	const float powerSpeed_ = speedIni_ * 2.0f;

	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤー
	Player* player_ = nullptr;

public:
	// X座標の獲得
	float GetX(int num) { return worldTransformBeam_[num].translation_.x; }
	// Z座標の獲得
	float GetZ(int num) { return worldTransformBeam_[num].translation_.z; }
	// 存在フラグの獲得
	int GetFlag(int num) { return shotFlag[num]; }
	// 衝突処理
	void Hit(int num) { shotFlag[num] = 0; }
	//EXビーム残弾の有無
	int GetPower() { return powerNum_; }
};
