#pragma once
#include <Windows.h>	//HWND

class IGraphicInstance;
struct object;				//light, wave�� ������ ���� �� ���. object, collider, wave�� ���������� TRS������ ���� �ִ�. �߻�ȭ ���??

class IModel;

class IGraphicCollider;
struct collider;			//

class IGraphicLight;
struct lightData;

class IGraphicWave;
struct waveData;

//__interface�� Ű���� ��ü�� �������� ����. 
class IGraphicsEngine abstract
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsEngine* CreateInstance() = 0;
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight) = 0;		//HWND �߻�ȭ ����?

	//���� ���ο��� ��ü ������Ʈ�� �׸��� ��� �䱸
	virtual void Draw() = 0;	 

	//Create Graphics Instance
	virtual IGraphicInstance*   CreateGraphicObject(object* pObj, IModel* pModel) = 0;
	virtual IGraphicCollider*	CreateGraphicCollider(collider* pCollider) = 0;
	virtual IGraphicLight*		CreateGraphicLight(lightData* pLight) = 0;
	virtual IGraphicWave*		CreateGraphicWave(waveData* pWave) = 0;

	//Picking
	virtual IGraphicInstance* PickingObject(unsigned int x, unsigned int y) = 0;

	//���� ������ ���� ����.
	virtual void LoadModel() = 0;
	virtual void LoadMaterial() = 0;
};