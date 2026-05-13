#pragma once
#include "KamataEngine.h"
#include "Player.h"

class GameScene {
	
	public:

	~GameScene();

	void Initialize();

	void Update();

	void Draw();

	private:

	uint32_t textureHandle_ = 0;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	//カメラ
	KamataEngine::Camera* camera_;

	//プレイヤー
	Player* player_ = nullptr;

};
