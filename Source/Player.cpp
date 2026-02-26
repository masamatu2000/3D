#include "Player.h"
#include<assert.h>
#include"Stage.h"
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
}

Player::~Player()
{
}

void Player::Update()
{
	const float G = 3.0f / 60.0f;
	const float H = 64.0f * 3.0f;
	if (CheckHitKey(KEY_INPUT_D)) {
		rotation.y += 3.0f*DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		rotation.y -= 3.0f*DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
		//VECTOR3 velocity = VECTOR3(sinf(rotation.y) ,0, cosf(rotation.y))*3.0f;
		VECTOR3 velocity = VECTOR3(0, 0, 5) * MGetRotY(rotation.y);
		//↑回っていないベクトル*回転行列
		position += velocity;
	}
	//地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	if (Input::IsKeyOnTrig(KEY_INPUT_SPACE)) {
		velocityY= sqrt(2 * G * H);
	}
	position.y += velocityY;
	velocityY -= G;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		if(position.y<hitPos.y)
		position = hitPos;
	}
	//カメラの位置をプレイヤーの位置に合わせる->回っていないベクトル＊プレイヤーの回転行列+プレイヤーの位置
	VECTOR3 camPos = VECTOR3(0, 300, -400);
	if (CheckHitKey(KEY_INPUT_R)) {
		camPos.z *= -1;
	}
	camPos = camPos * MGetRotY(rotation.y) + position;
	SetCameraPositionAndTarget_UpVecY(camPos, position+VECTOR3(0, 250, 0));
}

void MV1CollCheck_Line(int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd) {

}
