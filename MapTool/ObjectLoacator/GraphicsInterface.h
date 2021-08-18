#pragma once
#include <Windows.h>	//HWND

class GraphicObject;
struct object;				//light, wave와 형식을 맞출 것 고려. object, collider, wave는 공통적으로 TRS정보를 갖고 있다. 추상화 고려??



class GraphicCollider;
struct collider;			//

class GraphicLight;
struct lightData;

class GraphicWave;
struct waveData;

//__interface란 키워드 자체가 윈도우즈 전용. 
class IGraphicsInterface abstract
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsInterface* CreateInstance() = 0;
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight) = 0;		//HWND 추상화 가능?

	//엔진 내부에서 전체 오브젝트를 그리는 기능 요구
	virtual void Draw() = 0;	 

	//Create Graphics Instance
	virtual GraphicObject*   CreateGraphicObject(object* pObj) = 0;			
	virtual GraphicCollider* CreateGraphicCollider(collider* pCollider) = 0;
	virtual GraphicLight*    CreateGraphicLight(lightData* pLight) = 0;
	virtual GraphicWave*     CreateGraphicWave(waveData* pWave) = 0;

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y) = 0;

	//아직 디자인 되지 않음.
	virtual void LoadModel() = 0;
	virtual void LoadMaterial() = 0;
};