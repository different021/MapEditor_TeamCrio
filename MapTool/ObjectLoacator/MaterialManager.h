#pragma once
#include "../src/Defines.h"
#include "ToolDefine.h"

//�ϴ� ���� �ΰ��� ����.
//���� txt���� �̿��� �ڵ�ȭ
class MaterialManager
{
    static MaterialManager* m_pInstance;
    static int m_InsCount;
	HEngine_DX12_3D* m_Engine = NULL;			//�׷��� �ν��Ͻ� ������ ���ؼ� �ʿ�.
	std::vector<MATERIAL*> m_hMatList;

private:
    MaterialManager();
    ~MaterialManager();

    wchar_t* MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend);      //�ʿ��� ��� ����

    int CreateMaterial(HMaterialData* hMat, CString name);              //�׷��� �ν��Ͻ� ����

public:
    static MaterialManager* GetInstance();
    void DeleteInstance(); 
    void SetGraphicEngine(HEngine_DX12_3D* pEngine);

    void DeleteList();
    std::vector<MATERIAL*>* GetMatList();

    MATERIAL* GetMaterial(int index);
    int GetIndexByMaterialName(wchar_t* szMatName);

    int LoadTexture(CString& path, CString& name, CString& extension);       //�ؽ��� �ε�
    int LoadTexture(CString& name, strTEXTURE* pStrTexture);               
    
};

