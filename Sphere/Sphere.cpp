#include "Sphere/Sphere.h"
#include <Novice.h>
#include <numbers>
#include <cassert>

Sphere::Sphere() :
	radius(1.0f),
	loaclVec(radius,0.0f,0.0f),
	scale(1.0f,1.0f,1.0f),
	rotate(0.0f,0.0f,0.0f),
	translation(0.0f,0.0f,10.0f),
	worldMat(),
	kDivision(10),
	spherePosList(std::make_unique<std::vector<std::vector<Vector3D>>>(0))
{
	assert(spherePosList);

	for (int y = 0; y <= kDivision; y++) {
		rotate.z = -std::numbers::pi_v<float> / 2.0f;
		spherePosList->push_back(std::vector<Vector3D>(0));
		for (int z = 0; z <= kDivision; z++) {
			worldMat = MakeMatrixRotateZ(rotate.z) * MakeMatrixRotateY(rotate.y) * MakeMatrixAffin(scale, worldRoate, translation);
			spherePosList->rbegin()->push_back(loaclVec * worldMat);
			rotate.z += (std::numbers::pi_v<float> / static_cast<float>(kDivision));
		}

		rotate.y += (std::numbers::pi_v<float> / static_cast<float>(kDivision)) * 2.0f;
	}
	
}

Sphere::~Sphere() {
	spherePosList.reset();
}


void Sphere::Update() {
	spherePosList->clear();
	rotate = Vector3D();

	loaclVec.x = radius;

	for (int y = 0; y <= kDivision; y++) {
		rotate.z = -std::numbers::pi_v<float> / 2.0f;
		spherePosList->push_back(std::vector<Vector3D>(0));
		for (int z = 0; z <= kDivision; z++) {
			worldMat = MakeMatrixRotateZ(rotate.z) * MakeMatrixRotateY(rotate.y) * MakeMatrixAffin(scale, worldRoate, translation);
			spherePosList->rbegin()->push_back(loaclVec * worldMat);
			rotate.z += (std::numbers::pi_v<float> / static_cast<float>(kDivision));
		}

		rotate.y += (std::numbers::pi_v<float> / static_cast<float>(kDivision)) * 2.0f;
	}
}

void Sphere::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix, uint32_t color) {
	auto vpvpMat = viewProjectionMatrix * viewPortmatrix;

	
	std::vector<Vector3D> preHarfCircle;
	std::vector<Vector3D>::iterator preItr;
	for (auto i = spherePosList->begin(); i != spherePosList->end(); ++i) {
		if (i != spherePosList->begin()) {
			preItr = preHarfCircle.begin();
		}

		for (auto j = i->begin(); j != i->end();) {
			Vector3D startPos = *j * vpvpMat;
			if (++j != i->rbegin().base()) {
				Vector3D endPos = *(j)*vpvpMat;
				Novice::DrawLine(static_cast<int>(startPos.x), static_cast<int>(startPos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y), color);

				if (i != spherePosList->begin()) {
					preItr++;
					Vector3D prePos = (*preItr) * vpvpMat;
					Novice::DrawLine(static_cast<int>(prePos.x), static_cast<int>(prePos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y), color);
				}
			}
		}
		preHarfCircle = (*i);
	}
}