#pragma once
#include <string>

class Mat4x4;

class Vector3D {
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& vec) = default;

public:
	float x;
	float y;
	float z;

public:
	Vector3D& operator=(const Vector3D& vec) = default;
	Vector3D operator+(const Vector3D& vec) const;
	Vector3D operator-(const Vector3D& vec) const;
	Vector3D operator+=(const Vector3D& vec);
	Vector3D operator-=(const Vector3D& vec);
	Vector3D operator*(float scalar) const;
	Vector3D operator*=(float scalar);
	Vector3D Normalize() const;
	Vector3D operator*(const Mat4x4& mat) const;
	Vector3D& operator*=(const Mat4x4& mat);

	float Dot(const Vector3D& vec) const;
	Vector3D Cross(const Vector3D& vec) const;
	float Length() const;

	friend void Vec3ScreenPrintf(int x, int y, const Vector3D& vec, std::string label);
};

struct Line {
	Vector3D origin;
	Vector3D diff;
};

struct Ray {
	Vector3D origin;
	Vector3D diff;
};

struct Segment {
	Vector3D origin;
	Vector3D diff;
};

Vector3D Project(const Vector3D& vec1, const Vector3D& vec2);

Vector3D ClosestPoint(const Vector3D& point, const Segment& segment);