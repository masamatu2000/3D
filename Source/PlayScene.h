#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
private: 
	int timer;
	enum Mode {
		READY,
		PLAY,
		END
	};
	int mode;
	bool IsSpawned;
};
