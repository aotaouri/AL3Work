#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//エンジンの初期化
	Initialize(L"LE2D_01_アオタ_オウリ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//ゲームシーンのインスタンス生成
	GameScene* gameScene = new GameScene();

	//ゲームシーンの初期化
	gameScene->Initialize();

	//メインループ
	while (true) {
		//エンジンの更新
		if (Update()) {
			break;
		}

		//ゲームシーンの更新処理
		gameScene->Update();

		//描画開始
		dxCommon->PreDraw();

		//ゲームシーンの描画
		gameScene->Draw();

		//描画終了
		dxCommon->PostDraw();


	}

	delete gameScene;
	gameScene = nullptr;

	//エンジンの終了処理
	Finalize();

	return 0;
}
