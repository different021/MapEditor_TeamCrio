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
	  mouseX : ��ũ�� ��ǥ�迡�� ���콺 X��ǥ
	  mouseY : ��ũ�� ��ǥ�迡�� ���콺 Y��ǥ
	  ScreenWidth : ��ũ�� ����
	  ScreenHeight: ��ũ�� ����

	[return value]
	  ���õ� collider ������ 
	  ���� ��� nullptr. 
	  ���� �����͸� ã���ش�.(���� ����)
*/
collider* CColliderManager::Picking(unsigned int mouseX, unsigned int mouseY, int screenWidth, int screenHeight)
{
	collider* pResult = NULL;							//���� ��
	int vectorSize = m_ColliderList.size();				//��ü �ݶ��̴� �� -> ��ü �ݶ��̴��� �˻��Ѵ�. �� ����� Ŀ�� ��� ���� ���
	if (vectorSize == 0) return pResult;				//�浹ü 0�� -> ������ ����

	Camera* pCam = pEngine->GetCamera();				//�������̽�ȭ �� cam�� �Ķ���ͷ� ���� ��.
	DirectX::XMFLOAT4X4 mProj = pCam->GetProj4x4f();	//�������� ���
	
	//���� ���ϱ�
	int width  = screenWidth;		//��ũ�� ������
	int height = screenHeight;
	float viewX = ( ( 2.f * mouseX / width ) - 1.f) / mProj(0, 0);		//unProjection (����� ����ǥ�� ��� �ȴ�.)
	float viewY = ( (-2.f * mouseY / height) + 1.f) / mProj(1, 1);		//unProjection (Screen Y ��ǥ��� �Ʒ��� ���������� Y�� ���� -> ���� ����� �ݴ��̴�.)
	
	//w��? ��������, ���������� �����ϴ� ������ ��.
	//Position. = 1.f; 
	//Direction.w = 0.f; direction = vEnd - vStart ( vEnd.w - vStart.w �� ���� 0���� ��.)
	DirectX::XMVECTOR vCamPos_view = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);		//������� ī�޶� �߽��� ����(0, 0, 0, 1)
	DirectX::XMVECTOR vRayDir_view = XMVectorSet(viewX, viewY, 1.0f, 0.0f);		//��������� ����

	//���� ��ȯ
	XMMATRIX mView = pCam->GetView();					//ī�޶� �������� ��ȯ�ϱ� ���� view���
	XMVECTOR det = XMMatrixDeterminant(mView);			//������� �����ϴ��� Ȯ���ϴ� �Ǻ���
	XMMATRIX invView = XMMatrixInverse(&det, mView);	//view �����

	XMVECTOR mCam_world = XMVector3TransformCoord(vCamPos_view, invView);		//viwe������� ���ؼ� ���� �������� ���Ѵ�.(unView)
	XMVECTOR mRay_world = XMVector3TransformNormal(vRayDir_view, invView);		//unView
	mRay_world = XMVector3Normalize(mRay_world);								//����ȭ

	float fMin = FLT_MAX;	//1.175494351 E - 38 (���� ����� �ݶ��̴��� �Ÿ��� �����ϱ� ���� ����)
	for (int i = 0; i < vectorSize; i++)
	{
		//���� ��ȯ
		collider* col = m_ColliderList.at(i).first;
		
		DirectX::BoundingBox box = BoundingBox();				//������ �߽� ��ġ ���ϱ��.
		box.Center = col->pos;									//world Position
		box.Extents.x = col->size.x * 0.5f * col->scale.x;		//�߽���ġ�� ���ϴ� ����
		box.Extents.y = col->size.y * 0.5f * col->scale.y;
		box.Extents.z = col->size.z * 0.5f * col->scale.z;

		float lengthOfRay = 0;													//��ü���� �Ÿ��� ���� �ޱ� ���� ����.
		bool isHit = box.Intersects(mCam_world, mRay_world, lengthOfRay);		//�ڽ��� ���Ͱ� �����ϴ��� �Ǻ�. DirectX boundingBox���� �����ϴ� �Լ�. 
		
		//���� ����� �ݶ��̴��� ã�� ����.
		if (isHit == true)													
		{
			fMin = min(fMin, lengthOfRay);
			if (fMin == lengthOfRay)
			{
				//�������� ������ ����. -> ���� ����� �ݶ��̴��� �����Ѵ�.
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
		// ���ʹϾ� ��ȯ�� rollPitchYall
		

		//EditCollider(pCollider);  //-> Update��.

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

	//object�� ���� �˻�.
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

//��� �ϳ��� ����
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

