#include "GameScene.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
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

	// 背景モデルの読み込み (例: "bg_tree" や "bg_mountain" など)
	modelBgObject_ = KamataEngine::Model::CreateFromOBJ("enemy", true);

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

	//クリアアイテムのモデル
	modelClearItem_ = KamataEngine::Model::CreateFromOBJ("enemy", true); // モデル名に合わせて変更
	
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);

	for (int32_t i = 0; i < 1; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(28 + i * 2, 7);
		newEnemy->Initialize(modelenemy_, camera_, enemyPosition);

		newEnemy->SetMapChipField(mapChipField_);

		enemies_.push_back(newEnemy);
	}

	// カメラコントローラの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	Rect area = {14.5f, 84.5f, 8.0f, 20.0f};
	cameraController_->SetMovableArea(area);
	cameraController_->Reset();

	// 天球の生成
	Skydome_ = new Skydome();
	// 天球の初期化
	Skydome_->Initialize(modelSkydome_, camera_);

	// 背景オブジェクトの生成
	GenerateBgObjects();
	GenerateBlocks();
	GenerateEnemies();
	GenerateClearItems();


	// デバッグカメラの生成
	debugCamera_ = new KamataEngine::DebugCamera(1280, 720);
	isDebugCameraActive_ = false;

	fade_ = new Fade();
	fade_->Initialize();

	phase_ = Phase::kFadeIn;
	fade_->Start(Status::FadeIn, kFadeDuration);
}

void GameScene::GenerateBgObjects() {
	// 手動で好きな位置に配置する場合（Z軸を奥にずらすのがポイント！）
	// 例：プレイヤーやゲーム面(Z: 0.0f)より奥の Z: 5.0f や 10.0f に配置
	std::vector<Vector3> bgPositions = {
	    {5.0f,  2.0f, 10.0f},
	    {15.0f, 3.0f, 10.0f},
	    {30.0f, 2.0f, 12.0f},
	    {50.0f,  2.0f, 10.0f},
	    {75.0f, 3.0f, 10.0f},
	    {100.0f, 2.0f, 12.0f},
	};

	for (const Vector3& pos : bgPositions) {
		KamataEngine::WorldTransform* transform = new KamataEngine::WorldTransform();
		transform->Initialize();
		transform->translation_ = pos;

		// 必要に応じてサイズ変更や回転も設定可能
		// transform->scale_ = { 2.0f, 2.0f, 2.0f };

		worldTransformBgObjects_.push_back(transform);
	}
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

	// ① 通常の敵（Enemy）との当たり判定
	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	} // ★ ここで Enemy のループを閉じる

	// ② 上下する敵（FlyingEnemy）との当たり判定（変数名を flyingEnemy に変更）
	for (FlyingEnemy* flyingEnemy : flyingEnemies_) {
		aabb2 = flyingEnemy->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(flyingEnemy);
			flyingEnemy->OnCollision(player_);
		}
	}

	// クリアアイテムの当たり判定
	AABB playerAABB = player_->GetAABB();

	for (size_t i = 0; i < worldTransformClearItems_.size(); ++i) {
		// まだ取得していないアイテムのみ判定
		if (!itemCollectedFlags_[i] && worldTransformClearItems_[i]) {
			Vector3 itemPos = worldTransformClearItems_[i]->translation_;

			AABB itemAABB;
			itemAABB.min = {itemPos.x - 0.5f, itemPos.y - 0.5f, itemPos.z - 0.5f};
			itemAABB.max = {itemPos.x + 0.5f, itemPos.y + 0.5f, itemPos.z + 0.5f};

			if (IsCollision(playerAABB, itemAABB)) {
				itemCollectedFlags_[i] = true; // 該当アイテムを取得済みにする
				collectedItemCount_++;         // 取得数を加算
			}
		}
	}

}

