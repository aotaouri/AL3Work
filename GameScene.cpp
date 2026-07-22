#include "GameScene.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Matrix4x4.h"
#include "MyMath.h"
#include "Phase.h"
#include "Player.h"
#include "TitleScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	modelblock_ = KamataEngine::Model::CreateFromOBJ("block", true);
	// 3Dモデルの生成(天球)
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);
	modelplayer_ = KamataEngine::Model::CreateFromOBJ("player", true);
	modelenemy_ = KamataEngine::Model::CreateFromOBJ("enemy", true);

	// カメラ
	camera_ = new KamataEngine::Camera();
	camera_->farZ = 1000.0f;
	camera_->Initialize();

	// マップチップ
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 自キャラの生成
	player_ = new Player();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	// 自キャラの初期化
	player_->Initialize(modelplayer_, camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10 + i * 2, 18);
		newEnemy->Initialize(modelenemy_, camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// カメラコントローラの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	Rect area = {14.5f, 84.5f, 8.0f, 20.0f};
	cameraController_->SetMovableArea(area);
	cameraController_->Reset();

	// 天球の生成」
	Skydome_ = new Skydome();
	// 天球の初期化
	Skydome_->Initialize(modelSkydome_, camera_);

	GenerateBlocks();

	// デバッグカメラの生成
	debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
	isDebugCameraActive_ = false;

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 配列を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				KamataEngine::WorldTransform* worldTransform = new KamataEngine::WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {

			// 自キャラの衝突時関数を呼び出す
			player_->OnCollision(enemy);
			// 敵の衝突時関数を呼び出す
			enemy->OnCollision(player_);
		}
	}
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) {
	case Phase::kPlay:

		Skydome_->Update();

		// 自キャラの更新
		player_->Update();

		// 敵
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (!isDebugCameraActive_ && cameraController_) {
			cameraController_->Update();
		}

			// デバッグカメラの更新
		debugCamera_->Update();

		for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

			// ブロックの更新
			for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock) {
					continue;
				}

				KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransformBlock->translation_);
				KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransformBlock->rotation_);
				KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformBlock->scale_);

				// アフィン変換
				KamataEngine::Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
				worldMatrix = Multiply(worldMatrix, translationMatrix);

				// 定数バッファに転送する
				worldTransformBlock->matWorld_ = worldMatrix;
				worldTransformBlock->TransferMatrix();
			}
		}


		// 全ての当たり判定を行う
		CheckAllCollisions();

		break;
	case Phase::kDeath:

		Skydome_->Update();

		// 敵
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		
	if (deathParticles_ != nullptr) {
			// デスパーティクル
			deathParticles_->Update();
		}

	// デバッグカメラの更新
		debugCamera_->Update();

		for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

			// ブロックの更新
			for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock) {
					continue;
				}

				KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransformBlock->translation_);
				KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransformBlock->rotation_);
				KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformBlock->scale_);

				// アフィン変換
				KamataEngine::Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
				worldMatrix = Multiply(worldMatrix, translationMatrix);

				// 定数バッファに転送する
				worldTransformBlock->matWorld_ = worldMatrix;
				worldTransformBlock->TransferMatrix();
			}
		}

		break;
	}

	


#ifdef _DEBUG

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_E)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif

	// カメラの処理
	if (isDebugCameraActive_) {

		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェンクション行列の転送
		camera_->TransferMatrix();
	} else {
		// ビュープロジェンクション行列の更新と転送
	}

}

void GameScene::ChangePhase()
{
	switch (phase_) 
	{
	case Phase::kPlay:

		// 自キャラがデス状態
		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// 自キャラの座標にデスパーティクルを発生、初期化
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeathParticle_, camera_, deathParticlesPosition);
		}

		break;
	case Phase::kDeath:

		if (deathParticles_ && deathParticles_->IsFinished())
		{
			finished_ = true;
		}

		break;
	
	}
}

void GameScene::Draw() {

	Skydome_->Draw();

	// 自キャラの描画
	player_->Draw();

	KamataEngine::Model::PreDraw();

	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
	}

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		// ブロックの描画
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (worldTransformBlock == nullptr) {
				continue;
			}

			modelblock_->Draw(*worldTransformBlock, *camera_);
		}
	}
	KamataEngine::Model::PostDraw();

	// 敵全体の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
}

GameScene::~GameScene() {

	delete player_;
	delete cameraController_;
	delete Skydome_;
	delete camera_;
	delete debugCamera_;
	delete mapChipField_;
	delete modelplayer_;
	delete modelSkydome_;
	delete modelblock_;
	delete deathParticles_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (KamataEngine::WorldTransform* worldTransformBlocks : worldTransformBlockLine) {
			delete worldTransformBlocks;
		}
	}
	worldTransformBlocks_.clear();
}