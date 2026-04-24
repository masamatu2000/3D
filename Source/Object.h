#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
#include"Animator.h"
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
	Animator* animator;
	enum AnimState {
		Anim_Neutral,
		Anim_Run,
		Anim_Attack1,
	};
};
//問題
//プレイヤーが持った剣先の座標の求め方
//root+肩+肘+手首+剣先