#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "GameClearScene.h"
#include "GameOverScene.h"
#include "Hoshi_Yokeyouya3D.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "Hoshi_Yokeyouya.h"
#include <Hoshi_Yokeyouya2.h>

TitleScene* titleScene = nullptr;
SelectScene* selectScene = nullptr;
Hoshi_Yokeyouya3D* hoshi_Yokeyouya3D = nullptr;
GameClearScene* gameClearScene = nullptr;
GameOverScene* gameOverScene = nullptr;
Hoshi_Yokeyouya* hoshi_Yokeyouya = nullptr;
Hoshi_Yokeyouya2* hoshi_Yokeyouya2 = nullptr;
Model* model_ = nullptr;
ViewProjection* viewProjection_ = nullptr;

enum class Scene {
	kTitle,
	kStageSelect,
	kGameScene,
	kGameOver,
	kGameClear,
};

Scene scene = Scene::kTitle;

enum class GameScene {
	kHoshi_Yokeyouya,
	kHoshi_Yokeyouya2,
	kHoshi_Yokeyouya3D,
};

GameScene gameScene = GameScene::kHoshi_Yokeyouya;

void ChangeScene();
void UpdateScene();
void DrawScene();
void UpdateGameScene();
void DrawGameScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"2363_星、避けようや3D");//

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	scene = Scene::kTitle;
	// タイトルシーンの初期化
	titleScene = new TitleScene;
	titleScene->Initialize();

	
	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}
		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーン切り替え
		ChangeScene();
		// 現在シーン更新
		UpdateScene();
		// ゲームシーンの毎フレーム処理
		//gameScene->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// 現在シーンの描画
		DrawScene();
		// ゲームシーンの描画
		//gameScene->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete selectScene;
	delete hoshi_Yokeyouya3D;
	delete gameClearScene;
	delete gameOverScene;
	delete hoshi_Yokeyouya;
	delete hoshi_Yokeyouya2;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kStageSelect;
			delete titleScene;
			titleScene = nullptr;
			selectScene = new SelectScene;
			selectScene->Initialize();
		}
		break;

	case Scene::kStageSelect:
		if (selectScene) {
			if (selectScene->IsOne()) {
				gameScene = GameScene::kHoshi_Yokeyouya;
				scene = Scene::kGameScene; // Gameplayに移行
				delete selectScene;
				selectScene = nullptr;
				hoshi_Yokeyouya = new Hoshi_Yokeyouya;
				hoshi_Yokeyouya->Initialize();
			} else if (selectScene->IsTwo()) {
				gameScene = GameScene::kHoshi_Yokeyouya2;
				scene = Scene::kGameScene;
				delete selectScene;
				selectScene = nullptr;
				hoshi_Yokeyouya2 = new Hoshi_Yokeyouya2;
				hoshi_Yokeyouya2->Initialize();
			} else if (selectScene->IsThree()) {
				gameScene = GameScene::kHoshi_Yokeyouya3D;
				scene = Scene::kGameScene;
				delete selectScene;
				selectScene = nullptr;
				hoshi_Yokeyouya3D = new Hoshi_Yokeyouya3D;
				hoshi_Yokeyouya3D->Initialize();
			}
		}
		break;

	case Scene::kGameScene:
		switch (gameScene) {
		case GameScene::kHoshi_Yokeyouya:
			if (hoshi_Yokeyouya->IsFinished()) {
				if (hoshi_Yokeyouya->IsHealth()){
					scene = Scene::kGameOver;
					// 新シーンの生成と初期化
					gameOverScene = new GameOverScene;
					gameOverScene->Initialize();
				}
				else
				{
					scene = Scene::kGameClear;
					gameClearScene = new GameClearScene;
					gameClearScene->Initialize();
				}
				delete hoshi_Yokeyouya;
				hoshi_Yokeyouya = nullptr;
			}
			break;

		case GameScene::kHoshi_Yokeyouya2:
			if (hoshi_Yokeyouya2->IsFinished()) {
				if (hoshi_Yokeyouya2->IsHealth()) {
					scene = Scene::kGameOver;
					// 新シーンの生成と初期化
					gameOverScene = new GameOverScene;
					gameOverScene->Initialize();
				}
				else
				{
					scene = Scene::kGameClear;
					gameClearScene = new GameClearScene;
					gameClearScene->Initialize();
				}
				delete hoshi_Yokeyouya2;
				hoshi_Yokeyouya2 = nullptr;
			}
			break;

		case GameScene::kHoshi_Yokeyouya3D:

			if (hoshi_Yokeyouya3D->IsFinished()) {
				if (hoshi_Yokeyouya3D->IsHealth()) {
					scene = Scene::kGameOver;
					// 新シーンの生成と初期化
					gameOverScene = new GameOverScene;
					gameOverScene->Initialize();
				}
				else 
				{
					scene = Scene::kGameClear;
					gameClearScene = new GameClearScene;
					gameClearScene->Initialize();
				}
				delete hoshi_Yokeyouya3D;
				hoshi_Yokeyouya3D = nullptr;
			}
			break;
		}
		break;

	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	case Scene::kGameClear:
		if (gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
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
	case Scene::kStageSelect:
		selectScene->Update();
		break;
	case Scene::kGameScene:
		UpdateGameScene();
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
		titleScene->Draw(); // 描画処理を呼び出す
		break;
	case Scene::kStageSelect:
		selectScene->Draw();
		break;
	case Scene::kGameScene:
		DrawGameScene();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kGameClear:
		gameClearScene->Draw();
		break;
	}
}

void UpdateGameScene() {
	switch (gameScene) {
	case GameScene::kHoshi_Yokeyouya:
		hoshi_Yokeyouya->Update();
		break;
	case GameScene::kHoshi_Yokeyouya2:
		hoshi_Yokeyouya2->Update();
		break;
	case GameScene::kHoshi_Yokeyouya3D:
		hoshi_Yokeyouya3D->Update();
		break;
	}
}

void DrawGameScene() {
	switch (gameScene) {
	case GameScene::kHoshi_Yokeyouya:
		hoshi_Yokeyouya->Draw();
		break;
	case GameScene::kHoshi_Yokeyouya2:
		hoshi_Yokeyouya2->Draw();
		break;
	case GameScene::kHoshi_Yokeyouya3D:
		hoshi_Yokeyouya3D->Draw();
		break;
	}
}