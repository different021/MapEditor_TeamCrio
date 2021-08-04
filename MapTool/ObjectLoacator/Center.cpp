// MainDlg.cpp: 구현 파일
//
#include "pch.h"
#include "afxdialogex.h"
#include <thread>
#include <string.h>
#include <wchar.h>
#include <future>

#include "MapUtil.h"

//대화상자들					
#include "Center.h"
#include "ObjectLoacator.h"
#include "ViewDlg.h"
#include "ObjectLoacator.h"
#include "CLIGHTDLG.h"
#include "CWaveDlg.h"

//매니저들
#include "CSaver.h"		

#include "ModelManager.h"
#include "MaterialManager.h"
#include "DrawInsManager.h"
#include "CColliderManager.h"
#include "CLightManager.h"
#include "CWaveManager.h"

#include "WMDefine.h"		//윈도우 메세지 정의 헤더



/*
* 대화상자와 매니저를 관리하는 곳을 분리할 수 있을 것 같다.
*/

template <typename T>
inline void safe_delete(T*& p)
{
	delete p;
	p = static_cast<T*>(0);
};

void RequestCenter(UINT msg, WPARAM from, LPARAM para)
{
	::SendMessageW(g_hCenter, msg, from, para);
}

// MainDlg 대화 상자
HWND g_hCenter;
Center* g_pCenter = NULL;


IMPLEMENT_DYNAMIC(Center, CDialogEx)

Center::Center(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DLG, pParent)
{

}


Center::~Center()
{
	//CleanUp();	//-> OnClose()로 이동
}

void Center::Init_Loc()
{
	m_ObjLocator.Create(IDD_OBJECTLOACATOR_DIALOG);
	SeperateWnd_Loc();
	m_ObjLocator.ShowWindow(SW_SHOW);
	//m_ObjLocator.SetCenter(m_hWnd);
}

void Center::SeperateWnd_Loc()
{
	if (m_ObjLocator.IsDocking())
	{
		m_ObjLocator.ModifyStyle(WS_POPUP, WS_CHILD);
		m_ObjLocator.SetParent(this);
	}
	else
	{
		m_ObjLocator.ModifyStyle(WS_CHILD, WS_POPUP | WS_BORDER | WS_CAPTION);
		m_ObjLocator.SetParent(NULL);
	}
}

void Center::Init_Viewer()
{
	m_Viewer = new Viewer;
	m_Viewer->Create(IDD_VIEW_DLG);
	int iFrameX = GetSystemMetrics(SM_CXFRAME);
	int iFrameY = GetSystemMetrics(SM_CYFRAME);
	int iCaptionY = GetSystemMetrics(SM_CYCAPTION);

	int iWidth = 1920 + (iFrameX << 2);
	int iHeight = 1080 + (iFrameY << 2) + iCaptionY;

	m_Viewer->Init(iWidth, iHeight);
	//m_Viewer->GetModelManager(1920, 1080);
	
	SeperateWnd_Viewer();
	m_Viewer->ShowWindow(SW_SHOW);
}

void Center::SeperateWnd_Viewer()
{
	if (m_bIsDockingViewr)
	{
		m_Viewer->ModifyStyle(WS_POPUP, WS_CHILD);
		m_Viewer->SetParent(this);
	}
	else
	{
		m_Viewer->ModifyStyle(WS_CHILD, WS_POPUP | WS_BORDER | WS_CAPTION);
		m_Viewer->SetParent(NULL);
	}
}

void Center::Init_GridController()
{
	m_GridController.Create(IDD_GRIDCON);
	m_GridController.SetParent(this);
	m_GridController.ModifyStyle(WS_POPUP, WS_CHILD);
	m_GridController.ShowWindow(SW_SHOW);
}

void Center::Init_HelpDlg(CWnd* pParent)
{
	//m_HelpDlg = new CHelpDlg;
	m_HelpDlg.Create(IDD_HELP_DLG);
	m_HelpDlg.SetParent(pParent);
	//m_HelpDlg->ModifyStyle(WS_POPUP, WS_CHILD);
	//m_HelpDlg->ModifyStyle(NULL, WS_POPUP);
	m_HelpDlg.ShowWindow(SW_HIDE);
}

void Center::Init_LightDlg(CWnd* pParent)
{
	m_pLightDlg = CLIGHTDLG::CreateLightDlg(pParent);
}

void Center::Init_MainMenu()
{
	m_MainMenu.LoadMenuW(IDR_MAIN_MENU);
	SetMenu(&m_MainMenu);
}

void Center::Init_ColliderDlg()
{
	m_ColliderDlg.Create(IDD_COLLIDER_CONTROLLER);
	m_ColliderDlg.ModifyStyle(WS_POPUP, WS_CHILD);
	m_ColliderDlg.ShowWindow(SW_SHOW);
	m_ColliderDlg.SetParent(this);

}

void Center::Init_WaveDlg(CWnd* pParent)
{
	m_pWaveDlg = CWaveDlg::CreateWaveDlg(pParent);
	/*m_pWaveDlg = new CWaveDlg;
	m_pWaveDlg->Create(IDD_WAVE_DLG);
	m_pWaveDlg->SetParent(pParent);
	m_pWaveDlg->ModifyStyle(WS_POPUP, WS_CHILD);
	m_pWaveDlg->ShowWindow(SW_SHOW);*/
}

void Center::CleanUp()
{
	CleanupDlg();
	CleanupManager();
}

void Center::CleanupDlg()
{
	if (m_pLightDlg != NULL)
	{
		delete m_pLightDlg;
		m_pLightDlg = NULL;
	}

	if (m_pWaveDlg != NULL)
	{
		delete m_pWaveDlg;
		m_pWaveDlg = NULL;
	}

	//마지막에 지울 것.
	if (m_Viewer != NULL)
	{
		delete m_Viewer;
		m_Viewer = NULL;
	}
}

void Center::CleanupManager()
{
	m_pSaver->DeleteInstance();
	m_pSaver = NULL;

	m_pDrawInsManager->Release();
	m_pDrawInsManager = NULL;

	m_pMatManager->DeleteInstance();
	m_pMatManager = NULL;

	m_pModelManager->DeleteModelManager();
	m_pModelManager = NULL;

	m_pColliderManager->DeleteInstance();
	m_pColliderManager = NULL;

	m_pLightManager->Release();
	m_pLightManager = NULL;

	m_pWaveManager->Release();
	m_pWaveManager = NULL;
}

