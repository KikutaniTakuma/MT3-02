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

	auto sphere = std::array<std::unique_ptr<Sphere>,2>();
	for (auto& i : sphere) {
		i = std::make_unique<Sphere>();
		assert(i);
	}
	sphere[0]->radius = 0.3f;
	sphere[0]->translation = {0.65f, 0.0f, 1.0f};

	sphere[1]->radius = 0.5f;
	uint32_t sphreColor = WHITE;

	auto grid = std::make_unique<Grid>();
	grid->scalar = { 4.0f,0.0f,4.0f };

	Vector3D start;
	Vector3D end;

	int gridDivision = 10;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();


		// キー入力を受け取る
		std::copy(keys.begin(), keys.end(), preKeys.begin());
		Novice::GetHitKeyStateAll((char*)keys.data());

		///
		/// ↓更新処理ここから
		///
		ImGui::Begin("Window");
		ImGui::DragFloat3("Camera pos", &camera->pos.x, 0.01f);
		ImGui::DragFloat3("Camera rotate", &camera->rotate.x, 0.01f);
		ImGui::DragFloat3("Camera scale", &camera->scale.x, 0.01f);
		ImGui::DragFloat3("Sphere pos[0]", &sphere[0]->translation.x, 0.01f);
		ImGui::DragFloat("Sphere scale[0]", &sphere[0]->radius, 0.01f);
		ImGui::DragFloat3("Sphere pos[1]", &sphere[1]->translation.x, 0.01f);
		ImGui::DragFloat("Sphere scale[1]", &sphere[1]->radius, 0.01f);
		ImGui::End();

		for (auto& i : sphere) {
			i->Update();
		}

		if (sphere[1]->IsCollision((*sphere[0]))) {
			sphreColor = RED;
		}
		else {
			sphreColor = WHITE;
		}

		grid->Update(gridDivision);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Update();

		grid->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), 0xaaaaaaff);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		sphere[0]->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), WHITE);

		sphere[1]->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), sphreColor);



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
