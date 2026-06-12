#pragma once
#include"../Library/GameObject.h"
class Pad :public GameObject{
public:
	Pad();
	~Pad();
	void Update() override;
	float GetThumbLX() { return inputState.ThumbLX; }
	float LStickX();//Lスティックを横に倒した量(-1.0~1.0)
	float LStickY();
	float RStickX();
	float RStickY();
	bool IsPushed(int id);//idのボタンを押しているか
	bool PushTrigger(int id);//idのボタンを押した瞬間か
	static const int ATTACK = XINPUT_BUTTON_X;
	static const int JUMP = XINPUT_BUTTON_B;
	static const int DASH = XINPUT_BUTTON_A;
private:
	XINPUT_STATE inputState;
	XINPUT_STATE prevState;
};