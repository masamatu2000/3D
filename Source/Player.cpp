#include "Player.h"
#include<assert.h>
#include"Stage.h"
#include"Fighter.h"
namespace {
	const float DASH_SPEED = 3.0f;
	const int STAMINA_HEEL_TIMER = 60 * 3;
	const float G = 3.0f / 60.0f;
	const float H = 64.0f * 3.0f;
	const float LIMIT_FIGHTER_DIS = 50;
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
	
	position.y += velocityY;
	velocityY -= G;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
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

		camPos = camPos * MGetRotY(rotation.y) + position;
		SetCameraPositionAndTarget_UpVecY(camPos, position + VECTOR3(0, 250, 0));
		//DrawFormatString(0, 200, GetColor(255, 255, 255), "(%f,%f,%f)", position.x, position.y, position.z);
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

PlayerNormal::PlayerNormal(Player* parent) :PlayerStateBase(parent)
{

}

PlayerNormal::~PlayerNormal()
{
}

void PlayerNormal::Update()
{
	if (CheckHitKey(KEY_INPUT_D)) {
		player->rotation.y += 3.0f * DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		player->rotation.y -= 3.0f * DegToRad;
	}
	if (CheckHitKey(KEY_INPUT_W)) {
		VECTOR3 velocity;
		
		player->animator->Play(player->Anim_Run);
		if (CheckHitKey(KEY_INPUT_LSHIFT) && player->Stamina > 0) {
			player->Stamina--;
			velocity = VECTOR3(0, 0, 5 * DASH_SPEED) * MGetRotY(player->rotation.y);
			player->IsTired = true;
		}
		//VECTOR3 velocity = VECTOR3(sinf(rotation.y) ,0, cosf(rotation.y))*3.0f;
		else {
			velocity = VECTOR3(0, 0, 5) * MGetRotY(player->rotation.y);
		}
		//↑回っていないベクトル*回転行列←すげえ大事
		player->position += velocity;
	}
	else {
		player->animator->Play(player->Anim_Neutral);
	}
	if (Input::IsKeyOnTrig(KEY_INPUT_SPACE) && player->OnGround) {
		player->velocityY = sqrt(2 * G * H);
		player->OnGround = false;
	}
	if (player->velocityY > 0 && !(player->OnGround)) {
		player->animator->Play(player->Anim_JumpInFirst);
	}
	if (player->velocityY < 0 && !(player->OnGround)) {
		player->animator->Play(player->Anim_JumpInLast);
	}
	if (CheckHitKey(KEY_INPUT_B)) {//攻撃
		player->ChangeState(Player::sAttack1);
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
	
	if (player->animator->GetCurrentFrame() >= 8.5f) {
		if (CheckHitKey(KEY_INPUT_B)) {
			player->ChangeState(Player::sAttack2);
		}
	}
	else if (player->animator->IsFinish()) {
		player->ChangeState(Player::sNormal);
	}

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
	if (player->animator->GetCurrentFrame() >= 8.5f) {
		if (CheckHitKey(KEY_INPUT_B)) {
			player->ChangeState(Player::sAttack3);
		}
	}
	else if (player->animator->IsFinish()) {
		player->ChangeState(Player::sNormal);
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
	VECTOR3 p = VECTOR3(0, 0, 0) * m;
	VECTOR3 edge=VECTOR3(0,-100,0)*m;
	DrawLine3D(edge,p,GetColor(255,0,0));
}
