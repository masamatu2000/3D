#include"Light.h"
#include"Player.h"
Light::Light()
{
}

Light::~Light()
{
}

void Light::Update()
{
	Player* pl = FindGameObject<Player>();
	VECTOR3 ppos = pl->GetPos();
	ppos.y = -1;
	
	SetLightAmbColor(GetColorF(0.8,0.8,0.8,0.0));
	SetLightDirection(VECTOR3(1,-1,0));
	SetLightDifColor(GetColorF(1.0, 0.5, 0.5, 0.0));
	SetLightSpcColor(GetColorF(1.0, 0.5, 0.5, 0.0));
	/*SetFogEnable(TRUE);
	SetFogColor(0,0,0);
	SetFogStartEnd(2.0f,1000.0f);*/
	ObjectManager::SetShadowLight(VECTOR3(1, -1, 0));
}
