#include "Grid.h"
#include "Novice.h"

Grid::Grid():
	scalar(1.0f, 1.0f, 1.0f),
	rotate(),
	translate(),
	localPosX(0),
	localPosZ(0),
	worldMat()
{}

void Grid::Update(int32_t division) {
	if (localPosX.empty()) {
		for (int32_t x = 0; x <= division; ++x) {
			localPosX.push_back(static_cast<float>(division) / -2.0f + static_cast<float>(x));
		}
	}
	if (localPosZ.empty()) {
		for (int32_t z = 0; z<=division; z++) {
			localPosZ.push_back(static_cast<float>(division) / 2.0f - static_cast<float>(z));
		}
	}

	worldMat.Affin(scalar, rotate, translate);
}

void Grid::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Mat4x4 vpvpMatrix = viewProjectionMatrix * viewPortMatrix;

	std::vector<Vector3D> screenStartPosZ(0);
	std::vector<Vector3D> screenEndPosZ(0);

	for (const auto& z : localPosZ) {
		screenStartPosZ.push_back(Vector3D(*localPosX.begin(), 0.0f, z) * worldMat * vpvpMatrix);
		screenEndPosZ.push_back(Vector3D(*localPosX.rbegin(), 0.0f, z) * worldMat * vpvpMatrix);
	}

	std::vector<Vector3D> screenStartPosX(0);
	std::vector<Vector3D> screenEndPosX(0);

	for (const auto& x : localPosX) {
		screenStartPosX.push_back(Vector3D(x, 0.0f, *localPosZ.begin()) * worldMat * vpvpMatrix);
		screenEndPosX.push_back(Vector3D(x, 0.0f, *localPosZ.rbegin()) * worldMat * vpvpMatrix);
	}

	for (size_t i = 0; i < screenStartPosZ.size(); ++i) {
		if (i == screenStartPosZ.size() / 2) {
			Novice::DrawLine(static_cast<int>(screenStartPosZ[i].x), static_cast<int>(screenStartPosZ[i].y), static_cast<int>(screenEndPosZ[i].x), static_cast<int>(screenEndPosZ[i].y), 0xff);
		}
		else {
			Novice::DrawLine(static_cast<int>(screenStartPosZ[i].x), static_cast<int>(screenStartPosZ[i].y), static_cast<int>(screenEndPosZ[i].x), static_cast<int>(screenEndPosZ[i].y), color);
		}
	}

	for (size_t i = 0; i < screenStartPosX.size(); ++i) {
		if (i == screenStartPosZ.size() / 2) {
			Novice::DrawLine(static_cast<int>(screenStartPosX[i].x), static_cast<int>(screenStartPosX[i].y), static_cast<int>(screenEndPosX[i].x), static_cast<int>(screenEndPosX[i].y), 0xff);
		}
		else {
			Novice::DrawLine(static_cast<int>(screenStartPosX[i].x), static_cast<int>(screenStartPosX[i].y), static_cast<int>(screenEndPosX[i].x), static_cast<int>(screenEndPosX[i].y), color);
		}
	}
}