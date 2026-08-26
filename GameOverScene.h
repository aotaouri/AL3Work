#pragma once
#include "Fade.h"
#include "KamataEngine.h"

class GameOverScene {
public:

	// コンストラクタ / デストラクタ
	GameOverScene() = default;
	~GameOverScene() = default;

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelGameOver_ = nullptr;

	// トランスフォーム (位置・回転・縮小)
	KamataEngine::WorldTransform worldTransformGameOver_;

	KamataEngine::WorldTransform worldTransform_;

};
