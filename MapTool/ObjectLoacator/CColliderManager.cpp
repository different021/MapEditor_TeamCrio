#include "pch.h"
#include <algorithm>
#include "CColliderManager.h"
#include "EngineInterface.h"
#include "../src/Defines.h"
#include "CColliderManager.h"
#include "MapUtil.h"

HEngine_DX12_3D* pEngine = NULL;
CColliderManager* CColliderManager::m_pInstance = NULL;
int CColliderManager::m_iInstanceCounter = 0;

CColliderManager::CColliderManager()
{
}

CColliderManager::~CColliderManager()
{
	
}

CColliderManager* CColliderManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CColliderManager;
	}

	if (pEngine == NULL)
	{
		pEngine = HEngine_DX12_3D::GetInstance();
	}

	m_iInstanceCounter++;
	return m_pInstance;
}

void CColliderManager::DeleteInstance()
{
	m_iInstanceCounter--;
	if (m_iInstanceCounter <= 0)
	{
		DeleteColliderList();
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CColliderManager::SetSelectMode(eSELECT_MODE mode)
{
	if (mode > eSELECT_MAX) return;
	m_selectMode = mode;
}

void CColliderManager::Update()
{
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		collider* pCol = it->first;
		HSimplePrimitive* pPri = it->second;
		if (pCol == NULL) continue;
		if(pPri == NULL) continue;

		pCol->GetTM(pPri->worldTM);
	}
}

collider* CColliderManager::CreateCollider()
{
	collider* pCollider = new collider;
	pCollider->index = m_HighestIndex;
	m_HighestIndex++;

	return pCollider;
}

collider* CColliderManager::CreateColliderArray(int numberOfCollider)
{
	collider* pColList = new collider[numberOfCollider];
	return pColList;
}

int CColliderManager::GetHighestColliderIndex()
{
	int max = 0;
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		collider* pCol = it->first;
		max = (max > pCol->index) ? max : pCol->index;
	}

	return max;
}

void CColliderManager::SetIndexWhenYouLoad()
{
	m_HighestIndex = GetHighestColliderIndex() + 1;
}

const std::vector<COLLIDER>* CColliderManager::GetList()
{
	return &m_ColliderList;
}

const std::vector<COLLIDER>* CColliderManager::GetSelected()
{
	return &m_SelectedList;
}

void CColliderManager::DeleteColliderList()
{
	std::vector<COLLIDER>::iterator it;

	for (it = m_ColliderList.begin(); it != m_ColliderList.end();)
	{
		delete it->first;
		it->first = NULL;

		it->second->Delete();

		it = m_ColliderList.erase(it);
	}

}

void CColliderManager::EnableSelect(bool bSelect)
{
	m_bSelectEnable = bSelect;
	if (!m_bSelectEnable) m_SelectedList.clear();
}

bool CColliderManager::IsEnableColliderSelect()
{
	return m_bSelectEnable;
}

bool compare(COLLIDER& pCol1, COLLIDER& pCol2)
{
	eCOLLIDER_TYPE type1 = pCol1.first->type;
	eCOLLIDER_TYPE type2 = pCol2.first->type;
	return static_cast<int>(type1) < static_cast<int>(type2);
}

void CColliderManager::Sort()
{
	std::sort(m_ColliderList.begin(), m_ColliderList.end(), compare);
}


