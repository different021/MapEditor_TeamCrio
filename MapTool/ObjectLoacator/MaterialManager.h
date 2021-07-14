#pragma once
#include "../src/Defines.h"
#include "ToolDefine.h"

//일단 더미 두개씩 진행.
//차후 txt등을 이용한 자동화
class MaterialManager
{
    static MaterialManager* m_pInstance;
    static int m_InsCount;
	HEngine_DX12_3D* m_Engine = NULL;			//그래픽 인스턴스 관리를 위해서 필요.
	std::vector<MATERIAL*> m_hMatList;

private:
    MaterialManager();
    ~MaterialManager();

    wchar_t* MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend);      //필요한 경로 생성

    int CreateMaterial(HMaterialData* hMat, CString name);              //그래픽 인스턴스 생성

public:
    static MaterialManager* GetInstance();
    void DeleteInstance(); 
    void SetGraphicEngine(HEngine_DX12_3D* pEngine);

    void DeleteList();
    std::vector<MATERIAL*>* GetMatList();

    MATERIAL* GetMaterial(int index);
    int GetIndexByMaterialName(wchar_t* szMatName);

    int LoadTexture(CString& path, CString& name, CString& extension);       //텍스쳐 로드
    int LoadTexture(CString& name, strTEXTURE* pStrTexture);               
    
};

