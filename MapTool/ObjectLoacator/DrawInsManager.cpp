#include "pch.h"
#include "DrawInsManager.h"

#include "MapUtil.h"
//#include "ViewDlg.h"
#include "WMDefine.h"
#include "Center.h"

DrawInsManager* DrawInsManager::m_pInstance = NULL;
int DrawInsManager::m_InsCount = 0;

struct comp_hGraphic
{
	comp_hGraphic(HInstanceData* hIns) : _hIns(hIns) {}

	bool operator() (DRAW_INSTANCE* pIns)
	{
		return ((pIns->second) == _hIns);
	}

	HInstanceData* _hIns;
};

struct comp_obj									
{
	comp_obj(object* pObj) : _pObj(pObj) {}

	bool operator() (DRAW_INSTANCE* pIns)
	{
		return ((pIns->first) == _pObj);
	}

	object* _pObj;
};

DrawInsManager::DrawInsManager()
{
}

DrawInsManager::~DrawInsManager()
{
	cleanup();
}

//매니저 인스턴스 삭제시에만 사용 할것. use only delete manager
void DrawInsManager::cleanup()
{
	m_SelectedList.clear();
	m_DeleteList.clear();
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end();)
	{
		//DRAW_INSTANCE는 std::pair를 재정의 한 것.
		//소멸자 정의가 되나?
		//두개의 인스턴스의 묶음(오브젝트 인스턴스 + 그래픽 인스턴스)
		//각각 삭제한 후 본인 인스턴스도 삭제해야한다.
		DRAW_INSTANCE* pIns = *it;
		delete pIns->first;
		pIns->first = NULL;

		pIns->second->Delete();
		pIns->second = NULL;

		delete pIns;
		pIns = NULL;

		it = m_List.erase(it);
	}
	
}



void DrawInsManager::Update()
{
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pObj = pIns->first;
		HInstanceData* pGraphicData = pIns->second;

		if (pObj != NULL)
		{
			pObj->GetTm(pGraphicData->worldTM);
		}
	}
}

//bool DrawInsManager::UpdateTM(object* pObj)
//{
//	bool bResult = false;
//	size_t size = m_List.size();
//	if (size <= 0) return bResult;
//	std::vector<DRAW_INSTANCE>::iterator delete_it;
//
//	for (delete_it = m_List.begin(); delete_it != m_List.end(); delete_it++)
//	{
//		object* pCur = delete_it->first;
//		if (pCur == pObj)
//		{
//			HInstanceData* pInsData = delete_it->second;
//
//			pObj->GetTm(pInsData->worldTM);
//
//			bResult = true;
//			break;
//		}
//	}
//
//	return bResult;
//}


DrawInsManager* DrawInsManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new DrawInsManager;
	}
	m_InsCount++;
	return m_pInstance;
}

void DrawInsManager::Release()
{
	m_InsCount--;
	if (m_InsCount < 1)
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
	
}



void DrawInsManager::SetSelectedMode(eSELECT_MODE mode)
{
	if (mode > eSELECT_MAX) return;
	m_eSelectedMode = mode;
}

void DrawInsManager::EnableSelect(bool bEnable)
{
	m_bEnableSelect = bEnable;
	if (!m_bEnableSelect) m_SelectedList.clear();
}

bool DrawInsManager::IsEnableSelectObject()
{
	return m_bEnableSelect;
}

void DrawInsManager::AddToDeleteListAll()
{
	ClearSelectedListByDeleteList();
	//m_SelectedList.clear();
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		AddDeleteList(pIns);	//당장삭제하지 않고 리스트에 넣어둔다.
		//delete_it = m_List.erase(delete_it);		
	}
}


//함수 이름 변경. DeleteSelectedObject() <- public 함수.
//사용자 입장에서는 어떻게 지워지는지는 중요하지 않다.
void DrawInsManager::AddDeleteListInSelectedList()
{
	//벡터의 마지막부터 지운다.
	std::vector<DRAW_INSTANCE*>::iterator it = m_SelectedList.end();
	while ( it > m_SelectedList.begin() )
	{
		it--;
		DRAW_INSTANCE* pIns = *it;
		AddDeleteList(pIns);			//삭제 리스트에 추가
		it = m_SelectedList.erase(it);
	}
}

