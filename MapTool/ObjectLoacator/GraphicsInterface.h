#pragma once
#include <Windows.h>	//HWND

class object;
class GraphicObject;

class GraphicCollider;
struct collider;

class GraphicLight;
class lightData;

class GraphicWave;
class waveData;

//__interface란 키워드 자체가 윈도우즈 전용.
__interface IGraphicsInterface
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsInterface* CreateInstance();
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight);		//HWND 추상화 가능?

	//엔진 내부에서 전체 오브젝트를 그리는 기능 요구
	virtual void Draw();	

	//Create Graphics Instance
	virtual GraphicObject* CreateGraphicObject(object* pObj);			//
	virtual GraphicLight*  CreateGraphicLight(light* pLight);
	virtual GraphicWave*   CreateGraphicWave(wave* pWave);

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y);

	//아직 디자인 되지 않음.
	virtual void LoadModel();
	virtual void LoadMaterial();
};