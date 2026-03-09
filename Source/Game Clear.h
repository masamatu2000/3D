#pragma once
#pragma once
#include"../Library/SceneBase.h"

class GameClear : public SceneBase {
public:
	GameClear();
	~GameClear();
	void Update() override;
	void Draw() override;
private:
	int hImage;
	VECTOR3 position;
	float startX;
	float endX;
	float maxTime;
	float time;
};

