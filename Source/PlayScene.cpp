#include "PlayScene.h"
#include"Player.h"
#include"Stage.h"
#include"Skelton.h"
namespace {
	const int SkeltonAppear = 600;
}
PlayScene::PlayScene()
{
	new Player();
	new Stage();
	//new Skelton();
	timer = 0;
	mode = READY;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	
	if (timer <SkeltonAppear&&mode==READY) {
		timer++;
		float fsize = GetFontSize();
		SetFontSize(fsize * 2);
		DrawString(1024 / 2, 1024 / 2, "Š[œ‚©‚ç“¦‚°‚ëII", GetColor(255, 0, 0));
		DrawFormatString(1024 / 2,0, GetColor(255, 255, 255), "Š[œoŒ»‚Ü‚ÅŒã%d•b", ((SkeltonAppear - timer )/ 60));
		SetFontSize(fsize);
		if (timer >= SkeltonAppear) {
			new Skelton();
			mode = PLAY;
		}
	}
	DrawString(1024 , 1024/2, "R->ƒJƒƒ‰”½“]", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 50, "W->‘Oi,AorD->•ûŒü“]Š·", GetColor(255, 255, 255));
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawString(100, 400, "Push [T]Key To Title", COL_WHITE);
}
