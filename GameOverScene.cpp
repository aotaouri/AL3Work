#include "GameOverScene.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

void GameOverScene::Initialize() {
	finished_ = false;

	// カメラの生成と初期化
	camera_ = new Camera();
	camera_->Initialize();

	// 3Dモデルの読み込み (※ Resources/gameOver/ フォルダ内のモデルを指定)
	modelGameOver_ = Model::CreateFromOBJ("GAMEOVER", true);

	// トランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f}; // 画面中央付近に配置
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}

void GameOverScene::Update() {
	Input* input = Input::GetInstance();

	// Zキーでタイトルへ遷移
	if (input->TriggerKey(DIK_Z)) {
		finished_ = true;
	}

	// アフィン変換行列の計算と転送
	Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransform_.rotation_);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransform_.scale_);

	Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
	worldMatrix = Multiply(worldMatrix, translationMatrix);

	worldTransform_.matWorld_ = worldMatrix;
	worldTransform_.TransferMatrix();
}

void GameOverScene::Draw() {
	// 3Dモデル描画前処理
	Model::PreDraw();

	// モデルの描画
	if (modelGameOver_) {
		modelGameOver_->Draw(worldTransform_, *camera_);
	}

	// 3Dモデル描画後処理
	Model::PostDraw();
}