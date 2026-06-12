#include "Goblin.h"
#include <assert.h>
#include"Player.h"
#include"Stage.h"
Goblin::Goblin(VECTOR3 pos, float rotY)
{
	hModel = MV1LoadModel("data/models/Character/Goblin/Goblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = pos;
	spawnPoint = pos;
	rotation = VECTOR3(0, rotY, 0);

	animator = new Animator(hModel);
	animator->AddFile(aStand, "data/models/Character/Goblin/Anim_Neutral.mv1", true);
	animator->AddFile(aRun, "data/models/Character/Goblin/Anim_Run.mv1", true);
	animator->AddFile(aDamage, "data/models/Character/Goblin/Anim_Damage.mv1", false);
	animator->Play(aStand);

	state = sNormal;
	cap = new Capsule(VECTOR3(pos.x, pos.y+150, pos.z), VECTOR3(pos.x, pos.y, pos.z), 40);
}

Goblin::~Goblin()
{
	delete animator;
	delete cap;
}

void Goblin::Update()
{
	animator->Update();
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		if (position.y < hitPos.y) {
			position = hitPos;
		}
	}
	cap->bottom = VECTOR3(position.x, position.y, position.z);
	cap->top = VECTOR3(position.x, position.y + 150, position.z);
	switch (state) {
	case sNormal:
		UpdateNormal();
		break;
	case sDamage:
		UpdateDamage();
		break;
	case sChase:
		UpdateChase();
		break;
	case sGoBack:
		UpdateGoBack();
		break;
	}

}

void Goblin::Draw()
{
	Object3D::Draw();
	//カプセルの表示
	VECTOR3 pos = position;
	DrawCapsule3D(VECTOR3(pos.x, pos.y + 150, pos.z), VECTOR3(pos.x, pos.y , pos.z), cap->radius, 10, COL_RED,COL_RED,FALSE);
}

bool Goblin::PlayerAttack(VECTOR3 pos1, VECTOR3 pos2)
{
	//VECTOR3 p = position;
	float dist = Segment_Segment_MinLength(pos1, pos2, cap->bottom, cap->top);
	if (dist<cap->radius) {
		state = sDamage;
		animator->Play(aDamage);
		return true;
	}
	return false;
}

void Goblin::UpdateNormal()
{
	//Playerの気配を感じ取る必要がある
	Player* p = FindGameObject<Player>();
	VECTOR3 toPlayer = p->GetPosition() - position;//自分からプレイヤーへのベクトル
	if (toPlayer.Size() < 100.0f) {
		animator->Play(aRun);
		state = sChase;
	}
	//視野判定、半径10m、視野角６０°の内側
	else if (toPlayer.Size() < 10000.0f) {
		VECTOR3 a = VECTOR3(0, 0, 1) * MGetRotY(rotation.y);
		VECTOR3 b = toPlayer.Normalize();
		float cos_ = a.Dot(b);
		if (cos_ > cos(60* DegToRad)) {
			animator->Play(aRun);
			state = sChase;
		}
	}

}

void Goblin::UpdateDamage()
{
	float frame = animator->GetCurrentFrame();
	if (frame < 7.0f) {
		Player* pl = FindGameObject<Player>();
		VECTOR3 Attackedpos = pl->GetPos() - position;
		float rate = frame / 7.0f;//0.0~1.0になる
		Attackedpos = Attackedpos.Normalize();
		VECTOR3 TargetPos = position -Forward() * 50.0f;
		position = ((TargetPos - position) * rate + position);//Lerpのやつだね！
		rotation.y = atan2(Attackedpos.x, Attackedpos.z);
	}
	if (animator->IsFinish()) {
		state = sNormal;
		animator->Play(aStand);
	}
}

void Goblin::UpdateChase()
{
	Player* p = FindGameObject<Player>();
	VECTOR3 toPlayer = p->GetPosition() - position;//自分からプレイヤーへのベクトル
	position += VECTOR3(0, 0, 1) * MGetRotY(rotation.y)*3.0;
	
	VECTOR3 a = VECTOR3(1, 0, 0) * MGetRotY(rotation.y);
	VECTOR3 b = toPlayer.Normalize();
	float cos_ = a.Dot(b);
	//右にいれば、角度を増やす（角度を増やせばcosの値がどんどんちっちゃくなっていき０になる。つまり、2つのベクトルが重なるから。）
	if (cos_>=0) {
		rotation.y += 1.0f * DegToRad;
	}
	//左にいれば、角度を減らす（角度を減らせばcosの値がマイナスからどんどん大きくなっていき０になる。つまり、2つのベクトルが重なるから。）
	else {
		rotation.y -= 1.0f * DegToRad;
	}
	VECTOR3 f = spawnPoint - position;
	if (f.Size() > 1000.0f) {
		state = sGoBack;
	}
}

void Goblin::UpdateGoBack()
{
	
	
	VECTOR3 toSpawn = spawnPoint - position;
	VECTOR3 s = toSpawn.Normalize();
	VECTOR3 a = VECTOR3(1, 0, 0) * MGetRotY(rotation.y);
	float cos_ = a.Dot(s);
	position += VECTOR3(0, 0, 1) * MGetRotY(rotation.y) * 1.5f;
	//右にいれば、角度を増やす（角度を増やせばcosの値がどんどんちっちゃくなっていき０になる。つまり、2つのベクトルが重なるから。）
	if (cos_ >= 0) {
		rotation.y += 1.0f * DegToRad;
	}
	//左にいれば、角度を減らす（角度を減らせばcosの値がマイナスからどんどん大きくなっていき０になる。つまり、2つのベクトルが重なるから。）
	else {
		rotation.y -= 1.0f * DegToRad;
	}
	if (toSpawn.Size() < 10.0f) {
		state = sNormal;
	}
}

