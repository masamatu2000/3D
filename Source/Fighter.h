#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
class Fighter : public Object3D
{
public:
	Fighter(const VECTOR3& pos);
	~Fighter();
	void Update() override;

private:
	VECTOR3 playerPos;
	VECTOR3 dist;
	int timer;
	VECTOR3 Dir;
	float ChangeSpeed;
	VECTOR3 patrolTarget;
	bool IsAttached;
};
