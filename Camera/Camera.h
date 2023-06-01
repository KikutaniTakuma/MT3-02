#pragma once
#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"

class Camera {
public:
	Camera();


public:
	void Update();

	inline Mat4x4 getViewProjectionMatrix() const {
		return viewProjection;
	}

	inline Mat4x4 getViewPortMatrix() const {
		return viewPortMatirx;
	}

private:
	Mat4x4 viewMatrix;
	Mat4x4 projectionMatrix;
	Mat4x4 viewPortMatirx;
	Mat4x4 viewProjection;

public:
	int windowWidth;
	int windowHeight;

	Vector3D pos;
	Vector3D scale;
	Vector3D rotate;

	float fovY;
	float nearClip;
	float farClip;

	float left;
	float right;
	float minDepth;
	float maxDepth;
};