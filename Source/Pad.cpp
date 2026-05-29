#include "Pad.h"
#include"../ImGui/imgui.h"
float StickCal(float stick);
Pad::Pad()
{
	DontDestroyOnSceneChange();
}

Pad::~Pad()
{
}

void Pad::Update()
{
	prevState = inputState;
	GetJoypadXInputState(DX_INPUT_PAD1, &inputState);
	if (CheckHitKey(KEY_INPUT_D)) {
		inputState.ThumbLX = 32767;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		inputState.ThumbLX = -32768;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
		inputState.ThumbLY = 32767;
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		inputState.ThumbLY = -32768;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		inputState.ThumbRY = -32768;
	}
	if (CheckHitKey(KEY_INPUT_UP)) {
		inputState.ThumbRY = 32767;
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		inputState.ThumbRX = -32768;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		inputState.ThumbRX = 32767;
	}
	if (CheckHitKey(KEY_INPUT_B)) {
		inputState.Buttons[ATTACK] = 1;
	}
	ImGui::Begin("PAD");
	int xl = inputState.ThumbLX;
	int yl = inputState.ThumbLY;
	int xr = inputState.ThumbRX;
	int yr = inputState.ThumbRY;
	ImGui::InputInt("XL", &xl);
	ImGui::InputInt("YL", &yl);
	ImGui::InputInt("XR", &xr);
	ImGui::InputInt("YR", &yr);
	ImGui::End();

}

float Pad::LStickX()
{
	float x = StickCal((float)inputState.ThumbLX);
	ImGui::Begin("PAD");
	ImGui::InputFloat("X1.0:", &x);
	ImGui::End();
	return x;
}

float Pad::LStickY()
{
	return StickCal((float)inputState.ThumbLY);
}

float Pad::RStickX()
{
	return StickCal((float)inputState.ThumbRX);
}

float Pad::RStickY()
{
	return StickCal((float)inputState.ThumbRY);
	
}

bool Pad::IsPushed(int id)
{
	return inputState.Buttons[id]!=0;
}

bool Pad::PushTrigger(int id)
{
	if (prevState.Buttons[id]==0&&inputState.Buttons[id]!=0) {
		return true;
	}
	return false;
}

float StickCal(float stick)
{
	
	float s = stick / (float)32767;
	if (s > -0.2f && s < 0.2f) {
		s = 0.0f;//中央付近を無視
	}
	return s;
}