void DrawInsManager::AddDeleteList(DRAW_INSTANCE* pIns)
{
	if (pIns != nullptr)
	{
		m_DeleteList.push_back(pIns);
	}
}

void DrawInsManager::AddDeleteList(object* pObj)
{
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		if (pIns->first == pObj)
		{
			m_DeleteList.push_back(pIns);		//삭제 리스트에 추가.
			break;
		}
	}
}

void DrawInsManager::DeleteInDeleteList(std::vector<object*>* pOut)
{
	std::vector<object*>* pObjList = pOut;
	ClearSelectedListByDeleteList();
	int count = 0;
	std::vector<DRAW_INSTANCE*>::iterator delete_it;
	for (delete_it = m_DeleteList.begin(); delete_it != m_DeleteList.end(); delete_it++)
	{
		DRAW_INSTANCE* pIns = *delete_it;
		object* pTarget = pIns->first;				//지우려는 대상.

		std::vector<DRAW_INSTANCE*>::iterator list_iter;
		for (list_iter = m_List.begin(); list_iter != m_List.end(); list_iter++)
		{
			DRAW_INSTANCE* pDrawIns = *list_iter;
			object* pTemp = pDrawIns->first;
			if (pTarget == pTemp)
			{
				delete pDrawIns->first;
				pDrawIns->first = NULL;

				pDrawIns->second->Delete();
				pDrawIns->second = NULL;

				if (pObjList != NULL) 
				{
					pObjList->push_back(pTarget);
				}

				delete pDrawIns;
				pDrawIns = NULL;

				m_List.erase(list_iter);
				count++;
				break;
			}
		} //오브젝트 리스트 순회.
	}

	if (m_DeleteList.size() > 0)
	{
		wchar_t buffer[256];
		wsprintf(buffer, L"Delete COUNT : %d\n", count);
		OutputDebugStringW(buffer);

	}
	
	m_DeleteList.clear();
}

void DrawInsManager::MakeDrawInstance(object* pObj, HInstanceData* pGraphicsData)
{
	//std::make_pair<object*, HInstanceData*>(pObj, pGraphicsData);
	DRAW_INSTANCE* pIns = new DRAW_INSTANCE(pObj, pGraphicsData);

	m_List.push_back(pIns);
}

void DrawInsManager::EditObject(object* pDest, object* pSrc)
{
	pSrc->CopyObject(pDest);
}


object* DrawInsManager::AddSelected_public(HInstanceData* pHIns)
{
	if (!m_bEnableSelect) return NULL;
	HInstanceData* pIns = pHIns;
	object* pResult = NULL;
	
	if (m_eSelectedMode == eSELECT_ONE)
	{
		pResult = AddSelected_OnlyOne(pIns);
	}
	else if (m_eSelectedMode == eSELECT_MULTI)
	{
		pResult = AddSelected_DeleteAlreadySelected(pIns);
	}

	return pResult;
}

object* DrawInsManager::AddSelected(HInstanceData* pHIns)
{
	if (!m_bEnableSelect) return nullptr;
	if (pHIns == nullptr) return nullptr;

	HInstanceData* pIns = pHIns;
	std::vector<DRAW_INSTANCE*>::iterator it;
	object* pResult = NULL;

	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		if (pDrawIns->second == pIns)
		{
			//이미 선택 리스트에 있으면 추가적으로 선택을 하지 않는다.
			int a = 0;
			goto lb_return;
		}
	}

	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		object* pObj			= pDrawIns->first;
		HInstanceData* pGraphic = pDrawIns->second;
		if (pGraphic == pIns)
		{
			m_SelectedList.push_back( *it );
			pResult = pObj;
			
			///와이어 프레임 문제. ->...
			//pGraphic->SetShaderFlag(ShaderType::WIREFRAME);
			
			break;
		}
	}

