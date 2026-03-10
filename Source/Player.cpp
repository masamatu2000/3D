#include "Player.h"
#include<assert.h>
#include"Stage.h"
namespace {
	const float DASH_SPEED = 3.0f;
	const int STAMINA_HEEL_TIMER = 60 * 3;
	const float G = 3.0f / 60.0f;
	const float H = 64.0f * 3.0f;
}
Player::Player()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0,300,-400),VECTOR3(0,250,0));
	hModel = MV1LoadModel("data/models/Character/Player/PC.mv1");
	assert(hModel >0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0,0,0);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	velocityY = 0;
	OnGround = true;
	Stamina = 60 * 4;
	MaxStamina = Stamina;
	timer = 0;
	IsTired = false;
}

Player::~Player()
{
}

void Player::Update()
{
	if (!CheckHitKey(KEY_INPUT_LSHIFT) && IsTired) {
		timer++;
	}
	if (timer > STAMINA_HEEL_TIMER&&IsTired&&!CheckHitKey(KEY_INPUT_LSHIFT)) {
		Stamina += MaxStamina / 3*Time::DeltaTime();
		if (Stamina >= MaxStamina) {
			Stamina = MaxStamina;
			timer = 0;
			IsTired = false;
		}
	}
	if (CheckHitKey(KEY_INPUT_D)) {
		rotation.y += 3.0f*DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		rotation.y -= 3.0f*DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
		VECTOR3 velocity;
		if (CheckHitKey(KEY_INPUT_LSHIFT)&&Stamina>0) {
			Stamina--;
			velocity = VECTOR3(0, 0, 5*DASH_SPEED) * MGetRotY(rotation.y);
			IsTired = true;
		}
		//VECTOR3 velocity = VECTOR3(sinf(rotation.y) ,0, cosf(rotation.y))*3.0f;
		else {
			velocity = VECTOR3(0, 0, 5) * MGetRotY(rotation.y);
		}
		//↑回っていないベクトル*回転行列
		position += velocity;
	}
	DrawStaminaUI();
	//地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	if (Input::IsKeyOnTrig(KEY_INPUT_SPACE)&&OnGround) {
		velocityY= sqrt(2 * G * H);
		OnGround = false;
	}
	position.y += velocityY;
	velocityY -= G;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		if (position.y < hitPos.y) {
			position = hitPos;
			OnGround = true;
		}
	}
	//カメラの位置をプレイヤーの位置に合わせる->回っていないベクトル＊プレイヤーの回転行列+プレイヤーの位置
	VECTOR3 camPos = VECTOR3(0, 300, -400);
	if (CheckHitKey(KEY_INPUT_R)) {
		camPos.z *= -1;
	}
	camPos = camPos * MGetRotY(rotation.y) + position;
	SetCameraPositionAndTarget_UpVecY(camPos, position+VECTOR3(0, 250, 0));
	DrawFormatString(0, 200, GetColor(255, 255, 255), "(%f,%f,%f)", position.x, position.y, position.z);
}

void Player::DrawStaminaUI() {

	float RatioOfStamina = Stamina / MaxStamina;
	if (RatioOfStamina < 0) {
		RatioOfStamina = 0;
	}
	float BarHeight = 400+MaxStamina;
	float CurrBarHeight = BarHeight * RatioOfStamina;
	DrawBox(0, 400, 100, BarHeight, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 400+MaxStamina*(1-RatioOfStamina), 100, BarHeight, GetColor(253, 126, 0), TRUE);
}