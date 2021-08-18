#pragma once
#include <Windows.h>	//HWND

class GraphicObject;
struct object;				//light, wave�� ������ ���� �� ���. object, collider, wave�� ���������� TRS������ ���� �ִ�. �߻�ȭ ���??



class GraphicCollider;
struct collider;			//

class GraphicLight;
struct lightData;

class GraphicWave;
struct waveData;

//__interface�� Ű���� ��ü�� �������� ����. 
class IGraphicsInterface abstract
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsInterface* CreateInstance() = 0;
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight) = 0;		//HWND �߻�ȭ ����?

	//���� ���ο��� ��ü ������Ʈ�� �׸��� ��� �䱸
	virtual void Draw() = 0;	 

	//Create Graphics Instance
	virtual GraphicObject*   CreateGraphicObject(object* pObj) = 0;			
	virtual GraphicCollider* CreateGraphicCollider(collider* pCollider) = 0;
	virtual GraphicLight*    CreateGraphicLight(lightData* pLight) = 0;
	virtual GraphicWave*     CreateGraphicWave(waveData* pWave) = 0;

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y) = 0;

	//���� ������ ���� ����.
	virtual void LoadModel() = 0;
	virtual void LoadMaterial() = 0;
};