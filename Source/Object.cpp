#include "Object.h"
#include"Stage.h"
#include<assert.h>
Object::Object()
{
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 300, -400), VECTOR3(0, 250, 0));
	hModel = MV1LoadModel("data/models/Character/RedGoblin/RedGoblin.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
	velocity = VECTOR3(0, 0, 0);
}

Object::~Object()
{
}

void Object::Update()
{
	Stage* stage = FindGameObject<Stage>();
	VECTOR3 hitPos;
	stage->CollideRay(position + VECTOR3(0, 1000, 0), position + VECTOR3(0, -1000, 0), &hitPos);
	if (stage) {
		position = hitPos;
	}
}
