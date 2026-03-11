#include "Fighter.h"
#include"Player.h"
#include<assert.h>
#include"Stage.h"
Fighter::Fighter(const VECTOR3& pos)
{
	hModel = MV1LoadModel("data/models/Fighter/Fighter.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = pos;
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	patrolTarget = VECTOR3(GetRand(2000) - 1000, 0, GetRand(2000) - 1000);
	ChangeSpeed = 6;
	timer = 0;
	IsAttached = false;
}

Fighter::~Fighter()
{
}

void Fighter::Update()
{
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		if (position.y < hitPos.y) {
			position = hitPos;
		}
	}
	Player* player = FindGameObject<Player>();
	VECTOR3 camPos = VECTOR3(0, 300, -400);
	if (player->IsOnFigher()) {
		if (CheckHitKey(KEY_INPUT_D)) {
			rotation.y += 3.0f * DegToRad;
		}
		if (CheckHitKey(KEY_INPUT_A)) {
			rotation.y -= 3.0f * DegToRad;
		}
		if (CheckHitKey(KEY_INPUT_W)) {
			VECTOR3 velocity;
			velocity = VECTOR3(0, 0, 10) * MGetRotY(rotation.y);
			position += velocity;
		}
		timer++;
		if (timer > 60 * 10) {
			player->SetPos(position);
			player->SetCam(true);
			DestroyMe();
		}
		if (CheckHitKey(KEY_INPUT_R)) {
			camPos.z *= -1;
		}
		camPos = camPos * MGetRotY(rotation.y) + position;
		SetCameraPositionAndTarget_UpVecY(camPos, position + VECTOR3(0, 250, 0));
	}
	
}