/*
	[PARAMETERS]
	  mouseX : 스크린 좌표계에서 마우스 X좌표
	  mouseY : 스크린 좌표계에서 마우스 Y좌표
	  ScreenWidth : 스크린 가로
	  ScreenHeight: 스크린 세로

	[return value]
	  선택된 collider 포인터 
	  없을 경우 nullptr. 
	  가장 가까운것만 찾아준다.(개선 사항)
*/
collider* CColliderManager::Picking(unsigned int mouseX, unsigned int mouseY, int screenWidth, int screenHeight)
{
	collider* pResult = NULL;							//리턴 값
	int vectorSize = m_ColliderList.size();				//전체 콜라이더 수 -> 전체 콜라이더를 검사한다. 맵 사이즈가 커질 경우 수정 고려
	if (vectorSize == 0) return pResult;				//충돌체 0개 -> 계산없이 리턴

	Camera* pCam = pEngine->GetCamera();				//인터페이스화 된 cam을 파라미터로 받을 것.
	DirectX::XMFLOAT4X4 mProj = pCam->GetProj4x4f();	//프로젝션 행렬
	
	//광선 구하기
	int width  = screenWidth;		//스크린 사이즈
	int height = screenHeight;
	float viewX = ( ( 2.f * mouseX / width ) - 1.f) / mProj(0, 0);		//unProjection (결과로 뷰좌표를 얻게 된다.)
	float viewY = ( (-2.f * mouseY / height) + 1.f) / mProj(1, 1);		//unProjection (Screen Y 좌표계는 아래로 내려갈수록 Y값 증가 -> 기존 월드와 반대이다.)
	
	//w값? 벡터인지, 포지션인지 구분하는 기준이 됨.
	//Position. = 1.f; 
	//Direction.w = 0.f; direction = vEnd - vStart ( vEnd.w - vStart.w 로 인해 0값이 됨.)
	DirectX::XMVECTOR vCamPos_view = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);		//뷰공간은 카메라가 중심인 공간(0, 0, 0, 1)
	DirectX::XMVECTOR vRayDir_view = XMVectorSet(viewX, viewY, 1.0f, 0.0f);		//뷰공간상의 광선

	//월드 변환
	XMMATRIX mView = pCam->GetView();					//카메라 공간으로 변환하기 위한 view행렬
	XMVECTOR det = XMMatrixDeterminant(mView);			//역행렬이 존재하는지 확인하는 판별식
	XMMATRIX invView = XMMatrixInverse(&det, mView);	//view 역행렬

	XMVECTOR mCam_world = XMVector3TransformCoord(vCamPos_view, invView);		//viwe역행렬을 곱해서 월드 포지션을 구한다.(unView)
	XMVECTOR mRay_world = XMVector3TransformNormal(vRayDir_view, invView);		//unView
	mRay_world = XMVector3Normalize(mRay_world);								//정규화

	float fMin = FLT_MAX;	//1.175494351 E - 38 (가장 가까운 콜라이더의 거리를 저장하기 위한 변수)
	for (int i = 0; i < vectorSize; i++)
	{
		//월드 변환
		collider* col = m_ColliderList.at(i).first;
		
		DirectX::BoundingBox box = BoundingBox();				//상자의 중심 위치 구하기용.
		box.Center = col->pos;									//world Position
		box.Extents.x = col->size.x * 0.5f * col->scale.x;		//중심위치를 구하는 로직
		box.Extents.y = col->size.y * 0.5f * col->scale.y;
		box.Extents.z = col->size.z * 0.5f * col->scale.z;

		float lengthOfRay = 0;													//물체와의 거리를 리턴 받기 위한 변수.
		bool isHit = box.Intersects(mCam_world, mRay_world, lengthOfRay);		//박스와 벡터가 교차하는지 판별. DirectX boundingBox에서 제공하는 함수. 
		
		//가장 가까운 콜라이더를 찾는 과정.
		if (isHit == true)													
		{
			fMin = min(fMin, lengthOfRay);
			if (fMin == lengthOfRay)
			{
				//기존보다 가까우면 갱신. -> 가장 가까운 콜라이더를 리턴한다.
				pResult = col;
			}
		}
	}

	return pResult;
}

COLLIDER* CColliderManager::FindCollider(collider* pTarget)
{
	COLLIDER* pResult = NULL;
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		if (it->first == pTarget)
		{
			pResult = &*it;
		}
	}

	return pResult;
}

std::vector<COLLIDER>::iterator CColliderManager::FindColliderFromSelected(collider* pTarget)
{
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		if (it->first == pTarget)
			break;
	}

	return it;
}

size_t CColliderManager::AddSelected(collider* pObj)
{
	if (!m_bSelectEnable)	return m_SelectedList.size();

	COLLIDER* pTarget = NULL;
	std::vector<COLLIDER>::iterator it;
	if (pObj == NULL) goto lb_return;

	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		if (it->first == pObj)
			goto lb_return;
	}

	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		if (it->first == pObj)
		{
			m_SelectedList.push_back( std::make_pair(it->first, it->second) );
		}
	}

lb_return:
	return m_SelectedList.size();
}

size_t CColliderManager::AddSelected_public(collider* pObj)
{
	if (!m_bSelectEnable)	return m_SelectedList.size();

	switch (m_selectMode)
	{
	case eSELECT_ONE:
		AddSelected_OnlyOne(pObj);
		break;
	case eSELECT_MULTI:
		AddSelected_DeleteAlreadySelected(pObj);
		break;
	}

	return m_SelectedList.size();
}

size_t CColliderManager::AddSelected_OnlyOne(collider* pObj)
{
	if (!m_bSelectEnable)	return m_SelectedList.size();

	DeleteSelectedAll();
	AddSelected(pObj);

	return m_SelectedList.size();
}

