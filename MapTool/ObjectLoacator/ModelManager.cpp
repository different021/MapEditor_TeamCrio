#include "pch.h"
#include "ModelManager.h"
#include "utility/HGUtility.h"

ModelManager* ModelManager::m_pInstance = NULL;
int ModelManager::m_InsCount = 0;
HEngine_DX12_3D* ModelManager::m_pEngine = NULL;

void ModelDelete(MODEL*& pModel)
{
	if (pModel == nullptr) return;

	pModel->hModel->Delete();
	pModel->fileName.clear();
	delete pModel;
	pModel = NULL;
}

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
	ClearList();
}

ModelManager* ModelManager::GetModelManager()
{
	if (m_pInstance == NULL)
		m_pInstance = new ModelManager;
	
	if (m_pEngine == NULL)
		m_pEngine = HEngine_DX12_3D::GetInstance();

	m_InsCount++;
	return m_pInstance;
}


void ModelManager::DeleteModelManager()
{
	m_InsCount--;
	if (m_InsCount <= 0)
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
}

void ModelManager::ClearList()
{
	std::vector<MODEL*>::iterator it;
	for (it = m_ModelList.begin(); it != m_ModelList.end();)
	{
		MODEL* pTemp = (*it);

		ModelDelete(pTemp);
		//pTemp->fileName.Empty();
		//pTemp->hModel->Delete();
		//pTemp->fileName.clear();
		//delete pTemp;
		//pTemp = NULL;
		it = m_ModelList.erase(it);
	}
}

wchar_t* ModelManager::MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend)
{
	dest = path + name + L"/" + name + tex + extend;
	return dest.GetBuffer();
}

static int g_INDEX = 0;

//MODEL* ModelManager::LoadModelData(wchar_t* path, wchar_t* name, int texIndex)
//{
//	if (texIndex < 0)
//		__debugbreak();
//
//	//MODEL* pModel = NULL;
//	//wchar_t fullPathW[MAX_PATH];
//	//char fullPathA[MAX_PATH];
//	//wmemset(fullPathW, L'\0', MAX_PATH);
//	//memset(fullPathA, '\0', MAX_PATH);
//
//	//HGUtility::MakeFullPath(path, name, MAX_PATH, fullPathW);
//	//HGUtility::ConvertWideCharToMultiByte(fullPathW, fullPathA, MAX_PATH - 1);
//	//int a = 0;
//
//
//	//std::vector<std::string> strList;
//	//strList.push_back(std::string(fullPathW));		//ÀÌ°Ô ¹¼´Ï±î 
//
//	//BOOL bResult = FALSE;
//	//HModelData* hModel = m_pEngine->CreateModel(strList.at(0));
//	//if (hModel != NULL)
//	//{
//	//	pModel = new MODEL;
//	//	pModel->fileName = name;
//	//	pModel->index = g_INDEX;
//	//	pModel->textureIndx = texIndex;
//	//	pModel->hModel = hModel;
//	//	g_INDEX++;
//	//	m_ModelList.push_back(pModel);
//
//	//	bResult = TRUE;
//	//}
//
//	return NULL;
//
//}

MODEL* ModelManager::LoadModel(std::string fileName, std::string fullPath)
{
	MODEL* pModel = NULL;
	HModelData* hModel = m_pEngine->CreateModelFromHModelFile(fullPath);
	if (hModel != NULL)
	{
		pModel = new MODEL;
		pModel->fileName	= fileName;
		pModel->index		= g_INDEX;
		//pModel->textureIndx = texIndex;
		pModel->hModel		= hModel;
		g_INDEX++;
		m_ModelList.push_back(pModel);
	}

	return pModel;
}

MODEL* ModelManager::LoadModelFromHModel(CString path, CString name, int texIndex)
{
	if (texIndex < 0)
	{
		__debugbreak();
		return NULL;
	}
	else
	{
		MODEL* pModel = NULL;
		CStringW floder;
		AfxExtractSubString(floder, name, 0, L'_');
		CStringA fullPath = path + "/" + floder + "/" + name + ".hmodel";

		std::vector<std::string> strList;
		strList.push_back(std::string(fullPath));

		bool bIsExist = HGUtility::IsFileExists(fullPath.GetBuffer());
		if (!bIsExist)
		{
			wchar_t buf[256];
			wmemset(buf, 0, 256);
			wsprintfW(buf, L"[FAIL]MODEL LOAD :%s", name.GetBuffer());
			::MessageBoxW(NULL, buf, L"[MODEL] Load FAIL", MB_OK);
			return FALSE;
		}

		BOOL bResult = FALSE;
		HModelData* hModel = m_pEngine->CreateModelFromHModelFile(strList.at(0));
		if (hModel != NULL)
		{
			pModel = new MODEL;
			pModel->fileName = CStringA(floder);
			pModel->index = g_INDEX;
			//pModel->textureIndx = texIndex;
			pModel->hModel = hModel;
			g_INDEX++;
			m_ModelList.push_back(pModel);

			bResult = TRUE;
		}

		fullPath.Empty();

		std::vector<std::string>::iterator it;
		for (it = strList.begin(); it != strList.end();)
		{
			it = strList.erase(it);
		}

		//texIndex++;
		return pModel;
	}

}

