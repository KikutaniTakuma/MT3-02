#pragma once
#include <vector>
#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"

class Grid {
public:
	Grid();
	~Grid() {}

public:
	void Update(int32_t division);
	void Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color);

public:
	Vector3D scalar;
	Vector3D rotate;
	Vector3D translate;

private:
	std::vector<float> localPosX;
	std::vector<float> localPosZ;

	Mat4x4 worldMat;
};

struct Plane {
	Vector3D normal;
	float distance;
};

bool IsCollision(const Plane& plane, const Segment& segment);

void DrawPlane(const Plane& plane, const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color);

void IsCollisionTriangle(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Segment& segment);