size_t CColliderManager::AddSelected_DeleteAlreadySelected(collider* pObj)
{
	if (!m_bSelectEnable)	return m_SelectedList.size();
	
	if (pObj == NULL) return m_SelectedList.size();
	std::vector<COLLIDER>::iterator it = FindColliderFromSelected(pObj);
	
	if (it != m_SelectedList.end())
	{
		m_SelectedList.erase(it);
	}
	else
	{
		AddSelected(pObj);
	}
	
	return m_SelectedList.size();
}


void CColliderManager::DeleteSelectedAll()
{
	//m_SelectedList.clear();
	int size = m_SelectedList.size();
	for (int i = 0; i < size; i++)
	{
		m_SelectedList.pop_back();
	}
}


size_t CColliderManager::DeleteSelected(collider* pObj)
{
	std::vector<COLLIDER>::iterator it;
	if (pObj == NULL) goto lb_return;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		if (it->first == pObj)
		{
			m_SelectedList.erase(it);
			break;
		}
	}

lb_return:
	return m_SelectedList.size();
}

size_t CColliderManager::GetSizeOfSelected()
{
	return m_SelectedList.size();
}

void CColliderManager::SetSelectedPrvRot()
{
	size_t size = m_SelectedList.size();
	if (size <= 0) return;
	
	for (size_t i = 0; i < size; i++)
	{
		collider* pCollider = m_SelectedList.at(i).first;
		pCollider->prevRot = pCollider->quaternion;
	}
}

void CColliderManager::MoveSelected(__in float dx, __in float dy, __in float dz)
{
	size_t size = m_SelectedList.size();
	if (size == 0) return;
	for (size_t i = 0; i < size; i++)
	{
		m_SelectedList.at(i).first->pos.x += dx;
		m_SelectedList.at(i).first->pos.y += dy;
		m_SelectedList.at(i).first->pos.z += dz;
		//EditCollider(m_SelectedList.at(i).first);
	}
}

void CColliderManager::RotateSelected(DirectX::XMFLOAT4& quaternion)
{
	size_t size = m_SelectedList.size();
	if (size == 0) return;
	for (size_t i = 0; i < size; i++)
	{
		collider* pCollider = m_SelectedList.at(i).first;

		DirectX::XMFLOAT4 prvRot	= pCollider->prevRot;
		DirectX::XMVECTOR vPrevRot	= DirectX::XMVectorSet(prvRot.x, prvRot.y, prvRot.z, prvRot.w);
		DirectX::XMVECTOR vQuaternion	= DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		DirectX::XMVECTOR vResult	= DirectX::XMQuaternionMultiply(vPrevRot, vQuaternion);
		DirectX::XMStoreFloat4(&pCollider->quaternion, vResult);
		// 쿼터니언 변환을 rollPitchYall
		

		//EditCollider(pCollider);  //-> Update로.

	}
}

void CColliderManager::RescaleSelected(float dx, float dy, float dz)
{
	size_t size = m_SelectedList.size();
	if (size == 0) return;
	for (size_t i = 0; i < size; i++)
	{
		m_SelectedList.at(i).first->scale.x += dx;
		m_SelectedList.at(i).first->scale.y += dy;
		m_SelectedList.at(i).first->scale.z += dz;
		//EditCollider(m_SelectedList.at(i).first);		//-> Update
	}
}

void CColliderManager::ReScaleSelectedByRatio(float ratio)
{
	float r = ratio;
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		COLLIDER COL = *it;
		collider* pCol = COL.first;
		pCol->scale.x = pCol->scale.x * ratio;
		pCol->scale.y = pCol->scale.y * ratio;
		pCol->scale.z = pCol->scale.z * ratio;		
	}
}

void CColliderManager::RescaleSelectedByRatioX(float ratio)
{
	float r = ratio;
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		COLLIDER COL = *it;
		collider* pCol = COL.first;
		pCol->scale.x = pCol->scale.x * ratio;
	}
}

void CColliderManager::RescaleSelectedByRatioY(float ratio)
{
	float r = ratio;
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		COLLIDER COL = *it;
		collider* pCol = COL.first;
		pCol->scale.y = pCol->scale.y * ratio;
	}
}

void CColliderManager::RescaleSelectedByRatioZ(float ratio)
{
	float r = ratio;
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		COLLIDER COL = *it;
		collider* pCol = COL.first;
		pCol->scale.z = pCol->scale.z * ratio;
	}
}

collider* CColliderManager::GetLastSelected()
{
	if (m_SelectedList.size() == 0)	return NULL;

	return m_SelectedList.back().first;
}

DirectX::XMFLOAT3 CColliderManager::GetSelectedCenterPosition()
{
	DirectX::XMFLOAT3 pos = GetSumSelectedPosition();
	int size = static_cast<int>(m_SelectedList.size());
	
	if (size == 0) goto lb_return;
	
	pos.x = pos.x / size;
	pos.y = pos.y / size;
	pos.z = pos.z / size;

lb_return:
	return pos;
}

