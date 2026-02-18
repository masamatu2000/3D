#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
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
	MV1SetPosition(hModel, position);
	MATRIX mRotY= MGetRotY(rotation.y);
	MATRIX mTrans = MGetTranslate(position);
	MATRIX mat=mRotY*mTrans;
	MV1SetMatrix(hModel, mat);
	MV1DrawModel(hModel);

}
