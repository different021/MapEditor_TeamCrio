#pragma once
#include "../src/Defines.h"
#include "ToolDefine.h"

void ModelDelete(MODEL*& pModel);

class ModelManager
{
    static ModelManager* m_pInstance;
    static int m_InsCount;
    static HEngine_DX12_3D* m_pEngine; 		//그래픽 인스턴스 관리를 위해서 필요.
    std::vector<MODEL*> m_ModelList;

    ModelManager();
    ~ModelManager();

public:
    static ModelManager* GetModelManager();
    void DeleteModelManager();
    void ClearList();

    wchar_t* MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend);

    //MODEL* LoadModelData(wchar_t* path, wchar_t* name, int texIndex);
    MODEL* LoadModel(std::string fileName, std::string fullPath);
    MODEL* LoadModelFromHModel(CString path, CString name, int texIndex);             //모델 로드
    MODEL* LoadModelFromHModel(CString fullPathName, CString name);
    MODEL* LoadModelFromFBX(CString fullPathName, CString name);
    BOOL   LoadModelFromString(std::vector<std::string>& strList);

    int    GetIndexByModelName(wchar_t* szModelName);
    MODEL* GetModel(int index);
  
    std::vector<MODEL*>* GetModelList();
};

