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
		if (timer >= SkeltonAppear) {
			new Skelton();
			mode = PLAY;
		}
	}
	if (Input::IsKeyOnTrig(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", COL_WHITE);
	DrawString(100, 400, "Push [T]Key To Title", COL_WHITE);
}
