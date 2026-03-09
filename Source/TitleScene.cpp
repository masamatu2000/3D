#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (Input::IsKeyOnTrig(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (Input::IsKeyOnTrig(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	int font = GetFontSize();
	SetFontSize(font*2);
	DrawString(1024/2, 1024/2, "ä[çúÇ∆ãSÇ≤Ç¡Ç±ÅIÅI", COL_WHITE);
	SetFontSize(font);
	DrawFormatString(0, 50, COL_WHITE, "CoG: %s", Version() );
	DrawFormatString(0, 75, COL_WHITE, "DxLib: %s", DXLIB_VERSION_STR_T );
	DrawFormatString(0, 100, COL_WHITE, "FPS: %4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 300, "1ï™ä‘ä[çúÇ©ÇÁì¶Ç∞êÿÇÍÇŒèüÇøÅIí«Ç¢Ç¬Ç©ÇÍÇΩÇÁïâÇØ", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [P]Key To Play", COL_WHITE);
}
