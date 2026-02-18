#include "Skelton.h"
#include<assert.h>
Skelton::Skelton()
{
	hModel = MV1LoadModel("data/models/Skelton/Skelton.mv1");
	assert(hModel > 0);
	int root = MV1SearchFrame(hModel, "root");
	MV1SetFrameUserLocalMatrix(hModel, root, MGetRotY(DX_PI_F));
	position = VECTOR3(0, 0, 0);
	rotation = VECTOR3(0, 0, 0);
}

Skelton::~Skelton()
{
}

void Skelton::Update()
{
}
