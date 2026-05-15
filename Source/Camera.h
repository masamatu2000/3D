#pragma once
#include"Object3D.h"
class Camera :public Object3D {
public:
	Camera();
	~Camera();
	void Update()override;
	void Draw()override;//SetCameraをやるため
	void SetPlayerPosition(VECTOR3 pos) { lookPos = pos; }
	VECTOR3 ForWard();
private:
	VECTOR3 lookPos;
};