void Center::UpdateColliderList()
{
	std::vector<COLLIDER>* pList = (std::vector<COLLIDER>*)m_pColliderManager->GetList();
	m_ColliderDlg.UpdateColliderList(pList);
}

void Center::UpdateLightList()
{
	m_pLightDlg->UpdateLightList();
}

void Center::UpdateRegenColliderList()
{
	const std::vector<COLLIDER>* pList = m_pColliderManager->GetList();
	m_ObjLocator.UpdateRegenColiderList(pList);
}

void Center::UpdateWaveLlist()
{
	m_pWaveDlg->ResetWaveListBox();

	const std::vector<WAVE*>* pWaveList = m_pWaveManager->GetList();
	std::vector<WAVE*>::const_iterator it;
	for (it = pWaveList->begin(); it != pWaveList->end(); it++)
	{
		WAVE* pWave = *it;
		if (pWave != NULL)
			m_pWaveDlg->AddWaveTolistBox(pWave);
	}
}


void Center::Update()
{
	m_pDrawInsManager->Update();
	m_pColliderManager->Update();
	m_pLightManager->Update();
	m_pWaveManager->Update();
}



void Center::UpdateMaterial(DRAW_INSTANCE* pDrawIns)
{
	if (pDrawIns == NULL) return;

	object* pObj = pDrawIns->first;
	HInstanceData* pInsData = pDrawIns->second;
	int matIndex = pObj->matIndex;

	if (matIndex == 0)
	{
		pInsData->SetShaderFlag(ShaderType::COLORCHIP);
	}
	else
	{
		pInsData->SetShaderFlag(ShaderType::DEFAULT);
		MATERIAL* pMat = m_pMatManager->GetMaterial(matIndex);
		pInsData->SetMaterial(pMat->hMat);
	}

}

void Center::UpdateMaterial(object* pObj)
{
	DRAW_INSTANCE* pDrawIns = m_pDrawInsManager->GetDrawIns(pObj);
	UpdateMaterial(pDrawIns);
}

void Center::UpdateModel(DRAW_INSTANCE* pDrawIns)
{
	if (pDrawIns == NULL) return;

	object* pObj			= pDrawIns->first;
	HInstanceData* pInsData = pDrawIns->second;
	int modelIndex = pObj->modelIndex;

	pInsData->Delete();
	pDrawIns->second = NULL;
	CreateGraphicInstance(pObj);
}

void Center::UpdateModel(object* pObj)
{
	DRAW_INSTANCE* pDrawIns = m_pDrawInsManager->GetDrawIns(pObj);
	UpdateModel(pDrawIns);
}

void Center::UpdateWaveList()
{
	m_pWaveManager->Update();
}

void Center::DeleteInDeleteList()
{
	std::vector<object*> deletedObjList;
	m_pDrawInsManager->DeleteInDeleteList(&deletedObjList);
	m_ObjLocator.DeleteObjInListBox(&deletedObjList);

	std::vector<Light*> deleteLightList;
	m_pLightManager->DelteInDelList(&deleteLightList);
	m_pLightDlg->DeleteLightInListBox(&deleteLightList);

	m_pWaveManager->DeleteWaveInDeleteList();
}

object* Center::GetLastSelectedObject()
{
	return m_pDrawInsManager->GetLastSelected();
}

void Center::DeleteAllDrawInsList()
{
	m_pDrawInsManager->AddToDeleteListAll();
}


void Center::CreateObj(object* pObj)
{
	if (pObj == NULL) return;
	
	HInstanceData* hIns = CreateGraphicInstance(pObj);
	
	if (hIns == NULL)
	{
		delete pObj;
		return;
	}
	
	m_pDrawInsManager->CreateObj(pObj, hIns);
}

void Center::RescaleSelected(float ratio)
{
	m_pDrawInsManager->ReScaleSelectedByRatio(ratio);
	m_pColliderManager->ReScaleSelectedByRatio(ratio);
	m_pWaveManager->ReScaleSelectedByRatio(ratio);
}

HInstanceData* Center::CreateGraphicInstance(object* pObj)
{
	HInstanceData* hResult = NULL;

	DirectX::XMFLOAT4X4 mTm = MapUtil::Identity4x4();
	pObj->GetTm(mTm);

	int modelIndex = pObj->modelIndex;
	if (modelIndex < 0)
	{
		OutputDebugStringW(L"[CreateGraphicInstance] Wrong MODEL Index");
		modelIndex = 0;
	}

	int matIndex = pObj->matIndex;
	if (matIndex < 0)
	{
		OutputDebugStringW(L"[CreateGraphicInstance] Wrong MODEL Index");
		matIndex = 0;
	}
	
	HModelData* pModel = m_pModelManager->GetModel(modelIndex)->hModel;

	HMaterialData* pMat = NULL;
	if (matIndex == 0)
	{
		hResult = pModel->AddInstance(ShaderType::COLORCHIP);
		hResult->worldTM = mTm;
	}
	else
	{
		hResult = pModel->AddInstance(ShaderType::DEFAULT);
		pMat = m_pMatManager->GetMatList()->at(matIndex)->hMat;
		hResult->SetMaterial(pMat, 0);
		hResult->worldTM = mTm;
	}

	return hResult;
}

void Center::EidtGraphicInstance(object* pObj)
{
	if (pObj == NULL)
	{
		::OutputDebugStringW(L"[VIEWER]EditGraphicInstanceHandel() : Invaildate Parameter\n");
		return;
	}
	else
	{
		HInstanceData* hTarget = m_pDrawInsManager->GetInsByObject(pObj);
		if (hTarget != NULL)
		{			
			DirectX::XMFLOAT4X4 worldTM = MapUtil::Identity4x4();
			pObj->GetTm(worldTM);

			hTarget->worldTM = worldTM;
		}
	}
}

bool Center::DeleteGraphicInstance(object* pObj)
{
	bool bResult = false;
	if (pObj == NULL)
	{
		::OutputDebugStringW(L"[VIEWER]DeleteGraphiceInstance() : Invaildate Parameter\n");
		return bResult;
	}
	else
	{
		HInstanceData* hTarget = m_pDrawInsManager->GetInsByObject(pObj, NULL);
		if (hTarget != NULL)
		{
			m_pDrawInsManager->AddDeleteList(pObj);
			bResult = true;
		}
	}

	return bResult;
}

size_t Center::GetNumberOfSelectedObj()
{
	return m_pDrawInsManager->GetNumberOfSelectedObject();
}


//선택된 오브젝트 삭제
void Center::DeleteSelectedObject()
{
	m_pDrawInsManager->AddDeleteListInSelectedList();
	UpdateObjList();
}

