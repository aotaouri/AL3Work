#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include <vector>
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

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

private:

	// 3Dモデル
	//プレイヤー
	KamataEngine::Model* modelplayer_ = nullptr;

	//敵
	KamataEngine::Model* modelenemy_ = nullptr;

	//ブロック
	KamataEngine::Model* modelblock_ = nullptr;

	//デスパーティクル
	KamataEngine::Model* modelDeathParticle_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	// プレイヤー
	Player* player_ = nullptr;

	std::list<Enemy*> enemies_;

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

};
