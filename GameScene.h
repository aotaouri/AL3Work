#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

class GameScene {

public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

private:
	uint32_t textureHandle_ = 0;

	uint32_t textureHandle_2 = 0;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;

	// プレイヤー
	Player* player_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

};
