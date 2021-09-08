#pragma once
#include <Windows.h>	//HWND

//구현 예정
class IGraphicInstance;
struct object;				//light, wave와 형식을 맞출 것 고려. object, collider, wave는 공통적으로 TRS정보를 갖고 있다. 추상화 고려??

//리소스
class IModel;
class IMaterial;

//나누지말고, 인스턴스로 통합 관리.
class IGraphicCollider;
struct collider;			//

class IGraphicLight;
struct lightData;

class IGraphicWave;
struct waveData;

//__interface란 키워드 자체가 윈도우즈 전용. 
class IGraphicsEngine abstract
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsEngine* CreateInstance() = 0;
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight) = 0;		//HWND 추상화 가능?

	//엔진 내부에서 전체 오브젝트를 그리는 기능 요구
	virtual void Draw() = 0;	 

	//Create Graphics Instance
	//virtual IGraphicInstance*	CreateGraphicInstance(eInstanceType type, trs* pTrs); //->각 오브젝트들 구분하지 말까 고민 중.
	virtual IGraphicInstance*   CreateGraphicObject(object* pObj, IModel* pModel) = 0;
	virtual IGraphicCollider*	CreateGraphicCollider(collider* pCollider) = 0;
	virtual IGraphicLight*		CreateGraphicLight(lightData* pLight) = 0;
	virtual IGraphicWave*		CreateGraphicWave(waveData* pWave) = 0;

	//Picking
	virtual IGraphicInstance* PickingObject(unsigned int x, unsigned int y) = 0;

	//아직 디자인 되지 않음.
	virtual void LoadModel() = 0;
	virtual void LoadMaterial() = 0;
};