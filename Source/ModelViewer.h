#pragma once
#include"../Library/SceneBase.h"
class ModelViewer : public SceneBase {
public:
	ModelViewer();
	~ModelViewer();
	void Draw() override;
private:
	int hModel_;
	//VECTOR3 rotation_;
	MATRIX mat;
};