//선택된 오브젝트 리스트를 복사한다.
void Center::DuplicateObjectInSelectedList()
{
	std::vector<object*> pNewObjList;
	std::vector<DRAW_INSTANCE*>* pSelectedList = m_pDrawInsManager->GetSelectedList();
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = pSelectedList->begin(); it != pSelectedList->end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		object* pSrc = pDrawIns->first;
		object* pObj = new object;
		ZeroMemory(pObj, sizeof(object));

		pSrc->CopyObject(pObj);
		CreateObj(pObj);
		pNewObjList.push_back(pObj);
	}

	m_pDrawInsManager->ClearSelectedListAll();		//셀렉티드를 지운다.

	//새로 생성된 오브젝트들을 추가
	std::vector<object*>::iterator itr;
	for (itr = pNewObjList.begin(); itr != pNewObjList.end(); itr++)
	{
		m_pDrawInsManager->AddObjectInSelectedList(*itr);
	}
	pNewObjList.clear();

}

void Center::UpdateRegenColliderListInObjLocator()
{
	const std::vector<COLLIDER>* pColList = m_pColliderManager->GetList();
	m_ObjLocator.UpdateRegenColiderList(pColList);
}

int Center::SetModelIndexByName(object* pObj)
{
	int modelIndex = m_pModelManager->GetIndexByModelName(pObj->modelName);
	pObj->modelIndex = modelIndex;
	return modelIndex;
}

MODEL* Center::GetModelByInde(int index)
{
	return m_pModelManager->GetModel(index);
}

int Center::SetMatIndexByName(object* pObj)
{
	int matIndex = m_pMatManager->GetIndexByMaterialName(pObj->matName);
	pObj->matIndex = matIndex;
	return matIndex;
}

MATERIAL* Center::GetMaterialByIndex(int index)
{
	return m_pMatManager->GetMaterial(index);
}

void Center::EditMaterialSelectedALL(int matIndex)
{
	MATERIAL* pMat = m_pMatManager->GetMaterial(matIndex);
	if (pMat == NULL)
	{
		OutputDebugString(L"[EditMaterialSelectedALL] Can Not Found Material by Index\n");
		ASSERT(FALSE);
		return;
	}
	m_pDrawInsManager->ChangeMaterialSelected(pMat);
}


void Center::CreateObjectsFromList(int CountOfObject, object* pList)
{
	DeleteAllDrawInsList();

	int numOfObj = CountOfObject;
	for (int i = 0; i < numOfObj; i++)
	{
		object* pObj = new object;
		ZeroMemory(pObj, sizeof(object));

		(pList + i)->CopyObject(pObj);
		SetModelIndexByName(pObj);
		SetMatIndexByName(pObj);

		CreateObj(pObj);
		//OnCreateObj((WPARAM)m_hWnd, (LPARAM)(pObj));
	}
}

void Center::CreateCollidersFromList(STAGE_HEADER* pHeader, collider* pColliderList)
{
	m_pColliderManager->DeleteColliderList();
	int numOfCollider = pHeader->iColliderCnt;

	if (pColliderList == NULL)
	{
		OutputDebugStringW(L"[DrawColliderList]: INVAILD PARAMETER\n");
		return;
	}

	for (int i = 0; i < numOfCollider; i++)
	{
		collider* pCur = pColliderList + i;
		collider* pTemp = CreateCollider();
		pCur->CopyCollider(pTemp);
	
		HRESULT hr = m_pColliderManager->InsertColliderList(pTemp);
		if (hr != S_OK)
		{
			OutputDebugStringW(L"[DrawColliderList]FAIL TO Create Collider\n");
		}
	}

	m_pColliderManager->SetIndexWhenYouLoad();	//최대 인덱스부터 인덱스를 써 내려가기 위함.

}

void Center::CreateLightDataFromList(STAGE_HEADER* pHeader, lightData* pLightList)
{
	m_pLightManager->ClearList();
	int iNumberOfLight = pHeader->iLightCnt;
	if (pLightList == NULL)
	{
		OutputDebugStringW(L"LightList is Empty\n");
		return;
	}

	for (int i = 0; i < iNumberOfLight; i++)
	{
		m_pLightManager->CreateLight( pLightList[i] );
	}


}

//웨이브파일 리스트를 갯수 만큼 읽어 그래픽 인스턴스 생성.
void Center::CreateWaveFromList(int numOfWave, waveData* pWaveList)
{
	m_pWaveManager->CleanupWaves();
	if (pWaveList == NULL)
	{
		OutputDebugStringW(L"WaveList is empty\n");
		return;
	}

	for (int i = 0; i < numOfWave; i++)
	{
		m_pWaveManager->CreateWave(&pWaveList[i]);
	}
}


void Center::UpdateModelList()
{
	WPARAM wModelList = (WPARAM)m_pModelManager->GetModelList();
	::SendMessageW(m_ObjLocator, WM_UPDATE_MODEL_LIST, wModelList, NULL);
	//::SendMessageW(m_CharSetting, WM_UPDATE_MODEL_LIST, wModelList, NULL);
}

void Center::LoadFbxFile()
{
	//wchar_t szFilter[] = L"FBX (*.fbx)|*.fbx;*.FBX|ALL Files(*.*)|*.*||";
	wchar_t szFilter[] = L"FBX (*.fbx)|*.fbx;*.FBX|hModel (*.hmodel)|*.hmodel;*.HMODEL|ALL Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDCANCEL == dlg.DoModal())
	{
		//취소버튼 누름
		return;
	}
	CString path = dlg.GetPathName();
	CString name = dlg.GetFileName();
	CString ext  = dlg.GetFileExt();

	MODEL* pModel = NULL;
	if(ext.CompareNoCase(L"hmodel") == 0)
	{
		pModel = m_pModelManager->LoadModelFromHModel(path, name);
	}
	else if (ext.CompareNoCase(L"fbx") == 0)
	{
		pModel = m_pModelManager->LoadModelFromFBX(path, name);
	}
	
	if (pModel == NULL)
	{
		OutputDebugString(L"[FAIL]Load Model: Not Supported Format\n");
	}

	path.Empty();
	name.Empty();
	ext.Empty();

	return;
}

