#include "Wave.h"
#include "Novice.h"
#include <numbers>
#include <cmath>

Wave::Wave() :
	scalar(1.0f, 1.0f, 1.0f),
	rotate(),
	translate(),
	localPos(0),
	worldMat(),
	num(0.0f),
	numSpd(0.04f)
{}

void Wave::Update(int32_t division) {
	if (localPos.empty() || localPos.size() != static_cast<size_t>(std::pow(static_cast<float>(division - 1),2.0f))) {
		localPos.clear();
		for (int32_t x = 0; x <= division; ++x) {
			for (int32_t z = 0; z <= division; ++z) {
				localPos.push_back(
					Vector3D(
						(static_cast<float>(division) / -2.0f + static_cast<float>(x)) / static_cast<float>(division),
						1.0f, 
						(static_cast<float>(division) / 2.0f - static_cast<float>(z)) / static_cast<float>(division)
					)
				);
			}
		}
	}

	worldMat.Affin(scalar, rotate, translate);
	num += numSpd;
}

void Wave::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Mat4x4 worldVpvpMatrix = worldMat * viewProjectionMatrix * viewPortMatrix;

	std::vector<Vector3D> screenPos(0);

	Mat4x4 wave = MakeMatrixScalar({ 1.0f, 1.0f, 1.0f });
	//float theataSpd = std::numbers::pi_v<float> / std::sqrt(static_cast<float>(localPos.size()));
	//float theata = 0.0f;

	for (auto& i : localPos) {
		wave = MakeMatrixScalar({ 1.0f, std::cos(i.z * 5.0f + num) * 0.125f + std::cos(i.x * 5.0f + num) * 0.125f, 1.0f});
		screenPos.push_back(i * wave *  worldVpvpMatrix);
	}

	for (auto& i : screenPos) {
		Novice::DrawEllipse(static_cast<int>(i.x), static_cast<int>(i.y), 5, 5, 0.0f, color, kFillModeSolid);
	}
}