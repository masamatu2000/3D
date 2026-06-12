#include"StageObject.h"
#include<assert.h>
StageObject::StageObject(int id, VECTOR3 pos, VECTOR3 rot, VECTOR3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;
	char str[45];
	assert(sprintf_s<45>(str, "data/models/Stage/Stage_Obj%03d.mv1", id));
	hModel = MV1LoadModel(str);
	assert(sprintf_s<45>(str, "data/models/Stage/Stage_Obj%03d_c.mv1", id));
	hHitModel = MV1LoadModel(str);
	MV1SetupCollInfo(hHitModel, -1);
	MATRIX mRotY = MGetRotY(rotation.y);
	MATRIX mRotX = MGetRotX(rotation.x);
	MATRIX mRotZ = MGetRotZ(rotation.z);
	MATRIX mTrans = MGetTranslate(position);
	MATRIX mScale = MGetScale(scale);
	MATRIX mat = mScale * mRotZ * mRotX * mRotY * mTrans;
	MV1SetMatrix(hHitModel, mat);
}

StageObject::~StageObject()
{
}

void StageObject::Draw()
{
	Object3D::Draw();
}

bool StageObject::CollideRay(VECTOR3 start, VECTOR3 end, VECTOR3* hit)
{
	MV1RefreshCollInfo(hHitModel, -1);
	MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(hHitModel, -1, start, end);
	if (result.HitFlag > 0) {
		if (hit != nullptr) {
			*hit = result.HitPosition;
		}
		return true;
	}
	return false;
}

VECTOR3 StageObject::CollideSphere(VECTOR3 center, float radius)
{
	VECTOR3 ret = VECTOR3{ 0,0,0 };
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(hHitModel, -1, center, radius);
	for (int i = 0;i < result.HitNum;i++) {
		//result.Dim[i].Normal;//当たったポリゴンの法線
		//Normalが上を向いているのでcontinue
		if (result.Dim[i].Normal.y > cosf(30.0f * DegToRad)) {
			continue;
		}
		//TODO:横向きの壁にはあてるけど、上向きの壁にはアテナイ
		VECTOR3 pushDir = center - result.Dim[i].HitPosition;
		float dist = pushDir.Size();//中心から壁までの距離
		VECTOR3 push = pushDir.Normalize() * (radius - dist);//押し戻すベクトル
		if (ret.Size() < push.Size()) {
			ret = push;//TODO:複数のポリゴンのうち一番長いのを返すべき
		}
	}
	MV1CollResultPolyDimTerminate(result);
	return ret;
}