void SetTexture(strTEXTURE* pDest, wchar_t* pFileName)
{
	if (!pFileName) return;
	if (!pDest) return;

	enum eTex
	{
		eAlbedo = 0,
		eRoughness,
		eMetallic,
		eAo,
		eNormal,
		eHeight,
		eTex_MAX,
	};

	wchar_t* textures[eTex_MAX] = {};
	textures[eAlbedo]	= L"_albedo.png";
	textures[eRoughness] = L"_roughness.png";
	textures[eMetallic] = L"_metallic.png";
	textures[eAo]		= L"_ao.png";
	textures[eNormal]	= L"_normal.png";
	textures[eHeight]	= L"_height.png";


	wchar_t* pUnderBar = wcschr(pFileName, L'_');
	if (pUnderBar == NULL) return;					//언더바 위치 찾기.

	for (int i = 0; i < eTEX_MAX; i++)
	{
		//int res = wcscmp(pFileName, textures[i], lstrlenW(textures[i]));
		wchar_t* pD = wcsstr(pFileName, textures[i]);
		if (pD != NULL)
		{
			pDest->tex[i] = pFileName;
			break;	//같은거 찾음.
		}
	}
	
	return;
}

//여러 파일을 검사하여 해당 텍스쳐 위치에 넣는다.
void Center::LoadTextureFiles()
{
	wchar_t szFilter[] = L"PNG (*.png)|*.png;*.PNG|DDS (*.dds)|*.dds;*.DDS|ALL Files(*.*)|*.*||";
	wchar_t szDefFilter[] = L"png";

	strTEXTURE MaterialSet;		//로드할 마테리얼 파일명들을 저장한 장소
	ZeroMemory(&MaterialSet, sizeof(strTEXTURE));

	CFileDialog dlg(TRUE, szDefFilter, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_LONGNAMES | OFN_ALLOWMULTISELECT, szFilter, NULL);
	//WCHAR buffer[4096] = {};				//파일을 받을 경로	
	//dlg.m_ofn.lpstrFile = buffer;			//여러 파일을 받을 경우 퍼버 공간 부족 할 수도 있다. 6개 정도.
	//dlg.m_ofn.nMaxFile = sizeof(buffer);	
		
	if (IDOK == dlg.DoModal())
	{
		CString name;
		POSITION pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			CString strPathName = dlg.GetNextPathName(pos);
			strPathName.Replace(L"\\", L"/");
			
			if (name.IsEmpty())
			{
				int index = strPathName.ReverseFind(L'/') + 1;
				name = strPathName.Right(strPathName.GetLength() - index);
				int under_ = name.ReverseFind(L'_');	//좀 귀엽
				name.Delete(under_, name.GetLength());
			}
			
			wchar_t* pStr = new wchar_t[256];
			StrCpyW(pStr, strPathName.GetBuffer());
			SetTexture(&MaterialSet, pStr);

		}
		
		m_pEngine->StartSetting();
		m_pMatManager->LoadTexture(name, &MaterialSet);
		m_pEngine->FinishSetting();
		name.Empty();
	}

	for (int i = 0; i < 6; i++)
	{
		if (MaterialSet.tex[i] != NULL)
		{
			delete[] MaterialSet.tex[i];
			MaterialSet.tex[i] = NULL;
		}
	}

	WPARAM wMaterialList = (WPARAM)m_pMatManager->GetMatList();
	BOOL bRespond = ::PostMessageW(m_ObjLocator, WM_UPDATE_MATERIAL_LIST, wMaterialList, NULL);
	while (!bRespond)
	{
		::PostMessageW(m_ObjLocator, WM_UPDATE_MATERIAL_LIST, wMaterialList, NULL);
	}
		
}


void Center::SaveMap()
{
	//Header
	m_pSaver->SetHeader(m_MapHeader);		

	//Object
	DrawInsList* pList = (DrawInsList*)m_pDrawInsManager->GetDrawInsList();		
	m_pSaver->SetObjList(pList);

	//Collider
	m_pColliderManager->Sort();
	std::vector<COLLIDER>* pColliderList = (std::vector<COLLIDER>*)m_pColliderManager->GetList();	
	m_pSaver->SetColliderList(pColliderList);

	//Light
	std::vector<Light*>* pLightList = m_pLightManager->GetList();
	m_pSaver->SetLightList(pLightList);

	const std::vector<WAVE*>* pWaveList = m_pWaveManager->GetList();
	m_pSaver->SetWaveList(pWaveList);

	std::thread t1(&CSaver::Save, m_pSaver);
	t1.join();
}

void Center::LoadMap()
{
	int iResult = -1;
	std::thread t1(&CSaver::Load, m_pSaver, &iResult);
	std::thread::id ID = t1.get_id();
	t1.join();
	
	//다시 그리기. 분리 고려.
	if (iResult == 0)
	{
		//성공 -> 다시 그려라
		g_pCenter->ReDraw(&m_pSaver->m_StageHeader, m_pSaver);
		UpdateListBox();
	}		
	else if (iResult == 1)
	{
		//사용자 취소
		int a = 3;
	}
}

void Center::LoadFbx()
{
	std::thread t1(&Center::LoadFbxFile, this);
	t1.join();

	WPARAM wModelList = (WPARAM)m_pModelManager->GetModelList();
	::SendMessageW(m_ObjLocator, WM_UPDATE_MODEL_LIST, wModelList, NULL);
}

void Center::LoadTextureSet()
{
	std::thread t1(&Center::LoadTextureFiles, this);
	t1.join();

}