lb_return:
	return pResult;
}

object* DrawInsManager::AddSelected_DeleteAlreadySelected(HInstanceData* pHIns)
{
	if (!m_bEnableSelect) return NULL;

	std::vector<DRAW_INSTANCE*>::iterator it;
	object* pResult = NULL;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		HInstanceData* pGraphic = pDrawIns->second;
		if (pGraphic == pHIns)
		{
			object* pObj = pDrawIns->first;
			int matIndex = pObj->matIndex;
			if (matIndex == 0)
			{
				pGraphic->SetShaderFlag(ShaderType::COLORCHIP);
			}
			else
			{
				pGraphic->SetShaderFlag(ShaderType::DEFAULT);
			}
			
			m_SelectedList.erase(it);
			return pResult;
		}
	}

	pResult = AddSelected(pHIns);
	return pResult;
}

object* DrawInsManager::AddSelected_OnlyOne(HInstanceData* pHIns)
{
	ClearSelectedListAll();
	if (!m_bEnableSelect) return nullptr;
	if (pHIns == nullptr) return nullptr;
	
	return AddSelected(pHIns);
}

DRAW_INSTANCE* DrawInsManager::AddSelected(object* pObj)
{
	DRAW_INSTANCE* pDrawIns = FindDrawIns(pObj);
	if (pDrawIns != NULL)
	{
		bool isIn = IsSelected(pObj);
		if(isIn == false)
			m_SelectedList.push_back(pDrawIns);
	}

	return pDrawIns;
}

object* DrawInsManager::GetLastSelected()
{
	if (m_SelectedList.size() <= 0) return NULL;

	object* pResult = m_SelectedList.back()->first;
	return pResult;
}

void DrawInsManager::AddObjectInSelectedList(object* pObj)
{
	if (!m_bEnableSelect) return;

	std::vector<DRAW_INSTANCE*>::iterator it;
	for ( it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		if (pDrawIns->first == pObj)
		{
			m_SelectedList.push_back(pDrawIns);
			break;
		}
	}
	
}

size_t DrawInsManager::GetNumberOfSelectedObject()
{
	return m_SelectedList.size();
}

bool DrawInsManager::IsSelected(object* pObj)
{
	bool bResult = false;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		object* pTemp = pDrawIns->first;
		if (pObj == pTemp)
		{
			bResult = true;
			break;
		}
	}
	
	return bResult;
}

DRAW_INSTANCE* DrawInsManager::FindDrawIns(object* pObj)
{
	DRAW_INSTANCE* pResult = NULL;
	std::vector< DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); ++it)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		if (pDrawIns->first == pObj)
		{
			pResult = pDrawIns;
			break;
		}
	}
	return pResult;
}

void DrawInsManager::ClearSelectedListAll()
{
	std::vector<DRAW_INSTANCE*>::iterator iter;
	for (iter = m_SelectedList.begin(); iter != m_SelectedList.end(); iter++)
	{
		DRAW_INSTANCE* pDrawIns = *iter;
		object* pObj = pDrawIns->first;
		HInstanceData* pGraphic = pDrawIns->second;
		int matIndex = pObj->matIndex;
		if (matIndex == 0)
		{
			//선택 리스트에 잇는것을 삭제하기때문에 인스턴스를 삭제하지 않고 쉐이더 플레그만 바꿔준다.
			pGraphic->SetShaderFlag(ShaderType::COLORCHIP);
		}
		else
		{
			pGraphic->SetShaderFlag(ShaderType::DEFAULT);
			//pGraphic->SetMaterial()
		}
		
	}

	m_SelectedList.clear();
}

//삭제 리스트에 오브젝트들 삭제전에 돌려줄것.
void DrawInsManager::ClearSelectedListByDeleteList()
{
	//DeleteList에 있는 
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_DeleteList.begin(); it != m_DeleteList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pTarget = pIns->first;	//삭제 리스트에 있는 오브젝트

		ClerarSelectedListByObject(pTarget);
	}
}

