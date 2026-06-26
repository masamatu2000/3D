#include "Player.h"
#include<assert.h>
#include"Stage.h"
#include"Fighter.h"
#include"Goblin.h"
#include"Camera.h"
#include"Pad.h"
namespace {
	const float DASH_SPEED = 3.0f;
	const int STAMINA_HEEL_TIMER = 60 * 3;
	const float G = 3.0f / 60.0f;
	const float H = 64.0f * 3.0f;
	const float LIMIT_FIGHTER_DIS = 50;
	Pad* pad = nullptr;
}
Player::Player(VECTOR3 pos,float rotY)
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0,300,-400),VECTOR3(0,250,0));
	hModel = MV1LoadModel("data/models/Character/Player/PC.mv1");
	assert(hModel >0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	hWeapon = MV1LoadModel("data/models/Character/Weapon/Sabel/Sabel.mv1");
	assert(hWeapon>=0);
	
	position = pos;
	rotation = VECTOR3(0, rotY, 0);
	velocityY = 0;
	OnGround = true;
	Stamina = 60 * 4;
	MaxStamina = Stamina;
	timer = 0;
	IsTired = false;
	IsPlayerCam = true;
	IsOnFighter = false;
	
	animator = new Animator(hModel);
	animator->AddFile(AnimState ::Anim_Run, "data/models/Character/Player/Anim_Run.mv1", true);
	animator->AddFile(AnimState::Anim_Neutral,"data/models/Character/Player/Anim_Neutral.mv1",true);
	animator->AddFile(AnimState::Anim_JumpInFirst, "data/models/Character/Player/Anim_Jump_In.mv1", false);
	animator->AddFile(AnimState::Anim_JumpInLast, "data/models/Character/Player/Anim_Jump_Out.mv1", false);
	animator->AddFile(AnimState::Anim_Attack1, "data/models/Character/Player/Anim_Attack1.mv1", false);
	animator->AddFile(AnimState::Anim_Attack2, "data/models/Character/Player/Anim_Attack2.mv1", false);
	animator->AddFile(AnimState::Anim_Attack3, "data/models/Character/Player/Anim_Attack3.mv1", false);
	animator->Play(Anim_Neutral);
	//animator->Play(0);
	state = sNormal;
	stateInst = new PlayerNormal(this);
	pad = FindGameObject<Pad>();
}

Player::~Player()
{
}