void Center::BoardCast(MSG* pMsg)
{
	if (m_ObjLocator)
	{
		::SendMessage(m_ObjLocator, pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	if (m_Viewer->m_hWnd != NULL)
	{
		::SendMessage(*m_Viewer, pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	if (m_GridController)
	{
		::SendMessage(m_GridController, pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	//if (m_CharSetting)
	//{
	//	::SendMessage(m_CharSetting, pMsg->message, pMsg->wParam, pMsg->lParam);
	//}
	if (m_ColliderDlg)
	{
		::SendMessage(m_ColliderDlg, pMsg->message, pMsg->wParam, pMsg->lParam);
	}
}

collider* Center::CreateCollider()
{
	collider* pCollider = m_pColliderManager->CreateCollider();
	return pCollider;
}

void Center::DeleteCollider(collider* pCollider)
{
	if (pCollider == NULL) return;
	eCOLLIDER_TYPE colType = pCollider->type;
	if (colType == eCOLLIDER_REGENERATIONPOINT)
	{
		m_pDrawInsManager->EditRegenIndexDefault(pCollider->index);
	}
	
	m_pColliderManager->DeleteCollider(pCollider);
}


void Center::ClearColliderList()
{
	m_pColliderManager->DeleteColliderList();
}

void Center::DeleteSelectedCollider()
{
	m_pColliderManager->DeleteColliderInSelectedList();
	UpdateColliderList();
	UpdateRegenColliderListInObjLocator();
}

void Center::DuplicateColliderInSelectedList()
{
	m_pColliderManager->DuplicateColliderInSelectedList();
	UpdateColliderList();
}



void Center::ToggleColliderSelect()
{
	static bool bIsColliderSelect = true;
	bIsColliderSelect = !bIsColliderSelect;
	m_pColliderManager->EnableSelect(bIsColliderSelect);
}

void Center::ToggleObjectSelect()
{
	static bool bEnableSelect = true;
	bEnableSelect = !bEnableSelect;
	m_pDrawInsManager->EnableSelect(bEnableSelect);
}

void Center::ToggleLightSelect()
{
	static bool bCanISelect = true;
	bCanISelect = !bCanISelect;
	m_pLightManager->SetSelectEnabled(bCanISelect);
}

//다이알로그 박스에서 오브젝트를 선택했을 경우.
void Center::SelectChangeObject(object* pObj)
{
	//1. 선택 리스트 비우기.
	//2. 해당 오브젝트 선택된 리스트에 추가
	//3. 기즈모 셋팅.
	m_pDrawInsManager->ClearSelectedListAll();
	m_pDrawInsManager->AddSelected(pObj);
	SetGizmo(pObj);
}



collider* Center::PickingCollider(int screenX, int screenY)
{
	CPoint clickPoint;
	clickPoint.x = screenX;
	clickPoint.y = screenY;

	CRect rc;
	::GetClientRect(m_Viewer->m_hWnd, &rc);

	collider* pCollider = m_pColliderManager->Picking(clickPoint.x, clickPoint.y, rc.Width(), rc.Height());
	if (pCollider != NULL)
	{
		m_pColliderManager->AddSelected_public(pCollider);
		DirectX::XMFLOAT3 pos = m_pColliderManager->GetSelectedPos();
	}
	else
	{
		m_pColliderManager->DeleteSelectedAll();
	}

	m_ColliderDlg.SelectCollider(pCollider);

	return pCollider;
}


void Center::UpdateObjEditControl(object* pObj)
{
	m_ObjLocator.SetObjectEditBoxes(pObj);
}

void Center::UpdateColliderEditControl(collider* pCollider)
{
	if (pCollider)
		m_ColliderDlg.SetEditBoxByCollider(pCollider);
	else
		m_ColliderDlg.SetEditBoxZero();
}

void Center::UpdateWaveEditControl(WAVE* pWave)
{
	m_pWaveDlg->SetDataToEditCtrlByWave(pWave);
}

void Center::DuplicateLightSelectedList()
{
	m_pLightManager->DuplicateSelected();

}

void Center::DeleteSelectedLight()
{
	m_pLightManager->DeleteSelectedLight();
	m_pLightDlg->UpdateLightList();
}

Light* Center::PickingLight(int screenX, int screenY)
{
	CPoint clickPoint;
	clickPoint.x = screenX;
	clickPoint.y = screenY;

	CRect rc;
	::GetClientRect(m_Viewer->m_hWnd, &rc);

	Light* pLight = m_pLightManager->Picking(clickPoint.x, clickPoint.y, rc.Width(), rc.Height());
	if (pLight != NULL)
	{
		m_pLightManager->AddSelected_public(pLight);
		//DirectX::XMFLOAT3 pos = m_pLightVector->GetSelectedPos();
	}
	else
	{
		m_pLightManager->ClearSelected();
	}

	//Update Dialog
	m_pLightDlg->SetSelIndexInListBox(pLight);

	return pLight;
}

void Center::UpdateSelectedLight()
{
	m_pLightDlg->UpdateSelectedLight();
}

void Center::DrawGrid(int width, int height, int offset)
{
	m_Viewer->DrawGrid(width, height, offset);
}

void Center::ChangeGrid(int width, int height, int offset)
{
	m_Viewer->ChangeGrid(width, height, offset);
}



void Center::SelectObj(object* pObj)
{
	m_ObjLocator.SetObjectEditBoxes(pObj);
}

void Center::SelectCollider(collider* pCollider)
{
	m_ColliderDlg.SelectCollider(pCollider);
}

void Center::SetGizmo(collider* pCol)
{
	if (pCol != NULL)
		m_pColliderManager->AddSelected_OnlyOne(pCol);
}

void Center::SetGizmo(object* pObj)
{
	if(pObj != NULL)
		m_pDrawInsManager->AddSelected(pObj);
}


WAVE* Center::CreateWave(waveData* pInfo)
{
	return m_pWaveManager->CreateWave(pInfo);
}


bool Center::AddDeleteList(WAVE* pWave)
{
	return m_pWaveManager->AddDeleteList(pWave);
}


void Center::EditWave(WAVE* pDest, waveData* pSrc)
{
	m_pWaveManager->EditWave(pDest, pSrc);
}

void Center::AddSelectedWave(WAVE* pWave)
{
	m_pWaveManager->AddSelected(pWave);
}

void Center::AddSelectedWaveOnlyOne(WAVE* pWave)
{
	//다른것들은 선택리스트에서 지운다.
	m_pDrawInsManager->AddDeleteListInSelectedList();;
	m_pColliderManager->DeleteSelectedAll();
	m_pLightManager->DeleteSelectedLight();

	m_pWaveManager->AddSelectedOnlyOne(pWave);

}

void Center::DeleteSelectedListAll()
{
	m_pWaveManager->DeleteSelectedAll();

	WAVE* pWave = m_pWaveManager->GetLastSelected();
	m_pWaveDlg->SetWaveListBoxCurSel(pWave);
	m_pWaveDlg->SetDataToEditCtrl(&pWave->info);
}

void Center::ReDraw(STAGE_HEADER* pHeader, CSaver* pSaver)
{
	object* pObjList				= pSaver->m_pObjList;
	collider* pColliderList			= pSaver->m_pColliderList;
	lightData* pLightDataList		= pSaver->m_pLightDataList;
	
	int waveCnt = pSaver->m_cntWave;
	waveData* pWaveDataList			= pSaver->m_pWaveList;

	CreateObjectsFromList(pHeader->iObjCnt, pObjList);
	CreateCollidersFromList(pHeader, pColliderList);
	CreateLightDataFromList(pHeader, pLightDataList);
	CreateWaveFromList(waveCnt, pWaveDataList);

	pSaver->CleanUp();

}

void Center::UpdateListBox()
{
	//오브젝트 리스트 박스 업데이트.
	UpdateObjList();
	UpdateColliderList();
	m_pLightDlg->UpdateLightList();
	UpdateRegenColliderList();
	UpdateWaveLlist();
}

void Center::UpdateObjList()
{
	DrawInsList* pList = m_pDrawInsManager->GetDrawInsList();
	m_ObjLocator.UpdateObjListBox(pList);
}



void Center::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL Center::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pEngine = HEngine_DX12_3D::GetInstance();
	g_hCenter = m_hWnd;
	g_pCenter = this;
	m_pDrawInsManager = DrawInsManager::GetInstance();


	//최종적으로 여기에만 메니저가 남아있도록.
	m_pSaver			= CSaver::GetInstance();
	//m_pCharManager	= CharacterManager::GetModelManager();
	m_pMatManager		= MaterialManager::GetInstance();
	m_pModelManager		= ModelManager::GetModelManager();
	m_pColliderManager	= CColliderManager::GetInstance();
	m_pLightManager		= CLightManager::GetInstance();
	m_pWaveManager		= CWaveManager::GetInstance();
	m_pWaveManager->Initialize();	//엔진이 초기화 된 이후 얻을 것.

	Init_Viewer();		//매니저 업데이트가 들어잇어 먼저 업데이트 할 것.	
	

	Init_Loc();
	Init_GridController();
	Init_MainMenu();
	//Init_Cmd();
	Init_ColliderDlg();
	Init_HelpDlg(this);
	Init_LightDlg(this);
	Init_WaveDlg(this);

	
	CRect rcMain;
	GetClientRect(rcMain);

	CRect rcViewer;
	m_Viewer->GetClientRect(&rcViewer);
	CRect rcLoc;
	m_ObjLocator.GetClientRect(&rcLoc);

	m_ObjLocator.MoveWindow(rcViewer.Width() + 25, 0, rcLoc.Width() + 20, rcLoc.Height() + 45);
	
	CRect rcCol;
	m_ColliderDlg.GetClientRect(rcCol);

	CRect newCol	= rcCol;
	newCol.left		= rcViewer.Width() + 25;
	newCol.right	= newCol.left + rcCol.Width() + 20;
	newCol.top		= rcLoc.bottom + 50;
	newCol.bottom	= newCol.top + 280;
	m_ColliderDlg.MoveWindow(newCol);

	m_GridController.Move(rcViewer.Width() + 25, newCol.bottom + 5);

	UpdateModelList();

	return 0;
}


BOOL Center::PreTranslateMessage(MSG* pMsg)
{
	UINT msg	  = pMsg->message;
	WPARAM wParam = pMsg->wParam;
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F1:
			return TRUE;
		case VK_F2:
			return TRUE;
		case VK_F3:
			return TRUE;
		case VK_F4:
			return TRUE;

		case VK_UP:
		{
			m_Viewer->MoveCam(eDirection_Up);
			break;
		}

		case VK_DOWN:
		{
			m_Viewer->MoveCam(eDirection_Down);
			break;
		}
		case VK_LEFT:
		{
			m_Viewer->MoveCam(eDirection_Left);
			break;
		}
		case VK_RIGHT:
		{
			m_Viewer->MoveCam(eDirection_Right);
			break;
		}
		case VK_OEM_COMMA:
			m_Viewer->ControlCamVelo(-1.0f);
			break;
		case VK_OEM_PERIOD:
			m_Viewer->ControlCamVelo(1.0f);
			break;

		case VK_CONTROL:
			m_pColliderManager->SetSelectMode(eSELECT_MODE::eSELECT_MULTI);
			m_pDrawInsManager->SetSelectedMode(eSELECT_MODE::eSELECT_MULTI);
			m_pLightManager->SetSelectMode(eSELECT_MODE::eSELECT_MULTI);
			break;
		
		case 'L':
			{

				int a = 0;
				break;
			}

		case 'P':
		{
			int a = 0;
			break;
		}
			
		case VK_OEM_7://쿼테이션
		{
			int a = 0;
			break;
		}

		case VK_OEM_1://콜론
		{
			int a = 0;
			break;
		}
				
		}

		//BoardCast(pMsg);
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			m_HelpDlg.ShowWindow(SW_SHOW);
			//int a = 0;
			//break;
			return TRUE;
		case VK_F2:

			return TRUE;
		case VK_F3:
			SaveMap();
			return TRUE;
		case VK_F4:
			LoadMap();
			return TRUE;
		case VK_F5:
		{
			ToggleObjectSelect();
			return TRUE;
		}
		case VK_F6:
		{
			ToggleColliderSelect();
			return TRUE;
		}
		case VK_F7:
		{
			ToggleLightSelect();
			return TRUE;
		}

		case VK_F9:
		{
			LoadFbx();
			return TRUE;
		}
		case VK_F10:
		{
			LoadTextureSet();
			return TRUE;
		}

		case VK_F11:
		{
			static bool bRayTracing = true;
			bRayTracing = !bRayTracing;
			m_pEngine->SetReflectionEffect(bRayTracing);
			
			return TRUE;
		}

		case VK_SHIFT:
		{
			static bool bShadow = false;
			bShadow = !bShadow;
			m_pEngine->SetShadowEffect(bShadow);
			break;
		}

		case VK_DELETE:
			DeleteSelectedCollider();
			DeleteSelectedObject();
			DeleteSelectedLight();			
			
			break;
		case VK_CONTROL:
			m_pColliderManager->SetSelectMode(eSELECT_MODE::eSELECT_ONE);
			m_pDrawInsManager->SetSelectedMode(eSELECT_MODE::eSELECT_ONE);
			m_pLightManager->SetSelectMode(eSELECT_MODE::eSELECT_ONE);
			break;

		case 'D':
		{
			DuplicateObjectInSelectedList();
			DuplicateColliderInSelectedList();
			DuplicateLightSelectedList();

			//UpdateList
			UpdateObjList();
			UpdateColliderList();
			UpdateLightList();
			UpdateRegenColliderListInObjLocator();
			break;
		}
			
		case 'G':
		{
			GRID_INFO* pGridInfo = new GRID_INFO;
			m_MapHeader.iWidth = 0;
			m_MapHeader.iHeight = 0;
			m_MapHeader.offset = 0;

			pGridInfo->iHeight = 0;
			pGridInfo->iWidth = 0;
			pGridInfo->offset = 0;

			WPARAM wp = (WPARAM)pGridInfo;
			OnChangeGrid(wp, NULL);
			m_GridController.SetGrid(0, 0, 0.f);
			break;
		}

		case 'O':
		{
			m_Viewer->ToggleOffsetMode();
			break;
		}

		case '2':
		{
			object* pObj = m_pDrawInsManager->GetLastSelected();
			if (pObj != NULL)
			{
				m_Viewer->SetMainView(pObj->pos);
			}
			
			break;
		}

		}
		//BoardCast(pMsg);
		break;

	case WM_SYSKEYDOWN:
		switch (wParam)
		{
		case VK_F10:
			LoadTextureSet();
			return TRUE;
		}
		break;
	}

	return 0;
	//return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(Center, CDialogEx)
	ON_WM_KEYUP()
	ON_COMMAND(ID_MAIN_OBJLOCATOR, &Center::OnShowWindowObjlocator)
	ON_COMMAND(ID_MAIN_VIEWER, &Center::OnShowWindowViewer)
	ON_COMMAND(ID_MAIN_GRIDCON, &Center::OnShowWindowGridcon)
	ON_MESSAGE(WM_HIDE_WINDOW, &Center::OnHideWindow)
	ON_MESSAGE(WM_REQUEST_MODEL_LIST, &Center::OnRequestModelList)
	ON_MESSAGE(WM_REQUEST_MAT_LIST, &Center::OnRequestMatList)
	ON_MESSAGE(WM_OBJECT_EDIT, &Center::OnEditObj)
	ON_MESSAGE(WM_OBJECT_DELETE, &Center::OnDeleteObj)
	ON_MESSAGE(WM_UPDATE_MODEL_LIST, &Center::OnUpdateModelList)
	ON_MESSAGE(WM_UPDATE_MATERIAL_LIST, &Center::OnUpdateMatList)
	ON_MESSAGE(WM_SAVE_MAP, &Center::OnSaveMap)
	ON_MESSAGE(WM_LOAD_MAP, &Center::OnLoadMap)
	ON_COMMAND(ID_MAIN_CHAR_EIDT, &Center::OnShowWindowCharEidt)
	ON_COMMAND(ID_MAIN_CMD, &Center::OnShowWindowCmd)
	ON_MESSAGE(WM_LIST_BOX_NONE, &Center::OnListBoxNone)
	ON_MESSAGE(WM_CREATE_COLLIDER, &Center::OnCreateCollider)
	//(WM_DELETE_COLLIDER, &Center::OnDeleteCollider)
	ON_MESSAGE(WM_EDIT_COLLIDER, &Center::OnEditCollider)
	ON_MESSAGE(WM_LOAD_FBX, &Center::OnLoadFbx)
	ON_COMMAND(ID_MENU_SAVE, &Center::OnMenuSave)
	ON_COMMAND(ID_MENU_LOAD, &Center::OnMenuLoad)
	ON_COMMAND(ID_MENU_LOAD_FBX, &Center::OnMenuLoadFbx)
	ON_COMMAND(ID_MENU_EXIT, &Center::OnMenuExit)
	ON_MESSAGE(WM_PICKING_COLLIDER, &Center::OnPickingCollider)
	ON_COMMAND(ID_COLLIDER_WINDOW, &Center::OnColliderWindow)
	ON_COMMAND(ID_LIGHT_WINDOW, &Center::OnLightWindow)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// MainDlg 메시지 처리기
void Center::OnShowWindowObjlocator()
{
	if (m_MainMenu.GetMenuState(ID_MAIN_OBJLOCATOR, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_OBJLOCATOR, MF_CHECKED);
		m_ObjLocator.ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_OBJLOCATOR, MF_UNCHECKED);
		m_ObjLocator.ShowWindow(SW_HIDE);
	}

}

void Center::OnShowWindowViewer()
{
	if (m_MainMenu.GetMenuState(ID_MAIN_VIEWER, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_VIEWER, MF_CHECKED);
		m_Viewer->ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_VIEWER, MF_UNCHECKED);
		m_Viewer->ShowWindow(SW_HIDE);
	}
}

void Center::OnShowWindowGridcon()
{
	if (m_MainMenu.GetMenuState(ID_MAIN_GRIDCON, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_GRIDCON, MF_CHECKED);
		m_GridController.ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_GRIDCON, MF_UNCHECKED);
		m_GridController.ShowWindow(SW_HIDE);
	}
}

