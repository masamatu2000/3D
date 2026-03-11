#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
class Player : public Object3D
{
public: 
	Player();
	~Player();
	void Update() override;
	VECTOR3 GetPosition() { return position; }
	void DrawStaminaUI();
	bool IsOnFigher();
	void SetCam(bool cam) { IsPlayerCam = cam; }
private:
	float velocityY;
	bool OnGround;
	float Stamina;
	float MaxStamina;
	int timer;
	bool IsTired;
	bool IsPlayerCam;
	bool IsOnFighter;
};