#include "Mouse.h"
#include "Camera/Camera.h"
#include <Novice.h>

bool Mouse::isButton[(int)Button::MOUSE_BUTTON_NUMBER] = {
	false,
	false,
	false,
	false,
	false
};
bool Mouse::isPreButton[(int)Button::MOUSE_BUTTON_NUMBER] = {
	false,
	false,
	false,
	false,
	false
};
int Mouse::wheel = 0;

int Mouse::x = 0;
int Mouse::y = 0;

void Mouse::Input() {
	Mouse::CheckPreButton();

	Mouse::Wheel();

	for (int i = 0; i < (int)Button::MOUSE_BUTTON_NUMBER; i++) {
		isButton[i] = Novice::IsPressMouse(i);
	}

	Mouse::SetPos();
}

void Mouse::CheckPreButton() {
	memcpy(isPreButton, isButton, sizeof(bool) * (int)Button::MOUSE_BUTTON_NUMBER);
}

bool  Mouse::Pushed(Button buttonNumber) {
	if (isButton[(int)buttonNumber] && !isPreButton[(int)buttonNumber]) {
		return 1;
	}
	return 0;
}

bool Mouse::LongPush(Button buttonNumber) {
	if (isButton[(int)buttonNumber] && isPreButton[(int)buttonNumber]) {
		return 1;
	}
	return 0;
}

bool Mouse::Releaseed(Button buttonNumber) {
	if (!isButton[(int)buttonNumber] && isPreButton[(int)buttonNumber]) {
		return 1;
	}
	return 0;
}

void Mouse::Wheel() {
	wheel = 0;
	wheel = Novice::GetWheel();
}

void Mouse::SetPos() {
	Novice::GetMousePosition(&x, &y);
}

void Mouse::Draw() {
	Novice::ScreenPrintf(0, 0, "Left Click = %d", isButton[(int)Button::LEFT_BUTTON]);
	Novice::ScreenPrintf(0, 20, "Rightt Click = %d", isButton[(int)Button::RIGHT_BUTTON]);
	Novice::ScreenPrintf(0, 40, "Middle Click = %d", isButton[(int)Button::MIDDLE_BUTTON]);
	Novice::ScreenPrintf(0, 60, "side_back Click = %d", isButton[(int)Button::BACK_SIDE_BUTTON]);
	Novice::ScreenPrintf(0, 80, "side_forward Click = %d", isButton[(int)Button::FORWARD_SIDE_BUTTON]);
	Novice::ScreenPrintf(0, 100, "Wheel = %d", wheel);
}

Vector3D Mouse::GetWorldPos(const Mat4x4& vpvpMat) {
	Vector3D tmp(static_cast<float>(x), static_cast<float>(y),0.0f);

	return tmp * MakeMatrixInverse(vpvpMat);
}

bool Mouse::IsMouseRange(const Vector3D& pos, float radius, const Mat4x4& vpvpMat) {
	Vector3D mousePos(static_cast<float>(x), static_cast<float>(y),0.0f);

	mousePos *= MakeMatrixInverse(vpvpMat);

	if ((pos - mousePos).Length() < radius) {
		return true;
	}
	else {
		return false;
	}
}