void Center::OnShowWindowCharEidt()
{
	//if (m_MainMenu.GetMenuState(ID_MAIN_CHAR_EIDT, MF_BYCOMMAND) != MF_CHECKED)
	//{
	//	m_MainMenu.CheckMenuItem(ID_MAIN_CHAR_EIDT, MF_CHECKED);
	//	m_CharSetting.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_MainMenu.CheckMenuItem(ID_MAIN_CHAR_EIDT, MF_UNCHECKED);
	//	m_CharSetting.ShowWindow(SW_HIDE);
	//}
}

void Center::OnShowWindowCmd()
{
	if (m_MainMenu.GetMenuState(ID_MAIN_CMD, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_CMD, MF_CHECKED);
		//m_Cmd.ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_CMD, MF_UNCHECKED);
		//m_Cmd.ShowWindow(SW_HIDE);
	}
}

void Center::OnColliderWindow()
{
	if (m_MainMenu.GetMenuState(ID_COLLIDER_WINDOW, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_COLLIDER_WINDOW, MF_CHECKED);
		m_ColliderDlg.ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_COLLIDER_WINDOW, MF_UNCHECKED);
		m_ColliderDlg.ShowWindow(SW_HIDE);
	}
}



afx_msg LRESULT Center::OnHideWindow(WPARAM wParam, LPARAM lParam)
{
	if (lParam == eObjLocator)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_OBJLOCATOR, MF_UNCHECKED);
		m_ObjLocator.ShowWindow(SW_HIDE);
	}
	else if (lParam == eViewer)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_VIEWER, MF_UNCHECKED);
		m_Viewer->ShowWindow(SW_HIDE);
	}
	else if (lParam == eGridCon)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_GRIDCON, MF_UNCHECKED);
		m_GridController.ShowWindow(SW_HIDE);
	}
	else if (lParam == eCmd)
	{

	}
	else if (lParam == eCharSetting)
	{
		m_MainMenu.CheckMenuItem(ID_MAIN_CHAR_EIDT, MF_UNCHECKED);
		//m_CharSetting.ShowWindow(SW_HIDE);
	}

	return 0;
}


