#include "Object.h"
#include"Stage.h"
#include<assert.h>
#include"Player.h"
namespace {
	const VECTOR3 PlayerLimitDis = { 50,0,50 };
	int GAME_CLEAR_TIMER = 3600;
}
Object::Object(const VECTOR3 &pos,int time)
{
	hModel = MV1LoadModel("data/models/Character/RedGoblin/RedGoblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = pos;
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	timer = time;
	patrolTarget= VECTOR3(GetRand(2000) - 1000, 0, GetRand(2000) - 1000);
	ChangeSpeed = 6;
}

Object::~Object()
{
}

void Object::Update()
{
	Player* player = FindGameObject<Player>();
	playerPos = player->GetPosition();
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		position = hitPos;
	}
	float DistX = (playerPos.x - position.x);
	float DistZ = (playerPos.z - position.z);
	dist = VECTOR3{ DistX,0,DistZ };
	float distance = VSize(dist);
	
	if (distance < 1500 ) {

		// Œü‚«
		float angle = atan2(dist.x, dist.z) + DX_PI_F;
		rotation.y = angle;

		// ³‹K‰»
		VECTOR3 dir = VNorm(dist);

		// ˆÚ“®
		position.x += dir.x * ChangeSpeed;
		position.z += dir.z * ChangeSpeed;
	}
	else {
		VECTOR3 dir = VSub(patrolTarget, position);

		if (VSize(dir) < 50) {
			patrolTarget = VECTOR3(GetRand(2000) - 1000, 0, GetRand(2000) - 1000);
		}

		dir = VNorm(dir);

		position.x += dir.x * 2;
		position.z += dir.z * 2;
	}

	if (abs(dist.z) < PlayerLimitDis.z && abs(dist.x) < PlayerLimitDis.x) {//ƒxƒNƒ^[Œ^“¯Žm‚ÌŒvŽZ‚Í–³—@x,y,z‚»‚ê‚¼‚ê‚ÅŒvŽZ‚·‚é
		SceneManager::ChangeScene("GAME OVER");
	}
	if (timer < GAME_CLEAR_TIMER) {
		timer++;
		if (timer > GAME_CLEAR_TIMER - 60 * 10) {
			ChangeSpeed = 8;
		}
		else if (timer > GAME_CLEAR_TIMER / 2) {
			ChangeSpeed = 6.5;
		}
	}
}
