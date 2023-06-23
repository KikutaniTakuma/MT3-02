#include "MyModel.h"
#include "Novice.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <array>

MyModel::MyModel():
	pos(),
	scale(1.0f,1.0f,1.0f),
	rotation(),
	worldMat(),
	vertex(0),
	index(0),
	maxLocalPos(),
	minLocalPos()
{}

void MyModel::LoadObj(const std::string& fileName) {
	std::ifstream objFile(fileName);
	assert(objFile);

	std::string lineBuf;

	while (std::getline(objFile, lineBuf)) {
		std::istringstream line(lineBuf);
		if (lineBuf.find("#") == std::string::npos && lineBuf.find(".mtl") == std::string::npos) {
			if (lineBuf.find("v") != std::string::npos && lineBuf.find("vn") == std::string::npos && lineBuf.find("vt") == std::string::npos) {
				std::string buff;
				std::vector<float> posBuf(0);
				while (getline(line, buff, ' '))
				{
					if (std::any_of(buff.cbegin(), buff.cend(), isdigit)) {
						posBuf.push_back(std::stof(buff));
					}
				}
				if (posBuf.size() == 3) {
					if (posBuf[0] >= maxLocalPos.x) {
						maxLocalPos.x = posBuf[0];
					}
					if (posBuf[1] >= maxLocalPos.y) {
						maxLocalPos.y = posBuf[1];
					}
					if (posBuf[2] >= maxLocalPos.z) {
						maxLocalPos.z = posBuf[2];
					}

					if (posBuf[0] <= minLocalPos.x) {
						minLocalPos.x = posBuf[0];
					}
					if (posBuf[1] <= minLocalPos.y) {
						minLocalPos.y = posBuf[1];
					}
					if (posBuf[2] <= minLocalPos.z) {
						minLocalPos.z = posBuf[2];
					}

					vertex.push_back(Vector3D(posBuf[0], posBuf[1], posBuf[2]));
				}
			}
			else if (lineBuf.find("f") != std::string::npos) {
				std::string buff;
				std::vector<float> posBuf(0);
				while (getline(line, buff, ' '))
				{
					/// 0:vertexNumber 1:textureCoordinate 2:NormalNumber
					std::string num[3];
					int32_t count = 0;
					if (std::any_of(buff.cbegin(), buff.cend(), isdigit)) {
						for (auto ch = buff.begin(); ch != buff.end(); ch++) {
							if (*ch == '/') {
								count++;
							}
							else { num[count] += *ch; }
						}
					}
					if (!num[0].empty()) {
						index.push_back(static_cast<uint32_t>(std::stoi(num[0])) - 1u);
					}
				}
			}
		}
	}
	objFile.close();
}

void MyModel::WorldMatUpdate() {
	worldMat.Affin(scale, rotation, pos);
}

bool MyModel::IsCollision(const MyModel& model) const {
	Vector3D worldMax = maxLocalPos * worldMat;
	Vector3D worldMin = minLocalPos * worldMat;
	Vector3D worldMax2 = model.maxLocalPos * model.worldMat;
	Vector3D worldMin2 = model.minLocalPos * model.worldMat;

	if (
		(worldMin.x <= worldMax2.x && worldMax.x >= worldMin2.x) &&
		(worldMin.y <= worldMax2.y && worldMax.y >= worldMin2.y) &&
		(worldMin.z <= worldMax2.z && worldMax.z >= worldMin2.z)
		)
	{
		return true;
	}

	return false;
}

void MyModel::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortMatrix, uint32_t color) {
	Mat4x4 worldVpVpMat = worldMat * viewProjectionMatrix * viewPortMatrix;
	std::array<Vector3D, 3> screen;

	for (auto i = index.begin(); i != index.end();) {
		for (auto& j : screen) {
			j = vertex[*i] * worldVpVpMat;
			i++;
		}

		Novice::DrawTriangle(static_cast<int32_t>(screen[0].x), static_cast<int32_t>(screen[0].y), static_cast<int32_t>(screen[1].x), static_cast<int32_t>(screen[1].y), static_cast<int32_t>(screen[2].x), static_cast<int32_t>(screen[2].y), color, kFillModeWireFrame);
	}
}