afx_msg LRESULT Center::OnChangeGrid(WPARAM wParam, LPARAM lParam)
{
	GRID_INFO* pGridInfo = (GRID_INFO*)wParam;
	int width = pGridInfo->iWidth;
	int height = pGridInfo->iHeight;
	int offset = pGridInfo->offset;

	m_Viewer->ChangeGrid(width, height, offset);
	return 0;
}



afx_msg LRESULT Center::OnRequestModelList(WPARAM wParam, LPARAM lParam)
{
	//WPARMA = 요청한 윈도우 핸들
	HWND hTarget = (HWND)wParam;
	std::vector<MODEL*>* pModelList = (m_pModelManager->GetModelList());
	::SendMessageW(hTarget, WM_UPDATE_MODEL_LIST, (WPARAM)pModelList, NULL);
	return 0;
}


afx_msg LRESULT Center::OnRequestMatList(WPARAM wParam, LPARAM lParam)
{
	//WPARMA = 요청한 윈도우 핸들
	HWND hTarget = (HWND)wParam;
	std::vector<MATERIAL*>* pMatList = (m_pMatManager->GetMatList());
	::SendMessageW(hTarget, WM_UPDATE_MATERIAL_LIST, (WPARAM)pMatList, NULL);
	return 0;
}



void Center::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'Q':
	{
		OutputDebugStringW(L"[KEY_UP] Q");
		break;
	}

	case VK_CONTROL:
	{
		//OutputDebugStringW(L"[KEY_UP] CONTROL - Center\n");
		::SendMessageW(*m_Viewer, WM_KEYUP, (WPARAM)(nChar), NULL);
		break;
	}
	}
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}



