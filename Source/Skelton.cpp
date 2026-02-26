#include "Skelton.h"
#include<assert.h>
#include"Stage.h"
#include"Player.h"
namespace {
	VECTOR3 PlayerLimitDis = { 0,0,10 };
}
Skelton::Skelton()
{
	hModel = MV1LoadModel("data/models/Skelton/Skelton.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
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
	dist = playerPos - position;
	if (dist.z < PlayerLimitDis.z && dist.x < PlayerLimitDis.x) {//ƒxƒNƒ^[Œ^“¯Žm‚ÌŒvŽZ‚Í–³—@x,y,z‚»‚ê‚¼‚ê‚ÅŒvŽZ‚·‚é
		SceneManager::ChangeScene("GAME OVER");
	}
	position += dist * Time::DeltaTime();
}
