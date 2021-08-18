#pragma once
#include "GraphicsInterface.h"      //Interface
#include "EngineInterface.h"        //GraphicEngine

//OS : Windows
//Grpahic API : DirectX12
//
//additional Graphic features : 
//1. raytracing (dx 12)
//2. pbr
//3. ȯ�� �����Ʈ �������
//4. 
class ToolGraphicsEngine :
    public IGraphicsInterface
{
private:
	HEngine_DX12_3D* m_pEngine;			//�׷��� ����
	int  m_iWidth = 0;
	int  m_iHeight = 0;
	HWND m_hTarget = NULL;				//������ 

private:
	ToolGraphicsEngine();
	~ToolGraphicsEngine();
	void cleanup();

public:
	
	virtual IGraphicsInterface* CreateInstance();							/// Caution! Not Singleton. When call this func, make it's instance.;							
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight);		//Windows��.

	//���� ���ο��� ��ü ������Ʈ�� �׸��� ��� �䱸
	virtual void Draw();

	//Create Graphics Instance
	virtual GraphicObject*	 CreateGraphicObject(object* pObj);
	virtual GraphicCollider* CreateGraphicCollider(collider* pCollider);
	virtual GraphicLight*	 CreateGraphicLight(lightData* pLight);
	virtual GraphicWave*	 CreateGraphicWave(waveData* pWave);

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y);

	//���� ������ ���� ����.
	virtual void LoadModel();
	virtual void LoadMaterial();
};

