#include "Game Clear.h"

GameClear::GameClear()
{
	position = VECTOR3(200, 400, 0);
	startX = 800;
	endX = 200;
	maxTime = 5.0f;
	time = 0.0f;
}

GameClear::~GameClear()
{
}

void GameClear::Update()
{
	if (Input::IsKeyOnTrig(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (Input::IsKeyOnTrig(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void GameClear::Draw()
{
	int fsize = GetFontSize();
	SetFontSize(fsize * 5);
	DrawString(400, 384, "GAME CLEAR", COL_YELLOW);
	SetFontSize(fsize);
	DrawFormatString(0, 50, COL_WHITE, "CoG: %s", Version());
	DrawFormatString(0, 75, COL_WHITE, "DxLib: %s", DXLIB_VERSION_STR_T);
	DrawFormatString(0, 100, COL_WHITE, "FPS: %4.1f", 1.0f / Time::DeltaTime());
	DrawString(450,500 , "Push [P]Key To Continue...", COL_WHITE);

}
