#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include <vector>

using namespace KamataEngine;

class GameScene {

public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

    void GenerateBlocks();

private:

	// 3Dモデル
	//プレイヤー
	KamataEngine::Model* modelplayer_ = nullptr;

	//ブロック
	KamataEngine::Model* modelblock_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	// プレイヤー
	Player* player_ = nullptr;

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

};
