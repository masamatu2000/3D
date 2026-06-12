#pragma once
#include"../library/GameObject.h"
#include"Object3D.h"
#include"Animator.h"
class Goblin;
class Player;
class PlayerStateBase {
public:
	PlayerStateBase() = delete;
	PlayerStateBase(Player* parent) { player = parent; }
	~PlayerStateBase() {}
	virtual void Update() {}
	Goblin* FindTarget();
protected:
	Player* player;
};
class PlayerNormal :public PlayerStateBase {
public:
	PlayerNormal(Player* parent);
	~PlayerNormal();
	void Update() override;
};
class PlayerAttack1 :public PlayerStateBase {
public:
	PlayerAttack1(Player* parent);
	~PlayerAttack1();
	void Update() override;
	void AttackGoblin(VECTOR3 attackpos);
};
class PlayerAttack2 :public PlayerStateBase {
public:
	PlayerAttack2(Player* parent);
	~PlayerAttack2();
	void Update() override;
};
class PlayerAttack3 :public PlayerStateBase {
public:
	PlayerAttack3(Player* parent);
	~PlayerAttack3();
	void Update() override;
	
};
class Player : public Object3D
{
	friend class PlayerNormal;
	friend class PlayerAttack1;
	friend class PlayerAttack2;
	friend class PlayerAttack3;
public:
	Player(VECTOR3 pos,float rotY);
	~Player();
	void Update() override;
	VECTOR3 GetPosition() { return position; }
	void DrawStaminaUI();
	bool IsOnFigher();
	void SetCam(bool cam) { IsPlayerCam = cam; }
	void Draw() override;
private:
	Animator* animator;
private:
	float velocityY;
	bool OnGround;
	float Stamina;
	float MaxStamina;
	int timer;
	bool IsTired;
	bool IsPlayerCam;
	bool IsOnFighter;
	enum AnimState {
		Anim_Run,
		Anim_Neutral,
		Anim_JumpInFirst,
		Anim_JumpInLast,
		Anim_Attack1,
		Anim_Attack2,
		Anim_Attack3,
	};
	enum State {
		sNormal,//通常状態
		sAttack1,//攻撃
		sAttack2,
		sAttack3,
	};
	State state;
	PlayerStateBase* stateInst;
	void ChangeState(State st);
	int hWeapon;
	bool Attack();
};
