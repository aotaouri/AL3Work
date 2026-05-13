#include "GameScene.h"
#include "KamataEngine.h"

void GameScene::Initialize() {
	textureHandle_ = KamataEngine::TextureManager::Load("uvChecker.png");

	model_ = KamataEngine::Model::Create();

	camera_ = new KamataEngine::Camera();

	camera_->Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_,camera_);

}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
}

void GameScene::Draw() {
	// 自キャラの描画
	player_->Draw();
}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete camera_;
}