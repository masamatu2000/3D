#include "PlayScene.h"
#include"Player.h"
#include"Stage.h"
#include"Skelton.h"
namespace {
	const int SkeltonAppear = 600;
	int GAME_CLEAR_TIMER = 3600;
	Skelton* skelton=nullptr;
	Skelton* skelton2 = nullptr;
}
PlayScene::PlayScene()
{
	new Player();
	new Stage();
	//new Skelton();
	timer = 0;
	mode = READY;
	IsSpawned = false;
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
			skelton=new Skelton();
			mode = PLAY;
			timer = 0;
		}
	}
	if (mode == PLAY) {
		if (timer < GAME_CLEAR_TIMER) {
			timer++;
			DrawFormatString(1024 / 2, 0, GetColor(255, 255, 255), "ƒQ[ƒ€ƒNƒŠƒA‚Ü‚Å‚ ‚Æ%d•b", (GAME_CLEAR_TIMER - timer) / 60);
			if (timer >= GAME_CLEAR_TIMER) {
				SceneManager::ChangeScene("GAME CLEAR");
			}
			else if (!IsSpawned && timer >= GAME_CLEAR_TIMER / 2) {
				skelton2 = new Skelton(VECTOR3{2250,642,-3072}, 6, timer);
				IsSpawned = true;	
			}
		}

	}
	DrawString(1024 , 1024/2, "R->ƒJƒƒ‰”½“]", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 50, "W->‘Oi,AorD->•ûŒü“]Š·", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 100, "SPACE->ƒWƒƒƒ“ƒv", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 150, "W+LSHIT->ƒ_ƒbƒVƒ…", GetColor(255, 255, 255));
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawString(0, 50, "Push [T]Key To Title", COL_WHITE);
}
