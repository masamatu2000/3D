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
	void Delay(float timer=0.3f);
private:
	VECTOR3 lookPos;
	float fov;//画角用
	float delayTime;//遅延時間
	float delayElapseTime;//遅延の経過時間
};