#pragma once
#include <vector>
#include <string> 
#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"

class MyModel {
public:
	MyModel();
	~MyModel() = default;

	MyModel(const MyModel&) = default;
	MyModel(MyModel&&) = default;
	MyModel& operator=(const MyModel&) = default;

public:
	void LoadObj(const std::string& fileName);

	void WorldMatUpdate();

	bool IsCollision(const MyModel& model) const;
	bool IsCollision(const class Sphere& sphere) const;

	void Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color);

public:
	Vector3D pos;
	Vector3D scale;
	Vector3D rotation;

private:
	Mat4x4 worldMat;

private:
	std::vector<Vector3D> vertex;
	std::vector<uint32_t> index;

	Vector3D maxLocalPos;
	Vector3D minLocalPos;
};