#include "GameOverScene.h"
#include "GameClearScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

// シーン（型）
enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
	kGameOver,
	kGameClear,
};

// 現在シーン（型）
Scene scene = Scene::kUnknown;

// --- グローバル変数 ---
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameOverScene* gameOverScene = nullptr;
GameClearScene* gameClearScene = nullptr;

void ChangeScene();

void UpdateScene();

void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE2D_01_アオタ_オウリ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 2. タイトルシーンのインスタンス変数宣言
	scene = Scene::kTitle;

	// 3. タイトルシーンの生成と初期化[cite: 6]
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ImGuiManagerのインスタンス取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		ChangeScene();

		// ImGui受付開始
		imguiManager->Begin();

		// 4. タイトルシーンの更新処理
		UpdateScene();

		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// 5. タイトルシーンの描画処理
		DrawScene();

		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// ImGui描画
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;
	delete gameOverScene;
	delete gameClearScene;
	titleScene = nullptr;

	// エンジンの終了処理
	Finalize();

	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーン解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (gameScene && gameScene->IsFinished()) {
			// ★ クリアしたか死亡したかで分岐
			if (gameScene->IsClear()) {
				scene = Scene::kGameClear; // ClearSceneへ遷移
				delete gameScene;
				gameScene = nullptr;

				gameClearScene = new GameClearScene();
				gameClearScene->Initialize();
			} else {
				scene = Scene::kGameOver; // 死亡時はGameOverへ
				delete gameScene;
				gameScene = nullptr;

				gameOverScene = new GameOverScene();
				gameOverScene->Initialize();
			}
		}
		break;

	case Scene::kGameOver:
		if (gameOverScene && gameOverScene->IsFinished()) {
			// シーン変更: GameOver -> Title
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initialize();
		}

		break;

		case Scene::kGameClear: // ★追加
		if (gameClearScene && gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
			gameOverScene->Update();
		break;
	case Scene::kGameClear:
			gameClearScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
			gameOverScene->Draw();
		break;
	case Scene::kGameClear:
			gameClearScene->Draw();
		break;
	}
}