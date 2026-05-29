#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
	scale = VECTOR3(1.0f,1.0f,1.0f);
}

Object3D::~Object3D()
{
	if (hModel > 0) {
		MV1DeleteModel(hModel);
		hModel = -1;
	} 
}

void Object3D::Draw()
{
	MV1SetRotationXYZ(hModel, rotation);
	MV1SetPosition(hModel, VGet(position.x, position.y, position.z));
	MATRIX mRotY= MGetRotY(rotation.y);
	MATRIX mRotX = MGetRotX(rotation.x);
	MATRIX mRotZ = MGetRotZ(rotation.z);
	MATRIX mTrans = MGetTranslate(position);
	MATRIX mScale = MGetScale(scale);
	MATRIX mat=mScale*mRotZ*mRotX*mRotY*mTrans;
	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);
	
}