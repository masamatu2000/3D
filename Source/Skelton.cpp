#include "Skelton.h"
#include<assert.h>
#include"Stage.h"
#include"Player.h"
namespace {
	const VECTOR3 PlayerLimitDis = { 50,0,50};
	int GAME_CLEAR_TIMER = 3600;
	float DistX;
	float DistY;
	float DistZ;
}
Skelton::Skelton()
{
	hModel = MV1LoadModel("data/models/Skelton/Skelton.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
	timer = 0;
}

Skelton::~Skelton()
{
}

void Skelton::Update()
{
	Player* player = FindGameObject<Player>();
	playerPos = player->GetPosition();
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		position = hitPos;
	}
	DistX =(playerPos.x-position.x);
	DistY = (playerPos.y - position.y);
	DistZ = (playerPos.z - position.z);
	dist = VECTOR3{ DistX,0,DistZ };
	if (abs(dist.z) < PlayerLimitDis.z && abs(dist.x)< PlayerLimitDis.x) {//ƒxƒNƒ^[Œ^“¯Žm‚ÌŒvŽZ‚Í–³—@x,y,z‚»‚ê‚¼‚ê‚ÅŒvŽZ‚·‚é
		SceneManager::ChangeScene("GAME OVER");
	}
	
	if (timer < GAME_CLEAR_TIMER) {
		timer++;
		DrawFormatString(1024 / 2, 0, GetColor(255, 255, 255), "ƒQ[ƒ€ƒNƒŠƒA‚Ü‚Å‚ ‚Æ%d•b", (GAME_CLEAR_TIMER - timer) / 60);
		if (timer >= GAME_CLEAR_TIMER) {
			SceneManager::ChangeScene("GAME CLEAR");
		}
	}
	position += dist * Time::DeltaTime()*0.5;
}