DirectX::XMFLOAT3 CColliderManager::GetSumSelectedPosition()
{
	DirectX::XMFLOAT3 totalPos = {};
	int size = static_cast<int>(m_SelectedList.size());
	if (size != 0)
	{
		std::vector<COLLIDER>::iterator it;
		for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
		{
			COLLIDER& pIns = *it;
			totalPos.x += pIns.first->pos.x;
			totalPos.y += pIns.first->pos.y;
			totalPos.z += pIns.first->pos.z;
		}
	}

	return 	totalPos;
}

void CColliderManager::GetColliderInRect(float ndcX1, float ndcY1, float ndcX2, float ndcY2)
{
	float xMax = max(ndcX1, ndcX2);
	float xMin = min(ndcX1, ndcX2);
	float yMax = max(ndcY1, ndcY2);
	float yMin = min(ndcY1, ndcY2);

	Camera* pCam = pEngine->GetCamera();
	DirectX::XMMATRIX mView = pCam->GetView();
	DirectX::XMMATRIX mProj = pCam->GetProj();

	//object에 관한 검사.
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		collider* pCol = it->first;
		DirectX::XMMATRIX mVP = mView * mProj;

		DirectX::XMFLOAT3 CenterPos = pCol->pos;
		DirectX::XMVECTOR vCenterPos = DirectX::XMLoadFloat3(&CenterPos);

		DirectX::XMFLOAT3 screenPos;
		DirectX::XMVECTOR pos = XMVector3TransformCoord(vCenterPos, mVP);
		DirectX::XMStoreFloat3(&screenPos, pos);

		bool isInRect = MapUtil::AABB(xMin, xMax, yMin, yMax, screenPos.x, screenPos.y);
		if (isInRect == true)
		{
			AddSelected(pCol);
		}
		else
		{
			DeleteSelected(pCol);
		}
	}
}

HRESULT CColliderManager::InsertColliderList(collider* pCollider)
{
	static int index = 0;
	WCHAR buf[MAX_LENGTH];
	ZeroMemory(buf, MAX_LENGTH * 2);

	collider* pFirst = pCollider;
	if (pFirst == NULL) return E_INVALIDARG;

 	HSimplePrimitive* pSecond = pEngine->CreateBox(pFirst->size, eWireFrame_Primitive);
	if (pSecond == NULL) return E_OUTOFMEMORY;

	
	pFirst->GetTM( (&pSecond->worldTM) );
	COLLIDER NewCollider = COLLIDER(pFirst, pSecond);
	m_ColliderList.push_back(NewCollider);

	return S_OK;
}

//대상 하나만 삭제
HRESULT CColliderManager::DeleteCollider(collider* pCollider)
{
	if (pCollider == NULL) return E_INVALIDARG;
	
	HRESULT hr = E_HANDLE;
	//COLLIDER* pCOL = FindCollider(pCollider);
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		if (it->first == pCollider)
		{
			delete it->first;			//collider
			it->first = NULL;

			it->second->Delete();		//Graphic Box instance

			m_ColliderList.erase(it);

			hr = S_OK;
			break;
		}
	}

	return S_OK;
}

HRESULT CColliderManager::EditCollider(collider* pCollider)
{
	if (pCollider == NULL) return E_INVALIDARG;

	HRESULT hr = E_HANDLE;
	std::vector<COLLIDER>::iterator it;
	for (it = m_ColliderList.begin(); it != m_ColliderList.end(); it++)
	{
		if (it->first == pCollider)
		{
			collider* pCol = it->first;
			HSimplePrimitive* pPri = it->second;
			
			pCol->GetTM(pPri->worldTM);

			hr = S_OK;
			break;
		}
	}

	return hr;
}


HRESULT CColliderManager::DeleteColliderInSelectedList()
{
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		collider* pTarget = it->first;
		DeleteCollider(pTarget);
	}
	m_SelectedList.clear();

	return S_OK;
}

HRESULT CColliderManager::DuplicateColliderInSelectedList()
{
	std::vector<collider*> newList;
	std::vector<COLLIDER>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		collider* pNewCollider = CreateCollider();
		it->first->CopyExceptIndex(pNewCollider);
		InsertColliderList(pNewCollider);
		newList.push_back(pNewCollider);
	}
	m_SelectedList.clear();

	std::vector<collider*>::iterator itr;
	for (itr = newList.begin(); itr != newList.end(); itr++)
	{
		collider* pC = *itr;
		AddSelected(pC);
	}
	newList.clear();

	return S_OK;
}

