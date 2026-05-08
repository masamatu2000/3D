#pragma once
#include"Object3D.h"
#include"Animator.h"
class Skelton : public Object3D
{
public:
	Skelton();
	Skelton(const VECTOR3 &pos,int time);
	~Skelton();
	void Update() override;
private:
	VECTOR3 playerPos;
	VECTOR3 dist;
	int timer;
	VECTOR3 Dir;
	float ChangeSpeed;
	VECTOR3 patrolTarget;
	enum AnimState {
		Anim_Idle,
		Anim_Run,
		Anim_Damage,
	};
	Animator* animator;
};