void Player::Update()
{
	//アニメーション再生
	animator->Update();
	stateInst->Update();
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
	
	DrawStaminaUI();
	//地面との当たり判定
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	VECTOR3 pushVec = stage->CollideSphere(position + VECTOR3(0, 60, 0), 50);
	position += pushVec;
	
	position.y += velocityY;
	velocityY -= G;
	stage->CollideRay(position + VECTOR3(0, 100, 0), position + VECTOR3(0, -500, 0), &hitPos);
	if (stage) {
		if (position.y < hitPos.y) {
			position = hitPos;
			OnGround = true;
		}
	}
	if (IsPlayerCam) {
		
			//カメラの位置をプレイヤーの位置に合わせる->回っていないベクトル＊プレイヤーの回転行列+プレイヤーの位置
		VECTOR3 camPos = VECTOR3(0, 300, -400);
		if (CheckHitKey(KEY_INPUT_R)) {
			camPos.z *= -1;
		}

		/**/
		//DrawFormatString(0, 200, GetColor(255, 255, 255), "(%f,%f,%f)", position.x, position.y, position.z);
		Camera* cam = FindGameObject<Camera>();
		cam->SetPlayerPosition(position);
	}
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
bool Player::IsOnFigher() {
	Fighter* fighter = FindGameObject<Fighter>();
	if(fighter==nullptr) return false;
	VECTOR3 fPos = fighter->GetPos();
	float DistX = (fPos.x - position.x);
	float DistZ = (fPos.z - position.z);
	VECTOR3 dist = VECTOR3(DistX, 0, DistZ);
	float distance = VSize(dist);
	if (distance<LIMIT_FIGHTER_DIS) {
		DrawString(1024 / 2, 1024 / 2, "Eボタンを押せ！！", GetColor(255, 255, 255));
		if (CheckHitKey(KEY_INPUT_E)) {
			IsPlayerCam = false;
			IsOnFighter = true;
		}
	}
	return IsOnFighter;
}

void Player::ChangeState(State st)
{
	if (state != st) {
		state = st;
		if (stateInst != nullptr) {
			delete stateInst;
		}
		switch (st) {
			case sNormal:
				stateInst = new PlayerNormal(this);
				break;
			case sAttack1:
				stateInst = new PlayerAttack1(this);
				break;
			case sAttack2:
				stateInst = new PlayerAttack2(this);
				break;
			case sAttack3:
				stateInst = new PlayerAttack3(this);
				break;
		}
	}
}

bool Player::Attack()
{
	//std::list<Goblin*>gobs・・・
	auto gobs = FindGameObjects<Goblin>();
	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
	VECTOR3 p = VECTOR3(0, 0, 0) * m;
	VECTOR3 edge = VECTOR3(0, -100, 0) * m;
	for (auto g : gobs) {
		g->PlayerAttack(edge,p);
	}
	return false;
}

PlayerNormal::PlayerNormal(Player* parent) :PlayerStateBase(parent)
{

}

PlayerNormal::~PlayerNormal()
{
}

void PlayerNormal::Update()
{
	Camera* cam = FindGameObject<Camera>();
	Pad* pad = FindGameObject<Pad>();
	float lx = pad->LStickX();
	float ly = pad->LStickY();
	if (lx != 0.0f || ly != 0.0f) {
		float angle = atan2f(lx, ly);
		cam = FindGameObject<Camera>();
		VECTOR3 vel;//進むベクトル
		if (pad->IsPushed(pad->DASH)) {
			vel = cam->ForWard() * MGetRotY(angle) * 10.0f;
			cam->Delay(0.50f);
		}
		else {
			vel = cam->ForWard() * MGetRotY(angle) * 5.0f;
		}
		VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(player->rotation.y);//現在の正面ベクトル
		VECTOR3 velNorm = vel.Normalize();
		float c = velNorm.Dot(forward);
		if (c >= cosf(30.0f * DegToRad)) {
			player->position += vel;
			player->rotation.y = atan2f(vel.x, vel.z);
		}
		else {
			VECTOR3 Right = VECTOR3(1, 0, 0) * MGetRotY(player->rotation.y);
			float c = velNorm.Dot(Right);
			if (c > 0) {
				player->rotation.y += 30.0f * DegToRad;
			}
			else {
				player->rotation.y -= 30.0f * DegToRad;
			}
			//player->position += vel;
		}
		
		player->animator->Play(player->Anim_Run);
	}
	//if (CheckHitKey(KEY_INPUT_D)) {
	//	player->rotation.y += 3.0f * DegToRad;
	//}
	//if (CheckHitKey(KEY_INPUT_A)) {
	//	player->rotation.y -= 3.0f * DegToRad;
	//}
	//if (CheckHitKey(KEY_INPUT_W)) {
	//	VECTOR3 velocity;
	//	Camera* cam = FindGameObject<Camera>();
	//	player->animator->Play(player->Anim_Run);
	//	player->position += cam->ForWard() * 3.0f;
	//	if (CheckHitKey(KEY_INPUT_LSHIFT) && player->Stamina > 0) {
	//		player->Stamina--;
	//		velocity = VECTOR3(0, 0, 5 * DASH_SPEED) * MGetRotY(player->rotation.y);
	//		player->position += cam->ForWard() * 5.0f;
	//		player->IsTired = true;
	//	}
	//	//VECTOR3 velocity = VECTOR3(sinf(rotation.y) ,0, cosf(rotation.y))*3.0f;
	//	else {
	//		velocity = VECTOR3(0, 0, 5) * MGetRotY(player->rotation.y);
	//	}
	//	//↑回っていないベクトル*回転行列←すげえ大事
	//	//player->position += velocity;
	//}
	else {
		player->animator->Play(player->Anim_Neutral);
	}
	if ((Input::IsKeyOnTrig(KEY_INPUT_SPACE)|| pad->PushTrigger(pad->JUMP))&& player->OnGround) {
		player->velocityY = sqrt(2 * G * H);
		cam->Delay(0.1);
		player->OnGround = false;
	}
	if (player->velocityY > 0 && !(player->OnGround)) {
		player->animator->Play(player->Anim_JumpInFirst);
	}
	if (player->velocityY < 0 && !(player->OnGround)) {
		player->animator->Play(player->Anim_JumpInLast);
	}
	if (pad->PushTrigger(pad->ATTACK)) {//攻撃
		player->ChangeState(Player::sAttack1);
		
		cam->Delay(0.1);
	}
}

PlayerAttack1::PlayerAttack1(Player* parent) :PlayerStateBase(parent)
{
	player->animator->Play(player->Anim_Attack1);
}

PlayerAttack1::~PlayerAttack1()
{
}

void PlayerAttack1::Update()
{
	
	float frame = player->animator->GetCurrentFrame();
	//frameが3.5までの時にゴブリンに踏み込む
	if (frame < 3.5f) {
		Goblin* g = FindTarget();
		if (g != nullptr) {
			VECTOR3 AttackPos = g->GetPos() - player->GetPos();
			AttackPos = AttackPos.Normalize();
			VECTOR3 playerTarget = g->GetPos() - AttackPos * 100.0f;
			float rate = frame / 3.5f;//0.0~1.0になる
		
			player->position = ((playerTarget-player->position)*rate+player->position);//Lerpのやつだね！
			player->rotation.y = atan2(AttackPos.x, AttackPos.z);
		}
		else {
			VECTOR3 playerTarget = player->position + player->Forward() * 100;
			float rate = frame / 3.5f;//0.0~1.0になる
			player->position = ((playerTarget - player->position) * rate + player->position);//Lerpのやつだね！
		}
	}
	if (frame >= 3.5f && frame <= 8.5f) {
		player->Attack();
	}
	if (player->animator->IsFinish()) {
		player->ChangeState(Player::sNormal);
	}

	else if (player->animator->GetCurrentFrame() >= 8.5f) {
		if (pad->PushTrigger(pad->ATTACK)) {
			player->ChangeState(Player::sAttack2);
		}
	}
}

void PlayerAttack1::AttackGoblin(VECTOR3 attackpos)
{

}

PlayerAttack2::PlayerAttack2(Player* parent) :PlayerStateBase(parent)
{
	player->animator->Play(player->Anim_Attack2);

}

PlayerAttack2::~PlayerAttack2()
{
}

void PlayerAttack2::Update()
{
	player->Attack();
	if (player->animator->IsFinish()) {
		player->ChangeState(Player::sNormal);
	}else if (player->animator->GetCurrentFrame() >= 8.5f) {
		if (pad->PushTrigger(pad->ATTACK)) {
			player->ChangeState(Player::sAttack3);
		}
	}
}

PlayerAttack3::PlayerAttack3(Player* parent) :PlayerStateBase(parent)
{
	player->animator->Play(player->Anim_Attack3);
}

PlayerAttack3::~PlayerAttack3()
{
}

void PlayerAttack3::Update()
{
	player->Attack();
	if (player->animator->IsFinish()) {
		player->ChangeState(Player::sNormal);
	}
	
}

void Player::Draw()
{
	Object3D::Draw();
	//サーベル表示
	int wp = MV1SearchFrame(hModel, "wp");
	MATRIX m = MV1GetFrameLocalWorldMatrix(hModel, wp);
	MV1SetMatrix(hWeapon, m);
	MV1DrawModel(hWeapon);
	MV1SetPosition(hWeapon, VECTOR3(0, 200, 0));
	MV1DrawModel(hWeapon);

	if (not ObjectManager::ShadowDrawing()) {
		VECTOR3 p = VECTOR3(0, 0, 0) * m;
		VECTOR3 edge = VECTOR3(0, -100, 0) * m;
		DrawLine3D(edge, p, GetColor(255, 0, 0));
		DrawSphere3D(position + VECTOR3(0, 60, 0), 50, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
		DrawFormatString(0, 30, GetColor(255, 255, 255), "PLAYER POISTION (%f,%f,%f)", position.x, position.y, position.z);
	}
}

Goblin* PlayerStateBase::FindTarget()
{
	VECTOR3 tmp;
	float LimitDeg =cosf(60.0f*DegToRad);
	float tmpDeg = 0.0f;
	Goblin* ret = nullptr;
	float nearest = 600.0f;
	std::list<Goblin*> goblins = FindGameObjects<Goblin>();
	for (Goblin* g : goblins) {
		//近いもので60°（視野内）のゴブリンを抽出
		tmp = g->GetPos() - player->GetPos();
		tmpDeg = tmp.Dot(player->Forward());
		if (tmp.Size() < nearest&&tmpDeg>LimitDeg) {//角度はcosの値でかえって来てるのでtmpDegのcosの値がDegtoGobよりも大きいほうを採用する
			nearest = tmp.Size();
			ret = g;
		}
	}
	return ret;
}
