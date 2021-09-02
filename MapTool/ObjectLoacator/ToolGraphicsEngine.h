#pragma once
#include "IGraphicsEngine.h"      //Interface
#include "EngineInterface.h"        //GraphicEngine

class IModel;

//OS : Windows
//Grpahic API : DirectX12
//
//additional Graphic features : 
//1. raytracing (dx 12)
//2. pbr
//3. ȯ�� �����Ʈ �������
//4. 
class ToolGraphicsEngine :
    public IGraphicsEngine
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
	
	virtual IGraphicsEngine* CreateInstance();							/// Caution! Not Singleton. When call this func, make it's instance.;							
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight);		//Windows��.

	//���� ���ο��� ��ü ������Ʈ�� �׸��� ��� �䱸
	virtual void Draw();

	//Create Graphics Instance
	virtual IGraphicInstance*	CreateGraphicObject(object* pObj, IModel* pModel) override;
	virtual IGraphicCollider*	CreateGraphicCollider(collider* pCollider) override;
	virtual IGraphicLight*		CreateGraphicLight(lightData* pLight) override;
	virtual IGraphicWave*		CreateGraphicWave(waveData* pWave) override;

	//Picking
	virtual IGraphicInstance*  PickingObject(unsigned int x, unsigned int y) override;

	//���� ������ ���� ����.
	virtual void LoadModel() override;
	virtual void LoadMaterial() override;
};