//삭제 성공 : true 실패 false 리턴.
bool DrawInsManager::ClerarSelectedListByObject(object* pTarget)
{
	bool bResult = false;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pTemp = pIns->first;	

		//찾으려는 오브젝트.
		if (pTarget == pTemp)
		{
			m_SelectedList.erase(it);		//delete in List
			bResult = true;					//결과: 찾음.
			break;
		}
		
	}

	return bResult;
}


size_t DrawInsManager::GetSizeOfSelected()
{
	return m_SelectedList.size();
}



void DrawInsManager::SetSelectedPrvQuaternion()
{
	size_t size = m_SelectedList.size();
	if (size <= 0) return;

	for (size_t i = 0; i < size; i++)
	{
		object* pCollider = m_SelectedList.at(i)->first;
		pCollider->prevRot = pCollider->quaternion;
	}
}



void DrawInsManager::ChangeMaterialSelected(MATERIAL* pMat)
{
	int newIndex = pMat->index;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		object* pObj		 = pDrawIns->first;
		HInstanceData* pData = pDrawIns->second;

		if (pObj->matIndex != newIndex)
		{
			pObj->matIndex = newIndex;
			if (newIndex == 0)
			{
				pData->SetShaderFlag(ShaderType::COLORCHIP);
				pData->SetMaterial(NULL, 0);
				swprintf_s(pObj->matName, L"colorchip");
			}
			else
			{
				pData->SetShaderFlag(ShaderType::DEFAULT);
				pData->SetMaterial(pMat->hMat, 0);
				swprintf_s(pObj->matName, CString(pMat->matName.GetBuffer()) );
			}
			
		}
	}
}


DRAW_INSTANCE* DrawInsManager::GetDrawIns(object* pObj)
{
	DRAW_INSTANCE* pResult = NULL;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		if (pDrawIns->first == pObj)
		{
			pResult = pDrawIns;
			break;
		}
	}

	return pResult;
}

bool DrawInsManager::GetCenterPos(DirectX::XMFLOAT3* pOut)
{
	bool bResult = true;
	DirectX::XMFLOAT3 pos = {  };
	int size = static_cast<int>(m_SelectedList.size());

	if (size != 0)
	{
		std::vector<DRAW_INSTANCE*>::iterator it;
		for (int i = 0; i < size; i++)
		{
			pos.x += m_SelectedList[i]->first->pos.x;
			pos.y += m_SelectedList[i]->first->pos.y;
			pos.z += m_SelectedList[i]->first->pos.z;
		}

		pos.x = pos.x / size;
		pos.y = pos.y / size;
		pos.z = pos.z / size;
	}


	if (pOut != NULL)
	{
		pOut->x = pos.x;
		pOut->y = pos.y;
		pOut->z = pos.z;
	}
	else
	{
		bResult = false;
	}

	return bResult;
}

object* DrawInsManager::GetObjectByIns(__in HInstanceData* hIns, __out DRAW_INSTANCE* pOut)
{
	object* pObj = NULL;

	DrawInsList::iterator _i = std::find_if(m_List.begin(), m_List.end(), comp_hGraphic(hIns));
	if (_i != m_List.end())
	{
		DRAW_INSTANCE* pDrawIns = *_i;
		pObj = pDrawIns->first;

		//memcpy(&pOut, &(*_i), sizeof(DRAW_INSTANCE));
		pOut->first = pDrawIns->first;
		pOut->second = pDrawIns->second;
	}

	return pObj;
}



HInstanceData* DrawInsManager::GetInsByObject(object* pObj, DRAW_INSTANCE* pOut)
{
	HInstanceData* hTarget = NULL;

	DrawInsList::iterator _i = std::find_if(m_List.begin(), m_List.end(), comp_obj(pObj));
	if (_i != m_List.end())
	{
		DRAW_INSTANCE* pDrawIns = *_i;
		hTarget = pDrawIns->second;
		if (pOut != NULL)
		{
			pOut->first  = pDrawIns->first;
			pOut->second = pDrawIns->second;
		}
		//*pOut = *_i;
	}
	else
	{
		pOut = NULL;
	}

	return hTarget;
}

