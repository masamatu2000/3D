#include "Camera.h"
#include"Pad.h"
#include"Player.h"
Camera::Camera()
{
	rotation.y = 0.0f;
	rotation.x = 0.0f;
	fov = 60.0f * DegToRad;
	delayTime = 1.0f;
	delayElapseTime = delayTime;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	delayElapseTime += Time::DeltaTime();
	if (CheckHitKey(KEY_INPUT_C)) {
		fov = 45.0f * DegToRad;
	}
	else {
		fov = 60.0f * DegToRad;
	}
	SetupCamera_Perspective(fov);
	//右スティックの左右を見て、rotation.yを変える
	Pad* pad = FindGameObject<Pad>();
	/* if (pad->RStickX() >= 0.9f) {
		rotation.y += 2.2f * DegToRad;
	}
	else if (pad->RStickX() >= 0.5f) {
		rotation.y += 1.0f * DegToRad;
	}
	if (pad->RStickX() <= -0.9f) {
		rotation.y -= 2.2f * DegToRad;
	}
	else if (pad->RStickX() <= -0.5f) {
		rotation.y -= 1.0f * DegToRad;
	}*/
	rotation.y += pad->RStickX() * 3.0f * DegToRad;
	rotation.x += pad->RStickY() * 3.0f * DegToRad;
	if (rotation.x > 80.0f * DegToRad) {
		rotation.x = 80.0f * DegToRad;
	}
	if (rotation.x <- 45.0f * DegToRad) {
		rotation.x = -45.0f * DegToRad;
	}
}

void Camera::Draw()
{
	VECTOR3 target = VECTOR3(0, 0, -500) * MGetRotX(rotation.x) * MGetRotY(rotation.y) + lookPos+VECTOR3(0, 150, 0);
	if (delayElapseTime < delayTime) {
		Player* pl = FindGameObject<Player>();
		float rate = delayElapseTime / delayTime;
		position = (target - position) * rate + position;
	}
	else {
		position = target;
	}
	SetCameraPositionAndTarget_UpVecY(position, lookPos + VECTOR3(0, 150, 0));
}

VECTOR3 Camera::ForWard()
{
	return VECTOR3(0,0,1)*MGetRotY(rotation.y);
}

void Camera::Delay(float timer)
{
	delayTime = timer;
	delayElapseTime = 0.0f;
	if (delayElapseTime < delayTime) {
		
	}
}
