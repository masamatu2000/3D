#include "TitleScene.h"
#include"../ImGui/imgui.h"
#include"Pad.h"
#include<fstream>

TitleScene::TitleScene()
{
	//テキストファイルは0Aを0D0Aに変えて保存する
	std::ofstream fs("data/test.bin",std::ios::binary);//ファイルを開く
	struct STR{
		int i1;
		int i2;
	};
	STR st;
	st.i1 = 1000;
	st.i2 = 500;
	fs.write((char*)&st, sizeof(st));
	fs.close();//ファイルを閉じる
	std::ifstream fr("data/test.bin");//ファイルを開く
	STR str2;
	int y;
	fr.read((char*)&str2, sizeof(str2));
	fr.close();

	const char* s = "ABC\nDEF";
	std::ofstream ff("data/moji.bin",std::ios::binary);
	ff.write(s, strlen(s)+1);
	ff.close();
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
	/*Pad* pad = FindGameObject<Pad>();
	if (pad->LStickX()>=0.9f) {
		SceneManager::ChangeScene("PLAY");
	}*/
	Pad* pad = FindGameObject<Pad>();
	pad->LStickX();
}


void TitleScene::Draw()
{
	int font = GetFontSize();
	SetFontSize(font*2);
	DrawString(1024/2, 1024/2, "骸骨と鬼ごっこ！！", COL_WHITE);
	SetFontSize(font);
	DrawFormatString(0, 50, COL_WHITE, "CoG: %s", Version() );
	DrawFormatString(0, 75, COL_WHITE, "DxLib: %s", DXLIB_VERSION_STR_T );
	DrawFormatString(0, 100, COL_WHITE, "FPS: %4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 300, "1分間骸骨から逃げ切れば勝ち！追いつかれたら負け", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [P]Key To Play", COL_WHITE);
}
