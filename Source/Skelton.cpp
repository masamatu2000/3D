#include "Skelton.h"
#include<assert.h>
#include"Stage.h"
#include"Player.h"
namespace {
	const VECTOR3 PlayerLimitDis = { 50,0,50};
	int GAME_CLEAR_TIMER = 3600;
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
	ChangeSpeed = 4;
}

Skelton::Skelton(const VECTOR3 &pos, float speed, int time)
{
	hModel = MV1LoadModel("data/models/Skelton/Skelton.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = pos;
	rotation = VECTOR3(0, 0, 0);
	timer = time;
	ChangeSpeed = speed;
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
	float DistX = (playerPos.x - position.x);
	float DistZ = (playerPos.z - position.z);
	dist = VECTOR3{ DistX,0,DistZ };
	// å¸Ç´
	float angle = atan2(dist.x, dist.z)+DX_PI_F;
	rotation.y = angle;
	// ê≥ãKâª
	VECTOR3 dir = VNorm(dist);
	// à⁄ìÆ
	position.x+= dir.x * ChangeSpeed;
	position.z+= dir.z * ChangeSpeed;
	if (abs(dist.z) < PlayerLimitDis.z && abs(dist.x)< PlayerLimitDis.x) {//ÉxÉNÉ^Å[å^ìØémÇÃåvéZÇÕñ≥óùÅ@x,y,zÇªÇÍÇºÇÍÇ≈åvéZÇ∑ÇÈ
		SceneManager::ChangeScene("GAME OVER");
	}
	if (timer < GAME_CLEAR_TIMER) {
		timer++;
		if (timer > GAME_CLEAR_TIMER - 60 * 10) {
			ChangeSpeed = 8;
		}
		else if (timer > GAME_CLEAR_TIMER / 2) {
			ChangeSpeed = 6;
		}
	}
}
