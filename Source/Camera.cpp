#include "Camera.h"
#include"Pad.h"
Camera::Camera()
{
	rotation.y = 0.0f;
	rotation.x = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Update()
{
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
	VECTOR3 lp = lookPos + VECTOR3(0, 150, 0);
	
	VECTOR3 offset =
		VECTOR3(0, 0, -600)
		* MGetRotX(rotation.x)
		* MGetRotY(rotation.y);

	VECTOR3 camPos = lp + offset;
	SetCameraPositionAndTarget_UpVecY(camPos, lp);
}

VECTOR3 Camera::ForWard()
{
	return VECTOR3(0,0,1)*MGetRotY(rotation.y);
}