afx_msg LRESULT Center::OnEditObj(WPARAM wParam, LPARAM lParam)
{
	object* pObj = (object*)wParam;
	
	//EidtGraphicInstance(pObj);

	return 0;
}


afx_msg LRESULT Center::OnDeleteObj(WPARAM wParam, LPARAM lParam)
{
	object* pObj = (object*)wParam;
	
	DeleteGraphicInstance(pObj);

	return 0;
}



afx_msg LRESULT Center::OnUpdateModelList(WPARAM wParam, LPARAM lParam)
{
	WPARAM wp = wParam;
	::SendMessageW(m_ObjLocator, WM_UPDATE_MODEL_LIST, wp, NULL);
	//::SendMessageW(m_CharSetting, WM_UPDATE_MODEL_LIST, wp, NULL);

	return 0;
}


afx_msg LRESULT Center::OnUpdateMatList(WPARAM wParam, LPARAM lParam)
{
	WPARAM wp = wParam;
	::SendMessageW(m_ObjLocator, WM_UPDATE_MATERIAL_LIST, wp, NULL);
	//::SendMessageW(m_CharSetting, WM_UPDATE_MATERIAL_LIST, wp, NULL);

	return 0;
}


afx_msg LRESULT Center::OnSaveMap(WPARAM wParam, LPARAM lParam)
{
	SaveMap();
	return TRUE;
}


afx_msg LRESULT Center::OnLoadMap(WPARAM wParam, LPARAM lParam)
{
	LoadMap();
	return TRUE;
}




afx_msg LRESULT Center::OnListBoxNone(WPARAM wParam, LPARAM lParam)
{
	::SendMessageW(m_ObjLocator, WM_LIST_BOX_NONE, NULL, NULL);

	return 0;
}


afx_msg LRESULT Center::OnCreateCollider(WPARAM wParam, LPARAM lParam)
{
	collider* pCol = (collider*)wParam;
	HRESULT hr = m_pColliderManager->InsertColliderList(pCol);;

	return 0;
}


//afx_msg LRESULT Center::OnDeleteCollider(WPARAM wParam, LPARAM lParam)
//{
//	collider* pCol = (collider*)wParam;
//
//	m_pColliderVector->DeleteCollider(pCol);
//
//	return 0;
//}


afx_msg LRESULT Center::OnEditCollider(WPARAM wParam, LPARAM lParam)
{
	collider* pCol = (collider*)wParam;

	m_pColliderManager->EditCollider(pCol);
	UpdateColliderList();
	return 0;
}


afx_msg LRESULT Center::OnLoadFbx(WPARAM wParam, LPARAM lParam)
{
	LoadFbxFile();
	return 0;
}


afx_msg LRESULT Center::OnPickingCollider(WPARAM wParam, LPARAM lParam)
{
	DirectX::XMFLOAT3* pCenterPos = NULL;
	CPoint clickPoint;
	clickPoint.x = wParam;
	clickPoint.y = lParam;

	CRect rc;
	::GetClientRect(m_Viewer->m_hWnd, &rc);

	collider* pCollider = m_pColliderManager->Picking(clickPoint.x, clickPoint.y, rc.Width(), rc.Height());
	if (pCollider != NULL)
	{
		m_pColliderManager->AddSelected_public(pCollider);
		DirectX::XMFLOAT3 pos = m_pColliderManager->GetSelectedPos();
		pCenterPos = new DirectX::XMFLOAT3();
		pCenterPos->x = pos.x;
		pCenterPos->y = pos.y;
		pCenterPos->z = pos.z;
	}
	else
	{
		m_pColliderManager->DeleteSelectedAll();
	}


	//std::thread t1(&CColliderManager::Picking, m_pColliderVector, clickPoint.x, clickPoint.y, rc.Width(), rc.Height());
	//t1.join();

	return (LRESULT)pCenterPos;
}




void Center::OnMenuSave()
{
	SaveMap();
}


void Center::OnMenuLoad()
{
	LoadMap();
}


void Center::OnMenuLoadFbx()
{
	LoadFbx();
}

void ASK(int& retValue)
{
	retValue = ::MessageBoxW(NULL, L"종료?", L"종료?", MB_OKCANCEL);
}


void Center::OnMenuExit()
{
	int ret = 0;
	std::thread ti(ASK, std::ref(ret));
	ti.join();

	if (ret == IDOK)
	{
		::SendMessageW(m_hWnd, WM_CLOSE, NULL, NULL);
	}

}


void Center::OnLightWindow()
{
	
	if (m_MainMenu.GetMenuState(ID_LIGHT_WINDOW, MF_BYCOMMAND) != MF_CHECKED)
	{
		m_MainMenu.CheckMenuItem(ID_LIGHT_WINDOW, MF_CHECKED);
		m_pLightDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_MainMenu.CheckMenuItem(ID_LIGHT_WINDOW, MF_UNCHECKED);
		m_pLightDlg->ShowWindow(SW_HIDE);
	}

}


void Center::OnClose()
{
	int a = 0;
	CleanUp();

	CDialogEx::OnClose();
}
