#pragma once
#include "GraphicsInterface.h"      //Interface
#include "EngineInterface.h"        //GraphicEngine

//OS : Windows
//Grpahic API : DirectX12
//
//additional Graphic features : 
//1. raytracing (dx 12)
//2. pbr
//3. 환경 엠비언트 오쿨루젼
//4. 
class ToolGraphicsEngine :
    public IGraphicsInterface
{
private:
	HEngine_DX12_3D* m_pEngine;			//그래픽 엔진
	int  m_iWidth = 0;
	int  m_iHeight = 0;
	HWND m_hTarget = NULL;				//윈도우 

private:
	ToolGraphicsEngine();
	~ToolGraphicsEngine();
	void cleanup();

public:
	
	virtual IGraphicsInterface* CreateInstance();							/// Caution! Not Singleton. When call this func, make it's instance.;							
	virtual bool Initialize(HWND hTargetWnd, int iWidth, int iHeight);		//Windows용.

	//엔진 내부에서 전체 오브젝트를 그리는 기능 요구
	virtual void Draw();

	//Create Graphics Instance
	virtual GraphicObject*	 CreateGraphicObject(object* pObj);
	virtual GraphicCollider* CreateGraphicCollider(collider* pCollider);
	virtual GraphicLight*	 CreateGraphicLight(lightData* pLight);
	virtual GraphicWave*	 CreateGraphicWave(waveData* pWave);

	//Picking
	virtual GraphicObject* PickingObject(unsigned int x, unsigned int y);

	//아직 디자인 되지 않음.
	virtual void LoadModel();
	virtual void LoadMaterial();
};

