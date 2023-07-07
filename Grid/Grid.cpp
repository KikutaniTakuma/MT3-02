#include "Grid.h"
#include "Novice.h"
#include <cmath>

Grid::Grid():
	scalar(1.0f, 1.0f, 1.0f),
	rotate(),
	translate(),
	localPosX(0),
	localPosZ(0),
	worldMat()
{}

void Grid::Update(int32_t division) {
	if (localPosX.empty() || localPosX.size() != static_cast<size_t>(division - 1)) {
		localPosX.clear();
		for (int32_t x = 0; x <= division; ++x) {
			localPosX.push_back((static_cast<float>(division) / -2.0f + static_cast<float>(x)) / static_cast<float>(division));
		}
	}
	if (localPosZ.empty() || localPosX.size() != static_cast<size_t>(division - 1)) {
		localPosZ.clear();
		for (int32_t z = 0; z<=division; z++) {
			localPosZ.push_back((static_cast<float>(division) / 2.0f - static_cast<float>(z)) / static_cast<float>(division));
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

Vector3D Perpendicular(const Vector3D& vec) {
	if (vec.x != 0.0f || vec.y != 0.0f) {
		return Vector3D(-vec.y, vec.x, 0.0f);
	}
	return Vector3D(0.0f, -vec.z, vec.y);
}

void DrawPlane(const Plane& plane, const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Vector3D center = plane.normal * plane.distance;
	Vector3D perpendiculars[4];

	perpendiculars[0] = Perpendicular(plane.normal).Normalize();
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = plane.normal.Cross(perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y,-perpendiculars[2].z };

	Vector3D points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3D extend = perpendiculars[index] * 2.0f;
		Vector3D point = center + extend;
		points[index] = point * viewProjectionMatrix * viewPortMatrix;
	}

	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}

bool IsCollisioPlaneSegment(const Plane& plane, const Segment& segment) {
	Vector3D b = segment.diff - segment.origin;

	float dot = b.Dot(plane.normal);

	if (dot == 0.0f) { return false; };

	float t = (plane.distance - segment.origin.Dot(plane.normal)) / dot;

	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}

bool IsCollisionTriangle(const Vector3D& pos0, const Vector3D& pos1, const Vector3D& pos2, const Segment& segment) {
	Vector3D normal = (pos1 - pos0).Cross(pos2 - pos1).Normalize();

	float distance = pos0.Dot(normal);

	Plane plane;
	plane.distance = distance;
	plane.normal = Project(normal, pos0).Normalize();

	Vector3D b = segment.diff;
	//b = b.Normalize();

	float dot = b.Dot(plane.normal);

	if (dot == 0.0f) { return false; };

	float t = (plane.distance - segment.origin.Dot(plane.normal)) / dot;
	if (!(0.0f >= t && t <= 1.0f)) {
		return false;
	}

	Vector3D p = b * t;
	p += segment.origin;

	Vector3D cross1 = (pos1 - pos0).Cross(p - pos1);
	Vector3D cross2 = (pos2 - pos1).Cross(p - pos2);
	Vector3D cross3 = (pos0 - pos2).Cross(p - pos0);


	if (cross1.Dot(normal) >= 0.0f &&
		cross2.Dot(normal) >= 0.0f &&
		cross3.Dot(normal) >= 0.0f
		) {
		return true;
	}

	return false;
}

void DrawTriangle(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Vector3D screenPos1 = pos1 * viewProjectionMatrix * viewPortMatrix;
	Vector3D screenPos2 = pos2 * viewProjectionMatrix * viewPortMatrix;
	Vector3D screenPos3 = pos3 * viewProjectionMatrix * viewPortMatrix;
	
	
	Novice::DrawTriangle(static_cast<int>(screenPos1.x), static_cast<int>(screenPos1.y), static_cast<int>(screenPos2.x), static_cast<int>(screenPos2.y), static_cast<int>(screenPos3.x), static_cast<int>(screenPos3.y), color, kFillModeWireFrame);
}