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
	std::array<char, 256> keys = { 0 };
	std::array<char, 256> preKeys = { 0 };

	auto camera = std::make_unique<Camera>();
	assert(camera);

	camera->pos = { 0.0f,1.9f, -6.49f };
	camera->rotate = { 0.26f,0.0f,0.0f };
	camera->scale = { 1.0f,1.0f,1.0f };

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f, 2.0f,2.0f} };
	Vector3D point{ -1.5f,0.6f,0.6f };

	Vector3D project = Project(point - segment.origin, segment.diff);
	Vector3D closestPoint = ClosestPoint(point, segment);

	auto pointSphere = std::make_unique<Sphere>();
	pointSphere->radius = 0.01f;
	pointSphere->translation = point;

	auto closestPointSphere = std::make_unique<Sphere>();
	closestPointSphere->radius = 0.01f;
	closestPointSphere->translation = closestPoint;

	auto grid = std::make_unique<Grid>();
	grid->scalar = { 4.0f,0.0f,4.0f };

	Vector3D start;
	Vector3D end;

	int gridDivision = 20;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();


		// キー入力を受け取る
		std::copy(keys.begin(), keys.end(), preKeys.begin());
		Novice::GetHitKeyStateAll(keys.data());

		///
		/// ↓更新処理ここから
		///
		ImGui::Begin("Window");
		ImGui::DragFloat3("Point", &point.x, 0.01f);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment doff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("Project", &project.x, 0.01f);
		ImGui::End();

		pointSphere->Update();
		closestPointSphere->Update();

		grid->Update(gridDivision);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Update();

		grid->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), 0xaaaaaaff);

		start = segment.origin * camera->getViewProjectionMatrix() * camera->getViewPortMatrix();
		end = (segment.origin + segment.diff) * camera->getViewProjectionMatrix() * camera->getViewPortMatrix();

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);


		pointSphere->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), RED);
		closestPointSphere->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), 0xff);



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
