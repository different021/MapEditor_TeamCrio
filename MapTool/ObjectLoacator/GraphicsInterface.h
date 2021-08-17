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

//__interface�� Ű���� ��ü�� �������� ����.
__interface IGraphicsInterface
{
public:
	//Create/Initialize Interface Instance
	virtual IGraphicsInterface* CreateInstance();
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight);		//HWND �߻�ȭ ����?

	//���� ���ο��� ��ü ������Ʈ�� �׸��� ��� �䱸
	virtual void Draw();	

	//Create Graphics Instance
	virtual GraphicObject* CreateGraphicObject(object* pObj);			//
	virtual GraphicLight*  CreateGraphicLight(light* pLight);
	virtual GraphicWave*   CreateGraphicWave(wave* pWave);

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y);

	//���� ������ ���� ����.
	virtual void LoadModel();
	virtual void LoadMaterial();
};