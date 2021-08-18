#include "pch.h"
#include "ToolGraphicsEngine.h"
#include "../src/Defines.h"         //object, collider, lightData, waveData ����

ToolGraphicsEngine::ToolGraphicsEngine()
{
}

ToolGraphicsEngine::~ToolGraphicsEngine()
{
}

void ToolGraphicsEngine::cleanup()
{
    m_pEngine->EndEngine();;
    m_iWidth = 0;
    m_iHeight = 0;
    m_hTarget = NULL;
}


IGraphicsInterface* ToolGraphicsEngine::CreateInstance()
{
    //���� �׷��� ������ �̱���..
    //�������̽��� �̱������� �����ϳ�?
    m_pEngine = HEngine_DX12_3D::GetInstance(); 
    ToolGraphicsEngine* pGraphicsInterface = new ToolGraphicsEngine;
    return pGraphicsInterface;
}

bool ToolGraphicsEngine::Initialize(HWND hTargetWnd, int iWidth, int iHeight)
{
    if (m_pEngine == nullptr) return false;
    m_pEngine->InitEngine(hTargetWnd, iWidth, iHeight);

    return true;
}

void ToolGraphicsEngine::Draw()
{
    if (m_pEngine == nullptr) return;
    m_pEngine->Loop();
}

GraphicObject* ToolGraphicsEngine::CreateGraphicObject(object* pObj)
{

    return nullptr;
}
