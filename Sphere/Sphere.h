#pragma once

#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"
#include <vector>
#include <memory>

class Sphere {
public:
	Sphere();
	~Sphere();


public:
	void Update();
	void Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix, uint32_t color);

public:
	float radius;
	Vector3D translation;
	Vector3D worldRoate;

private:
	Vector3D loaclVec;

	Vector3D scale;
	Vector3D rotate;

	Mat4x4 worldMat;

	const int kDivision;

	std::unique_ptr<std::vector<std::vector<Vector3D>>> spherePosList;

};