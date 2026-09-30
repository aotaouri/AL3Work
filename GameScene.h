#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include <vector>
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "Fade.h"
#include "Phase.h"
#include "FlyingEnemy.h"

using namespace KamataEngine;

class GameScene {

public:


	~GameScene();

	void Initialize();

	void Update();

	void Draw();

    void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	void ChangePhase();

	bool IsFinished() const { return finished_; }

	bool IsClear() const { return isClear_; }

private:

	void GenerateEnemies();

	void GenerateClearItems();

	void GenerateBgObjects(); // 背景オブジェクト生成関数

	// フェード時間定数（1.0秒）
	static inline const float kFadeDuration = 1.0f;

	// ★ 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// ★ フェードオブジェクト
	Fade* fade_ = nullptr;

	// 3Dモデル
	//プレイヤー
	KamataEngine::Model* modelplayer_ = nullptr;

	//クリアアイテム
	KamataEngine::Model* modelClearItem_ = nullptr;

	//敵
	KamataEngine::Model* modelenemy_ = nullptr;

	//飛ぶ敵
	KamataEngine::Model* modelflyingEnemy_ = nullptr;

	//ブロック
	KamataEngine::Model* modelblock_ = nullptr;

	//デスパーティクル
	KamataEngine::Model* modelDeathParticle_ = nullptr;

	// --- 背景用オブジェクト ---
	KamataEngine::Model* modelBgObject_ = nullptr;
	std::vector<KamataEngine::WorldTransform*> worldTransformBgObjects_;

	// カメラ
	KamataEngine::Camera* camera_;

	// プレイヤー
	Player* player_ = nullptr;

	std::list<Enemy*> enemies_;

	std::list<FlyingEnemy*> flyingEnemies_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// 3Dモデル
	KamataEngine::Model* modelSkydome_ = nullptr;

	// 天球
	Skydome* Skydome_ = nullptr;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	CameraController* cameraController_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;

	// 終了フラグ
	bool finished_ = false;

    KamataEngine::WorldTransform* worldTransformClearItem_ = nullptr;
	bool isClearItemSpawned_ = false;
	bool isItemCollected_ = false;

	bool isClear_ = false;

	// ★ 複数管理のために vector に変更
	std::vector<KamataEngine::WorldTransform*> worldTransformClearItems_;
	std::vector<bool> itemCollectedFlags_; // アイテムごとの取得フラグ

	int32_t totalItemCount_ = 0;     // マップ上の総アイテム数
	int32_t collectedItemCount_ = 0; // 取得したアイテムの数

};
