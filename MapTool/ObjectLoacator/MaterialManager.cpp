#include "pch.h"
#include "MaterialManager.h"
#include "MapUtil.h"
#include "utility/HGUtility.h"

CString strALBEDO = L"_albedo";
CString strROUGHNESS = L"_roughness";
CString strMETALLIC = L"_metallic";
CString strAO = L"_ao";
CString strNORMAL = L"_normal";
CString strHEIGHT = L"_height";

CString strPNG = L".png";
CString strDDS = L".dds";


MaterialManager* MaterialManager::m_pInstance = NULL;
int MaterialManager::m_InsCount = 0;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
	DeleteList();
}

wchar_t* MaterialManager::MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend)
{
	dest = path + name + L"/" + name + tex + extend;
	return dest.GetBuffer();
}

//HMaterialData(그래픽 인스턴스)의 NULL이 허용된다. 이것은 컬러칩을 나타낸다. 
int MaterialManager::CreateMaterial(HMaterialData* hMat, CString name)
{
	static int INDEX = 1;
	BOOL bResult = FALSE;
	MATERIAL* pMat = NULL;
	CStringA nameA = CStringA(name.GetBuffer());

	pMat = new MATERIAL;
	if (pMat != NULL)
	{
		bResult = TRUE;

		pMat->hMat = hMat;
		if (pMat->hMat == NULL)
		{
			//hMat이 NULL일 경우. -> 인덱스 무조건 0(인덱스 0은 컬러칩을 나타낸다.)
			pMat->index = 0;
		}
		else
		{
			pMat->index = INDEX;
			INDEX++;
		}

		pMat->matName = nameA;
		m_hMatList.push_back(pMat);

	}
	else
	{
		MapUtil::DebugString(L"[FAILE]Create Material");
		return -1;
	}

	nameA.Empty();

	return pMat->index;
}

MaterialManager* MaterialManager::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new MaterialManager;
	m_InsCount++;
	return m_pInstance;
}

void MaterialManager::DeleteInstance()
{
	m_InsCount--;
	if (m_InsCount <= 0)
	{
		if (m_pInstance != NULL)
		{
			DeleteList();
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
}

void MaterialManager::SetGraphicEngine(HEngine_DX12_3D* pEngine)
{
	m_Engine = pEngine;
}

void MaterialManager::DeleteList()
{
	//여기
	std::vector<MATERIAL*>::iterator it;
	for (it = m_hMatList.begin(); it != m_hMatList.end();)
	{
		MATERIAL* pMat = (*it);
		pMat->matName.Empty();
		pMat->matName.ReleaseBuffer();
		
		if (pMat->hMat != NULL)
		{
			pMat->hMat->Delete();
		}
		delete pMat;
		pMat = NULL;
		
		it = m_hMatList.erase(it);
	}
}

std::vector<MATERIAL*>* MaterialManager::GetMatList()
{
	return &m_hMatList;
}

MATERIAL* MaterialManager::GetMaterial(int index)
{
	int iMatIndex = index;
	MATERIAL* pResult = NULL;
	std::vector<MATERIAL*>::iterator it;
	for (it = m_hMatList.begin(); it != m_hMatList.end(); it++)
	{
		MATERIAL* pTemp = *it;
		if (pTemp->index == iMatIndex)
		{
			pResult = pTemp;
			break;
		}
	}

	return pResult;
}

int MaterialManager::GetIndexByMaterialName(wchar_t* szMatName)
{
	int iResult = -1;
	std::vector<MATERIAL*>::iterator it;
	for (it = m_hMatList.begin(); it != m_hMatList.end(); it++)
	{
		char* pBuffer = NULL;
		HGUtility::Convert_Unicode_To_MultiByte(pBuffer, szMatName);
		MATERIAL* pModel = *it;
		CStringA aName = CStringA(pBuffer);
		int isSame = pModel->matName.Compare(aName);
		if (isSame == 0)
		{
			iResult = pModel->index;
			break;
		}
		delete[] pBuffer;
	}
	return iResult;

}

int MaterialManager::LoadTexture(CString& path, CString& name, CString& extension)
{
	int index = -1;

	strTEXTURE texset;
	CString tex[eTEX_MAX];

	texset.tex[eAlbedo] = MakeFullPath(tex[eAlbedo], path, name, strALBEDO, extension);
	texset.tex[eRoughness] = MakeFullPath(tex[eRoughness], path, name, strROUGHNESS, extension);
	texset.tex[eMetallic] = MakeFullPath(tex[eMetallic], path, name, strMETALLIC, extension);
	texset.tex[eAo] = MakeFullPath(tex[eAo], path, name, strAO, extension);
	texset.tex[eNormal] = MakeFullPath(tex[eNormal], path, name, strNORMAL, extension);
	texset.tex[eHeight] = MakeFullPath(tex[eHeight], path, name, strHEIGHT, extension);
	index = LoadTexture(name, &texset);

	for (int i = 0; i < eTEX_MAX; i++)
	{
		tex[i].Empty();
	}

	return index;
}



int MaterialManager::LoadTexture(CString& name, strTEXTURE* pStrTexture)
{
	int index = -1;
	HMaterialData* hMat = NULL;

	if (m_Engine == NULL)
	{
		OutputDebugStringW(L"[FAIL] Texture Load : Need to Set the Graphic Engine\n");
	}

	int iCamp = name.CompareNoCase(L"COLORCHIP");		
	if (iCamp == 0)
	{
		m_Engine->LoadColorChip(
			L"Media/Material/colorchip/colorChip_baseColor.png",
			L"Media/Material/colorchip/roughness_baseColor.png",
			L"Media/Material/colorchip/metallic_baseColor.png",
			L"Media/Material/colorchip/colorChip_emissive.png"
		);

		//컬러칩일 경우. -> 메테리얼이 NULL
		hMat = NULL;
	}		
	else
	{
		hMat = m_Engine->CreateMaterial(
			pStrTexture->tex[eAlbedo],
			pStrTexture->tex[eRoughness],
			pStrTexture->tex[eMetallic],
			pStrTexture->tex[eAo],
			pStrTexture->tex[eNormal],
			pStrTexture->tex[eHeight],
			NULL								//emissive
		);
	}

	index = CreateMaterial(hMat, name);

	return index;
}

