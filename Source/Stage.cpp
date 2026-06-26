#include "Stage.h"
#include<assert.h>
#include<fstream>
#include"Player.h"
#include"Goblin.h"
#include"Object.h"
#include"StageObject.h"
#include"../ImGui/imgui.h"
#define DEBUG_MODE false //コメントつけるより、DEBUG_MODEを使うことで効率よく開発していますよとアピールできる

Stage::Stage()
{
	hModel = MV1LoadModel("data/models/Stage/Stage00.mv1");
	assert(hModel > 0);
	MV1SetupCollInfo(hModel, -1);
	ReadMappingData(0);
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	MV1DrawModel(hModel);
}

bool Stage::CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit)
{
	bool ret = false;
	VECTOR3 tmpPos=end;//今までで一番startに近いもの
	MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(hModel, -1, start, end);
	if (result.HitFlag > 0) {
		tmpPos = result.HitPosition;
		if (hit != nullptr) {
			*hit = result.HitPosition;
		}
		ret = true;
	}
	 std::list<StageObject*> objs=FindGameObjects<StageObject>();
	 for (StageObject* ob : objs) {
		 VECTOR3 hitPos;
		 bool res = ob->CollideRay(start, end, &hitPos);
		 if (res) {
			 //tmpPosと、hitPosで一番近いほうにtmpPosに代入
			 float tmpFar = VSize(tmpPos - start);
			 float hitFar = VSize(hitPos - start);
			 if (hitFar < tmpFar) {
				 tmpPos = hitPos;
			 }
			 ret = res;
		 }
		 if (ret && hit != nullptr) {
			 *hit = tmpPos;
		 }
	 }
	return ret;
}

VECTOR3 Stage::CollideSphere(VECTOR3 center, float radius)
{
	VECTOR3 ret = VECTOR3{0,0,0};
	bool sg = false;
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(hModel, -1, center, radius);
	for (int i = 0;i < result.HitNum;i++) {
		//result.Dim[i].Normal;//当たったポリゴンの法線
		//Normalが上を向いているのでcontinue
		if (result.Dim[i].Normal.y > cosf(30.0f * DegToRad)) {
			continue;
		}
	//TODO:横向きの壁にはあてるけど、上向きの壁にはアテナイ
		VECTOR3 pushDir = center-result.Dim[i].HitPosition ;
		float dist = pushDir.Size();//中心から壁までの距離
		VECTOR3 push = pushDir.Normalize() * (radius- dist);//押し戻すベクトル
		if (ret.Size() < push.Size()) {
			ret=push;//TODO:複数のポリゴンのうち一番長いのを返すべき
		}
	}
	MV1CollResultPolyDimTerminate(result);
	//StageObjectも観る
	std::list<StageObject*> objs = FindGameObjects<StageObject>();
	for (StageObject* ob : objs) {
		VECTOR3 push=ob->CollideSphere(center,radius);
		if (ret.Size() < push.Size()) {
			ret = push;//TODO:複数のポリゴンのうち一番長いのを返すべき
		}
	}
	return ret;
}

void Stage::ReadMappingData(int stageNo)
{
	
	struct Header {
		char chunk[4]; // "MAPD"
		int CharaInfoNum; // キャラデータの数
		int ObjectInfoNum; // オブジェクトデータの数
		int EventInfoNum; // イベントデータの数
		VECTOR PlayerPosition; // プレイヤー座標
		float PlayerAngle; // プレイヤーのY軸回転
		int RenderType; // レンダリング方法
		int ClearCondition; // クリア条件
		int killCharaNum; // 倒すべき敵の数
		int killTargetChara; // 倒すべき敵の番号
	};
	struct CharaInfo {
		int id; // 種類(1:Goblin, 2:Bee, 3:Golem, 4:RedGoblin
		VECTOR position; // 座標
		float angle; // Y軸回転
	};
	struct ObjectInfo {
		int id; // 種類
		VECTOR position; // 座標
		VECTOR rotation; // 回転
		VECTOR scale; // 拡縮
	};
	struct EventInfo {
		int type; // 種類
		VECTOR position; // 中心座標
		VECTOR area; // 範囲
		int bgm; // 曲番号
		int objectNum; // オブジェクトの数
		int object[8]; // オブジェクトの番号
	};
	std::ifstream ifs("data/models/Stage/Stage00.dat",std::ios::binary);
	assert(ifs);
	Header h;
	ifs.read((char*)&h, sizeof(h));
	new Player(h.PlayerPosition, h.PlayerAngle);
	for (int c = 0;c < h.CharaInfoNum;c++) {
		CharaInfo ci;
		ifs.read((char*)&ci, sizeof(ci));
		/*switch (ci.id) {
		case 1:
			new Goblin(ci.position, ci.angle);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			new Object(ci.position, ci.angle);
			break;
		}*/
		new Goblin(ci.position,ci.angle);
	}
#if !DEBUG_MODE
	for (int c = 0;c < h.ObjectInfoNum;c++) {
		ObjectInfo oi;
		ifs.read((char*)&oi, sizeof(oi));
		
		new StageObject(oi.id, oi.position, oi.rotation, oi.scale);
	}
#endif
	ifs.close();
#if DEBUG_MODE
	new Goblin(VECTOR3(80, 103, 440), 0);
#endif
}
