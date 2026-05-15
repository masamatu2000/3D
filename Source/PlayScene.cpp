#include "PlayScene.h"
#include"Player.h"
#include"Stage.h"
#include"Skelton.h"
#include"Object.h"
#include"Fighter.h"
#include"Goblin.h"
#include"Camera.h"
namespace {
	const int SkeltonAppear = 600;
	int GAME_CLEAR_TIMER = 3600;
	Skelton* skelton=nullptr;
	Skelton* skelton2 = nullptr;
	const int SKELTON_NUM = 3;
}
PlayScene::PlayScene()
{
	new Player(VECTOR3(0,0,0),0);
	new Goblin(VECTOR3(0, 100, 300), 180 * DegToRad);
	new Goblin(VECTOR3(300, 100, 300), 180 * DegToRad);
	new Stage();
	new Camera();
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
	/*switch (mode)
	{
	case READY:
		Ready();
		break;
	case PLAY:
		Play();
		break;
	}
	DrawString(1024, 1024 / 2, "R->ƒJƒƒ‰”½“]", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 50, "W->‘Oi,AorD->•ûŒü“]Š·", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 100, "SPACE->ƒWƒƒƒ“ƒv", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 150, "W+LSHIT->ƒ_ƒbƒVƒ…", GetColor(255, 255, 255));
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}*/
	/*if (timer <SkeltonAppear&&mode==READY) {
		timer++;
		float fsize = GetFontSize();
		SetFontSize(fsize * 2);
		DrawString(1024 / 2, 1024 / 2, "Š[œ‚©‚ç“¦‚°‚ëII", GetColor(255, 0, 0));
		DrawFormatString(1024 / 2,0, GetColor(255, 255, 255), "Š[œoŒ»‚Ü‚ÅŒã%d•b", ((SkeltonAppear - timer )/ 60));
		SetFontSize(fsize);
		if (timer >= SkeltonAppear) {
			new Skelton();
			for (int i = 0; i < SKELTON_NUM; i++) {
				new Skelton(VECTOR3{ float(GetRand(3000) + 1000), float(0), float(GetRand(-3000)-1000)},0);
				new Skelton(VECTOR3{ float(GetRand(-3000) -1000), float(0), float(GetRand(3000) + 1000) },0);
				new Skelton(VECTOR3{ float(GetRand(-3000) - 1000), float(0), float(GetRand(-3000) - 1000) }, 0);
			}
			mode = PLAY;
			timer = 0;
			new Fighter(VECTOR3{ float(GetRand(3000) + 1000), float(0), float(GetRand(-3000) - 1000) });
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
				new Object(VECTOR3{2250,642,-3072},timer);
				IsSpawned = true;	
			}
			if (IsSpawned&&timer<(GAME_CLEAR_TIMER/2)+60*5) {
				DrawString(1024 / 2, 50, "‹S‚ª’Ç‰Á‚³‚ê‚½‚¼I", GetColor(255, 0 ,0));
			}
		}

	}
	DrawString(1024 , 1024/2, "R->ƒJƒƒ‰”½“]", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 50, "W->‘Oi,AorD->•ûŒü“]Š·", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 100, "SPACE->ƒWƒƒƒ“ƒv", GetColor(255, 255, 255));
	DrawString(1024, 1024 / 2 + 150, "W+LSHIT->ƒ_ƒbƒVƒ…", GetColor(255, 255, 255));
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}*/
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawString(0, 50, "Push [T]Key To Title", COL_WHITE);
}

void PlayScene::Ready()
{
	timer++;
	float fsize = GetFontSize();
	SetFontSize(fsize * 2);
	DrawString(1024 / 2, 1024 / 2, "Š[œ‚©‚ç“¦‚°‚ëII", GetColor(255, 0, 0));
	DrawFormatString(1024 / 2, 0, GetColor(255, 255, 255), "Š[œoŒ»‚Ü‚ÅŒã%d•b", ((SkeltonAppear - timer) / 60));
	SetFontSize(fsize);
	if (timer >= SkeltonAppear) {
		new Skelton();
		for (int i = 0; i < SKELTON_NUM; i++) {
			new Skelton(VECTOR3{ float(GetRand(3000) + 1000), float(0), float(GetRand(-3000) - 1000) }, 0);
			new Skelton(VECTOR3{ float(GetRand(-3000) - 1000), float(0), float(GetRand(3000) + 1000) }, 0);
			new Skelton(VECTOR3{ float(GetRand(-3000) - 1000), float(0), float(GetRand(-3000) - 1000) }, 0);
		}
		mode = PLAY;
		timer = 0;
		new Fighter(VECTOR3{ float(GetRand(3000) + 1000), float(0), float(GetRand(-3000) - 1000) });
	}
}

void PlayScene::Play()
{
	if (timer < GAME_CLEAR_TIMER) {
		timer++;
		DrawFormatString(1024 / 2, 0, GetColor(255, 255, 255), "ƒQ[ƒ€ƒNƒŠƒA‚Ü‚Å‚ ‚Æ%d•b", (GAME_CLEAR_TIMER - timer) / 60);

		if (timer >= GAME_CLEAR_TIMER) {
			SceneManager::ChangeScene("GAME CLEAR");
		}
		else if (!IsSpawned && timer >= GAME_CLEAR_TIMER / 2) {
			new Object(VECTOR3{ 2250,642,-3072 }, timer);
			IsSpawned = true;
		}
		if (IsSpawned && timer < (GAME_CLEAR_TIMER / 2) + 60 * 5) {
			DrawString(1024 / 2, 50, "‹S‚ª’Ç‰Á‚³‚ê‚½‚¼I", GetColor(255, 0, 0));
		}
	}
}

