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
};
