#pragma once
#include "Object3D.h"
#include "Animator.h"
class Capsule {
public:
	VECTOR3 top;
	VECTOR3 bottom;
	float radius;
	/// <summary>
	/// カプセルのコンストラクタ
	/// </summary>
	/// <param name="p1">カプセルの上の座標</param>
	/// <param name="p2">カプセルの下の座標</param>
	/// <param name="rad">線分の半径</param>
	Capsule(VECTOR3 p1, VECTOR3 p2, float rad) {
		top = p1;
		bottom = p2;
		radius = rad;
	}
};
/// <summary>
/// ゴブリンのクラス
/// author akimotomasaki
/// </summary>
class Goblin : public Object3D
{
public:
	/// <summary>
	/// ゴブリンのコンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="rotY">向き（ラジアン）</param>
	Goblin(VECTOR3 pos, float rotY);
	~Goblin();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// プレイヤーからの攻撃を受ける
	/// </summary>
	/// <param name="pos1">線分(剣先)の点1</param>
	/// <param name="pos2">線分（剣の根元）の点２</param>
	/// <returns>当たったらtrue</returns>
	bool PlayerAttack(VECTOR3 pos1,VECTOR3 pos2);
private:
	Capsule* cap;
	Animator* animator;
	VECTOR3 spawnPoint;
	enum State {
		sNormal, // 通常状態
		sDamage, // ダメージ
		sGoBack,//帰る
		sChase,//プレイヤーを追う
	};
	State state;
	void UpdateNormal();
	void UpdateDamage();
	void UpdateChase();
	void UpdateGoBack();
	enum AnimID {
		aStand,
		aDamage,
		aRun,
	};
};

// 問題
// プレイヤーが持った剣先の座標を求め方