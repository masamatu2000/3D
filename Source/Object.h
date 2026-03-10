#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
class Object : public Object3D
{
public:
	Object();
	~Object();
	void Update() override;
	
private:
	
};