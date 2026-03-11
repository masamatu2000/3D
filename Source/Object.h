#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
class Object : public Object3D
{
public:
	Object(const VECTOR3 &pos,int time);
	~Object();
	void Update() override;
	
private:
	VECTOR3 playerPos;
	VECTOR3 dist;
	int timer;
	VECTOR3 Dir;
	float ChangeSpeed;
	VECTOR3 patrolTarget;
};