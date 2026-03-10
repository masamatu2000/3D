#pragma once
#include"../library/GameObject.h"

class Object3D : public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Draw() override;
	virtual void SetVel(VECTOR3 speed) { velocity = speed; }
	virtual VECTOR3 GetVel() { return velocity; }
	virtual VECTOR3 GetPos() { return position; }
	virtual void SetPos(VECTOR3 pos) { position = pos; }
	virtual void SetModel(int model) { hModel = model; }
protected:
	int hModel;
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 velocity;
};