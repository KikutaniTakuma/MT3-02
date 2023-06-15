#include "Wave.h"
#include "Novice.h"
#include <numbers>
#include <cmath>
#include <random>
#include <float.h>

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
	if (num < FLT_MAX && num >= 0.0f) {
		num += numSpd;
	}
	else {
		num = 0.0f;
	}
}

void Wave::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Mat4x4 worldVpvpMatrix = worldMat * viewProjectionMatrix * viewPortMatrix;

	std::vector<Vector3D> screenPos(0);

	Mat4x4 wave;

	for (auto& i : localPos) {
		wave = MakeMatrixScalar({ 1.0f, 1.0f / i.y, 1.0f }) *
			MakeMatrixScalar({ 1.0f, (std::cos(i.z * 3.0f + num) + std::cos(i.x * 3.0f + num) + std::cos((i.x + i.x) * 5.0f + num) + std::cos((i.z + i.z) * 5.0f + num)) * 0.125f, 1.0f });
		float y = i.y;
		Vector3D tmp = i * wave;
		tmp.y += y - 1.0f;
		tmp *= worldVpvpMatrix;
		screenPos.push_back(tmp);
	}

	for (auto& i : screenPos) {
		Novice::DrawEllipse(static_cast<int>(i.x), static_cast<int>(i.y), 5, 5, 0.0f, color, kFillModeSolid);
	}
}