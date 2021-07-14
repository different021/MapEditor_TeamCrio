#pragma once
#include "EngineInterface.h"
#include "ToolDefine.h"
#pragma comment(lib,"HEngine.lib")  //Engin Lib

class CamController
{
	HEngine_DX12_3D* m_pEngine;
	Camera* m_pCamera;

	bool m_bCanMove = false;
	bool m_bCanRot  = false;

public:
	CamController();
	~CamController();

	void Update(int curX, int curY, int lastX, int lastY);
	void MoveFront(float fDelta, float deltaTime);
	void MoveRight(float fDelta, float deltaTime);
	void MoveUp(float fDelta, float deltaTime);
	void MoveCam(int curX, int curY, int lastX, int lastY);
	void RotCam(int curX, int curY, int lastX, int lastY);

	void RotationAvailable(bool isRot);
	void MovementAvailable(bool isMove);

};