void DrawInsManager::MoveSelected(float dx, float dy, float dz)
{
	size_t size = m_SelectedList.size();

	if (size <= 0)
		return;

	for (int i = 0; i < static_cast<int>(size); i++)
	{
		object* pObj = m_SelectedList[i]->first;
		pObj->pos.x += dx;
		pObj->pos.y += dy;
		pObj->pos.z += dz;
	}

}

void DrawInsManager::ReScaleSelected(float dx, float dy, float dz)
{
	size_t size = m_SelectedList.size();
	float min = 0.1f;

	if (size <= 0)
		return;

	for (int i = 0; i < static_cast<int>(size); i++)
	{
		m_SelectedList[i]->first->scale.x += dx;
		if (m_SelectedList[i]->first->scale.x < min)
		{
			m_SelectedList[i]->first->scale.x = min;
		}

		m_SelectedList[i]->first->scale.y += dy;
		if (m_SelectedList[i]->first->scale.y < min)
		{
			m_SelectedList[i]->first->scale.y = min;
		}

		m_SelectedList[i]->first->scale.z += dz;
		if (m_SelectedList[i]->first->scale.z < min)
		{
			m_SelectedList[i]->first->scale.z = min;
		}
		//UpdateTM(m_SelectedList[i].first);
	}
}

void DrawInsManager::ReScaleSelectedByRatio(float ratio)
{
	float r = ratio;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pObj = pIns->first;
		pObj->scale.x = pObj->scale.x * ratio;
		pObj->scale.y = pObj->scale.y * ratio;
		pObj->scale.z = pObj->scale.z * ratio;
		//UpdateTM(pObj);
	}
}

void DrawInsManager::ReScaleSelectedByRatioX(float ratio)
{
	float r = ratio;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pObj = pIns->first;
		pObj->scale.x = pObj->scale.x * ratio;
	}
}

void DrawInsManager::ReScaleSelectedByRatioY(float ratio)
{
	float r = ratio;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pObj = pIns->first;
		pObj->scale.y = pObj->scale.y * ratio;
	}
}

void DrawInsManager::ReScaleSelectedByRatioZ(float ratio)
{
	float r = ratio;
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		DRAW_INSTANCE* pIns = *it;
		object* pObj = pIns->first;
		pObj->scale.z = pObj->scale.z * ratio;
	}
}

void DrawInsManager::RotateSelected(DirectX::XMFLOAT4& quaternion)
{
	size_t size = m_SelectedList.size();
	if (size <= 0)	return;

	for (size_t i = 0; i < size; i++)
	{
		object* pObj = m_SelectedList.at(i)->first;

		DirectX::XMFLOAT4 prvRot = pObj->prevRot;
		DirectX::XMVECTOR vPrevRot = DirectX::XMVectorSet(prvRot.x, prvRot.y, prvRot.z, prvRot.w);
		DirectX::XMVECTOR vQuaternion = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		DirectX::XMVECTOR vResult = DirectX::XMQuaternionMultiply(vPrevRot, vQuaternion);
		DirectX::XMStoreFloat4(&pObj->quaternion, vResult);
	}

}

void DrawInsManager::EditRegenIndexDefault(int regenColliderIndex)
{
	//콜라이이더가 삭제될 경우. 해당 콜라이더의 인덱스를 참조하고 있는 객체들의 인덱스를 -1로 변경 시켜준다.
	std::vector<DRAW_INSTANCE*>::iterator it;
	for (it = m_List.begin(); it != m_List.end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		object* pObj = pDrawIns->first;
		if (pObj->regenIndex == regenColliderIndex)
		{
			pObj->regenIndex = -1;
		}
	}
}


std::vector<DRAW_INSTANCE*>* DrawInsManager::GetDrawInsList()
{
	return &m_List;
}

std::vector<DRAW_INSTANCE*>* DrawInsManager::GetSelectedList()
{
	return &m_SelectedList;
}

