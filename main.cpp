#include <Novice.h>
#include <array>
#include <string>
#include <memory>
#include <cassert>
#include <imgui.h>
#include <numbers>
#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"
#include "Camera/Camera.h"
#include "Sphere/Sphere.h"
#include "Grid/Grid.h"
#include "Mouse/Mouse.h"
#include "Wave.h"

const std::string kWindowTitle = "LE2A_04_キクタニ_タクマ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle.c_str(), 1280, 720);

	// キー入力結果を受け取る箱
	std::array<int8_t, 256> keys = { 0 };
	std::array<int8_t, 256> preKeys = { 0 };

	auto camera = std::make_unique<Camera>();
	assert(camera);

	camera->pos = { 0.0f,1.9f, -6.49f };
	camera->rotate = { 0.26f,0.0f,0.0f };
	camera->scale = { 1.0f,1.0f,1.0f };

	auto wave = std::make_unique<Wave>();
	wave->scalar = { 3.0f,1.0f,3.0f };

	int gridDivision = 10;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();


		// キー入力を受け取る
		std::copy(keys.begin(), keys.end(), preKeys.begin());
		Novice::GetHitKeyStateAll((char*)keys.data());
		Mouse::Input();

		///
		/// ↓更新処理ここから
		///
		ImGui::Begin("Window");
		ImGui::DragFloat3("Camera pos", &camera->pos.x, 0.01f);
		ImGui::DragFloat3("Camera rotate", &camera->rotate.x, 0.01f);
		ImGui::DragFloat3("Camera scale", &camera->scale.x, 0.01f);
		ImGui::End();
		wave->Update(gridDivision);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Update();

		wave->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), 0xaaaaaaff);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
