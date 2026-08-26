#pragma once
#include "KamataEngine.h"

class GameClearScene {
public:
	GameClearScene() = default;
	~GameClearScene() = default;

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelClear_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

};