void GameScene::Update() {

	// ★ フェードの更新
	fade_->Update();

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
	case Phase::kPlay:

		Skydome_->Update();


		// Update() 内の「Skydome_->Update();」の下あたりに追加
		for (KamataEngine::WorldTransform* bgTransform : worldTransformBgObjects_) {
			if (!bgTransform)
				continue;

			KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(bgTransform->translation_);
			KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(bgTransform->rotation_);
			KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(bgTransform->scale_);

			KamataEngine::Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
			worldMatrix = Multiply(worldMatrix, translationMatrix);

			bgTransform->matWorld_ = worldMatrix;
			bgTransform->TransferMatrix();
		}


		// 自キャラの更新
		player_->Update();

		// 敵
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (FlyingEnemy* enemy : flyingEnemies_) {
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

		if (isClearItemSpawned_ && !isItemCollected_) {
			KamataEngine::Matrix4x4 translationMatrix = MakeTranslateMatrix(worldTransformClearItem_->translation_);
			KamataEngine::Matrix4x4 rotationMatrix = MakeRotationMatrix(worldTransformClearItem_->rotation_);
			KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(worldTransformClearItem_->scale_);

			KamataEngine::Matrix4x4 worldMatrix = Multiply(scaleMatrix, rotationMatrix);
			worldMatrix = Multiply(worldMatrix, translationMatrix);

			worldTransformClearItem_->matWorld_ = worldMatrix;
			worldTransformClearItem_->TransferMatrix();
		}

		// Update() 内のブロック更新などの下に追加
		for (size_t i = 0; i < worldTransformClearItems_.size(); ++i) {
			if (!itemCollectedFlags_[i] && worldTransformClearItems_[i]) {
				KamataEngine::Matrix4x4 itemTranslation = MakeTranslateMatrix(worldTransformClearItems_[i]->translation_);
				KamataEngine::Matrix4x4 itemRotation = MakeRotationMatrix(worldTransformClearItems_[i]->rotation_);
				KamataEngine::Matrix4x4 itemScale = MakeScaleMatrix(worldTransformClearItems_[i]->scale_);

				KamataEngine::Matrix4x4 itemWorldMatrix = Multiply(itemScale, itemRotation);
				itemWorldMatrix = Multiply(itemWorldMatrix, itemTranslation);

				worldTransformClearItems_[i]->matWorld_ = itemWorldMatrix;
				worldTransformClearItems_[i]->TransferMatrix();
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

void GameScene::GenerateEnemies() {
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			// 2: 歩く敵
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kEnemy) {
				Enemy* newEnemy = new Enemy();
				Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				newEnemy->Initialize(modelenemy_, camera_, enemyPosition);
				newEnemy->SetMapChipField(mapChipField_);
				enemies_.push_back(newEnemy);
			}
			// 3: 上下する敵（★追加）
			else if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kFlyingEnemy) {
				FlyingEnemy* newEnemy = new FlyingEnemy();
				Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				newEnemy->Initialize(modelenemy_, camera_, enemyPosition); // 同じモデルを使用
				newEnemy->SetMapChipField(mapChipField_);
				flyingEnemies_.push_back(newEnemy);
			}
		}
	}
}

void GameScene::GenerateClearItems() {
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			// CSVの値が 4 (kClearItem) の場合
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kClearItem) {
				KamataEngine::WorldTransform* itemTransform = new KamataEngine::WorldTransform();
				itemTransform->Initialize();
				itemTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

				worldTransformClearItems_.push_back(itemTransform);
				itemCollectedFlags_.push_back(false); // 未取得状態
				totalItemCount_++;                    // 総数をカウント
			}
		}
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kFadeIn:
		// ★ フェードイン完了でゲームプレイ開始
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		break;
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
		} else if (collectedItemCount_ >= 3) { // ※ CSVに置いた全アイテムなら totalItemCount_ に変更
			phase_ = Phase::kClear;
			isClear_ = true;
			fade_->Start(Status::FadeOut, kFadeDuration);
		}

		break;
	case Phase::kDeath:

		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut; // ★ フェーズを切り替える
			fade_->Start(Status::FadeOut, kFadeDuration);
		}

		break;
	case Phase::kClear: // ★ クリア演出待ち
		if (fade_->IsFinished()) {
			finished_ = true; // フェードが終わったら GameScene 終了
		}
		break;

	case Phase::kFadeOut:
		// ★ フェードアウト完了でシーン終了
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void GameScene::Draw() {

	Skydome_->Draw();

	KamataEngine::Model::PreDraw();

	// 2. ★ 背景オブジェクトを真っ先に描画（奥にあるため）
	if (modelBgObject_) {
		for (KamataEngine::WorldTransform* bgTransform : worldTransformBgObjects_) {
			modelBgObject_->Draw(*bgTransform, *camera_);
		}
	}

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

	if (modelClearItem_) {
		for (size_t i = 0; i < worldTransformClearItems_.size(); ++i) {
			if (!itemCollectedFlags_[i] && worldTransformClearItems_[i]) {
				modelClearItem_->Draw(*worldTransformClearItems_[i], *camera_);
			}
		}
	}

	KamataEngine::Model::PostDraw();

	// 自キャラの描画
	player_->Draw();

	// 敵全体の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// Draw() 内
	for (FlyingEnemy* enemy : flyingEnemies_) {
		enemy->Draw();
	}


	fade_->Draw();
}

GameScene::~GameScene() {

	delete fade_;
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
	delete modelClearItem_;
	delete worldTransformClearItem_;
	delete modelBgObject_;

	// 生成した WorldTransform をすべて解放
	for (auto* transform : worldTransformClearItems_) {
		delete transform;
	}
	worldTransformClearItems_.clear();

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	for (FlyingEnemy* enemy : flyingEnemies_) {
		delete enemy;
	}
	flyingEnemies_.clear();

	for (auto* transform : worldTransformBgObjects_) {
		delete transform;
	}
	worldTransformBgObjects_.clear();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (KamataEngine::WorldTransform* worldTransformBlocks : worldTransformBlockLine) {
			delete worldTransformBlocks;
		}
	}
	worldTransformBlocks_.clear();
}