MODEL* ModelManager::LoadModelFromHModel(CString fullPathName, CString filename)
{
	MODEL* pModel = NULL;
	CStringA fullPath(fullPathName);
	CStringA name(filename);
	fullPath.Replace("\\", "/");

	std::vector<std::string> strList;
	strList.push_back(std::string(fullPath));

	BOOL bResult = FALSE;
	m_pEngine->StartSetting();
	HModelData* hModel = m_pEngine->CreateModelFromHModelFile(strList.at(0));
	m_pEngine->FinishSetting();
	if (hModel != NULL)
	{
		pModel = new MODEL;
		pModel->fileName = name;
		pModel->index = g_INDEX;
		//pModel->textureIndx = 0;
		pModel->hModel = hModel;
		g_INDEX++;
		m_ModelList.push_back(pModel);

		bResult = TRUE;
	}

	return pModel;
}

MODEL* ModelManager::LoadModelFromFBX(CString fullPathName, CString filename)
{

	MODEL* pModel = NULL;
	CStringA fullPath(fullPathName);
	CStringA name(filename);
	fullPath.Replace("\\", "/");

	std::vector<std::string> strList;
	strList.push_back(std::string(fullPath));

	BOOL bResult = FALSE;
	m_pEngine->StartSetting();
	HModelData* hModel = m_pEngine->CreateModelFromFbxFile(strList.at(0));
	m_pEngine->FinishSetting();
	if (hModel != NULL)
	{
		pModel = new MODEL;
		pModel->fileName = name;
		pModel->index = g_INDEX;
		//pModel->textureIndx = 0;
		pModel->hModel = hModel;
		g_INDEX++;
		m_ModelList.push_back(pModel);

		bResult = TRUE;
	}

	return pModel;
}

BOOL ModelManager::LoadModelFromString(std::vector<std::string>& strList)
{
	static int INDEX = 0;

	BOOL bResult = FALSE;
	HModelData* hModel = m_pEngine->CreateModelFromFbxFile(strList.at(0));
	if (hModel != NULL)
	{
		CStringA str = strList[0].c_str();
		CStringA fileName = str.Mid(str.ReverseFind('/') + 1);

		MODEL* pModel = new MODEL;
		pModel->fileName = fileName;
		pModel->index = INDEX;
		pModel->hModel = hModel;
		INDEX++;
		m_ModelList.push_back(pModel);

		bResult = TRUE;

		str.Empty();
		fileName.Empty();
	}

	return bResult;
}

//return -1 When Can't find Model byName
int ModelManager::GetIndexByModelName(wchar_t* szModelName)
{
	int iResult = -1;
	std::vector<MODEL*>::iterator it;
	for (it = m_ModelList.begin(); it != m_ModelList.end(); it++)
	{
		char* pBuffer = NULL;
		HGUtility::Convert_Unicode_To_MultiByte(pBuffer, szModelName);
		MODEL* pModel = *it;
		CStringA aName = CStringA(pBuffer);
		//int isSame = pModel->fileName.Compare(aName);
		//int isSame = pModel->fileName;
		
		if (pModel->fileName.compare(pBuffer) == 0)
		{
			iResult = pModel->index;
			break;
		}
		delete[] pBuffer;
	}
	return iResult;
}

MODEL* ModelManager::GetModel(int index)
{
	MODEL* pResult = NULL;
	std::vector<MODEL*>::iterator it;
	for (it = m_ModelList.begin(); it != m_ModelList.end(); it++)
	{
		MODEL* pModel = *it;
		if (pModel->index == index)	pResult = pModel;
	}

	return pResult;
}

std::vector<MODEL*>* ModelManager::GetModelList()
{
	return &m_ModelList;
}

