#include "pch.h"
#include "CWaveManager.h"
#include <EngineInterface.h>


int CWaveManager::m_InstanceCounter = 0;
CWaveManager* CWaveManager::m_pInstance = nullptr;

CWaveManager::CWaveManager()
{
}

CWaveManager::~CWaveManager()
{
	CleanupWaves();
}

bool CWaveManager::IsExistInList(WAVE* pWave, const std::vector<WAVE*>& list)
{
	if (pWave == nullptr) return false;
	bool bResult = false;

	std::vector<WAVE*>::const_iterator it;
	for (it = list.begin(); it != list.end(); it++)
	{
		WAVE* pTemp = *it;
		if (pTemp == pWave)
		{
			bResult = true;
			break;
		}
	}

	return bResult;
}

HWaveData* CWaveManager::CreateGraphicInstance(waveData* pData)
{
	if (pData == NULL) return NULL;
	HWaveData* pGraphic = m_pEngine->CreateWave(pData->row, pData->col, pData->dx, pData->speed, pData->damping);
	return pGraphic;
}

CWaveManager* CWaveManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		//size_t size = sizeof(CWaveManager);
		m_pInstance = new CWaveManager;
	}
	m_InstanceCounter++;
	return m_pInstance;
}
void CWaveManager::Release()
{
	m_InstanceCounter--;
	if (m_InstanceCounter < 1)	//(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}	
}

void CWaveManager::CleanupWaves()
{
	std::vector<WAVE*>::iterator it;
	for (it = m_WaveList.begin(); it != m_WaveList.end(); )
	{
		WAVE* pWave = *it;

		if (pWave != NULL)
		{
			delete pWave;
			pWave = NULL;
		}
		
		it = m_WaveList.erase(it);
	}
}

void CWaveManager::Initialize()
{
	m_pEngine = HEngine_DX12_3D::GetInstance();
}

void CWaveManager::Update()
{
	ReCreate();
	std::vector<WAVE*>::iterator it;
	for (it = m_WaveList.begin(); it != m_WaveList.end(); it++)
	{
		WAVE* pWave = *it;
		pWave->Update();
	}
}

WAVE* CWaveManager::CreateWave(waveData* pSrc)
{
	WAVE* pReturn	 = new WAVE;				//리턴값.

	memset(pReturn, 0, sizeof(WAVE));
	//waveData* pInfo = &pReturn->info;			//
	
	if (pSrc == nullptr)
	{
		//default : 128, 128, 1.0f, 0.03f, 4.0f, 0.2f 
		pReturn->info.row		= 128;
		pReturn->info.col		= 128;
		pReturn->info.dx		= 1.f;
		pReturn->info.speed	= 4.0f;
		pReturn->info.damping	= 0.2f;
		pReturn->info.pos		= {0.f, 0.f, 0.f};
		pReturn->info.rot		= {0.f, 0.f, 0.f};
		pReturn->info.scale		= {1.f, 1.f, 1.f};

	}
	else
	{
		memcpy(&pReturn->info, pSrc, sizeof(waveData));
	}
	
	HWaveData* pGraphic = CreateGraphicInstance(pSrc);
	pReturn->pGraphicInstance = pGraphic;

	AddWave(pReturn);

	return pReturn;
}

bool CWaveManager::DeleteWaveInDeleteList()
{
	size_t size = m_DeleteList.size();
	if (size > 0)
	{
		std::vector<WAVE*>::iterator it;
		for (it = m_DeleteList.begin(); it != m_DeleteList.end(); it++)
		{
			WAVE* pTemp = *it;
			DeleteWave(pTemp);
		}
		//일단 모두 순회하고, 마지막에 리스트 초기화.
		m_DeleteList.clear();
	}

	return false;
}

bool CWaveManager::DeleteWave(WAVE* pWave)
{
	bool bResult = false;
	if (pWave == NULL) return bResult;
	std::vector<WAVE*>::iterator it;
	for (it = m_WaveList.begin(); it != m_WaveList.end(); it++)
	{
		WAVE* pTemp = *it;
		if (pTemp == pWave)
		{
			delete pTemp;
			pTemp = NULL;

			m_WaveList.erase(it);
			bResult = true;
			break;
		}
	}

	return bResult;
}

bool CWaveManager::AddDeleteList(WAVE* pWave)
{
	bool bResult = false;
	if (pWave != NULL) bResult = true;
	m_DeleteList.push_back(pWave);

	return bResult;
}

void CWaveManager::AddWave(WAVE* pDest)
{
	if (pDest == nullptr) return;
	bool isExsit = IsExistInList(pDest, m_WaveList);	//리스트에 잇는지 검사.
	
	if(isExsit != true)	
		m_WaveList.push_back(pDest);		//리스트에 없으면 추가.
}

void CWaveManager::EditWave(WAVE* pDest, waveData* pSrc)
{
	if (pDest->info.row != pSrc->row)
	{
		AddReCreateList(pDest);
	}
	else if (pDest->info.col != pSrc->col)
	{
		AddReCreateList(pDest);
	}
	else if (pDest->info.dx != pSrc->dx)
	{
		AddReCreateList(pDest);
	}
	else if (pDest->info.speed != pSrc->speed)
	{
		AddReCreateList(pDest);
	}
	else if (pDest->info.damping != pSrc->damping)
	{
		AddReCreateList(pDest);
	}

	memcpy(&pDest->info, pSrc, sizeof(waveData));
}



int CWaveManager::AddSelected(WAVE* pWave)
{
	if (pWave == NULL) return m_SelectedList.size();	//잘못된 파라미터.
	
	bool bIsIn = IsExistInList(pWave, m_WaveList);
	if (bIsIn == false)		//선택된적 없다.
	{
		m_SelectedList.push_back(pWave);
	}

	return m_SelectedList.size();
}

int CWaveManager::AddSelectedOnlyOne(WAVE* pWave)
{
	m_SelectedList.clear();

	if (pWave != NULL) 
	{
		m_SelectedList.push_back(pWave);
	}
	
	return static_cast<int>(m_SelectedList.size());
}

//int CWaveManager::DeleteSelected(WAVE* pWave)
//{
//	if (pWave == NULL) return m_SelectedList.size();
//
//	std::vector<WAVE*>::iterator recreate_it;
//	for (recreate_it = m_SelectedList.begin(); recreate_it != m_SelectedList.end(); recreate_it++)
//	{
//		WAVE* pTemp = *recreate_it;
//		if (pWave == pTemp)
//		{
//			m_SelectedList.erase(recreate_it);
//			break;
//		}
//	}
//
//	return m_SelectedList.size();
//}

void CWaveManager::DeleteSelectedAll()
{
	m_SelectedList.clear();
}

const std::vector<WAVE*>* CWaveManager::GetSelectedList()
{
	return &m_SelectedList;
}


DirectX::XMFLOAT3 CWaveManager::GetSelectedCenterPos()
{
	DirectX::XMFLOAT3 pos = {};

	int size = m_SelectedList.size();
	if (size == 0) goto lb_return;

	for (int i = 0; i < size; i++)
	{
		pos.x += m_SelectedList.at(i)->info.pos.x;
		pos.y += m_SelectedList.at(i)->info.pos.y;
		pos.z += m_SelectedList.at(i)->info.pos.z;
	}

	pos.x = pos.x / static_cast<float>(size);
	pos.y = pos.y / static_cast<float>(size);
	pos.z = pos.z / static_cast<float>(size);

lb_return:
	return pos;
}

int CWaveManager::GetNumberOfSelected()
{
	return static_cast<int>(m_SelectedList.size());
}

void CWaveManager::MoveSelected(float x, float y, float z)
{

	std::vector<WAVE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		WAVE* pWave = *it;
		pWave->AddPos(x, y, z);
		//pWave->Update();
	}
}

void CWaveManager::RotateSelected(DirectX::XMFLOAT4& quaternion)
{
	size_t size = m_SelectedList.size();
	if (size == 0) return;

	for (size_t i = 0; i < size; i++)
	{
		WAVE* pWave = m_SelectedList.at(i);

		DirectX::XMFLOAT4 prvRot		= pWave->info.prevRot;
		DirectX::XMVECTOR vPrevRot		= DirectX::XMVectorSet(prvRot.x, prvRot.y, prvRot.z, prvRot.w);
		DirectX::XMVECTOR vQuaternion	= DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		DirectX::XMVECTOR vResult		= DirectX::XMQuaternionMultiply(vPrevRot, vQuaternion);
		DirectX::XMStoreFloat4(&pWave->info.quaternion, vResult);

		//pWave->Update();
	}
}

void CWaveManager::ReScaleSelectedByRatio(float ratio)
{
	float r = ratio;
	std::vector<WAVE*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		WAVE* pWave = *it;
		pWave->info.scale.x = pWave->info.scale.x * ratio;
		pWave->info.scale.y = pWave->info.scale.y * ratio;
		pWave->info.scale.z = pWave->info.scale.z * ratio;
	}
}

void CWaveManager::SetSelectedPrvRot()
{
	size_t size = m_SelectedList.size();
	if (size <= 0) return;

	for (size_t i = 0; i < size; i++)
	{
		WAVE* pWave = m_SelectedList.at(i);
		pWave->info.prevRot = pWave->info.quaternion;
	}
}

void CWaveManager::AddReCreateList(WAVE* pWave)
{
	if(pWave != NULL)
		m_ReCreateList.push_back(pWave);
}

void CWaveManager::ReCreate()
{
	std::vector<WAVE*>::iterator recreate_it, list_it;
	for (recreate_it = m_ReCreateList.begin(); recreate_it != m_ReCreateList.end(); recreate_it++)
	{
		//모두 순회 한뒤 리스트 클리어
		WAVE* pTarget = *recreate_it;		
		for (list_it = m_WaveList.begin(); list_it != m_WaveList.end(); list_it++)
		{
			WAVE* pTemp = *list_it;
			if (pTemp == pTarget)
			{
				ReCreateWave(pTemp);	//재생성
				break;
			}
		}
	}

	m_ReCreateList.clear();
}

WAVE* CWaveManager::ReCreateWave(WAVE* pWave)
{
	if (pWave == NULL) return NULL;
	if (pWave->pGraphicInstance != NULL)
	{
		//존재하는 그래픽 인스턴스 재생성.
		pWave->pGraphicInstance->Delete();
		pWave->pGraphicInstance = NULL;
	}

	//재생성.
	HWaveData* pGraphic = CreateGraphicInstance(&pWave->info);
	pWave->pGraphicInstance = pGraphic;

	return pWave;
}

WAVE* CWaveManager::GetLastSelected()
{
	WAVE* pResult = nullptr;
	if (m_SelectedList.size() == 0) goto lb_return;

	pResult = m_SelectedList.back();

lb_return:
	return pResult;
}

const std::vector<WAVE*>* CWaveManager::GetList()
{
	return &m_WaveList;
}



WAVE::WAVE()
{
}

WAVE::~WAVE()
{
	if (pGraphicInstance != nullptr)
	{
		pGraphicInstance->Delete();
		pGraphicInstance = NULL;
	}
		
}

void WAVE::Update()
{
	pGraphicInstance->worldTM = info.GetTm();
}

void WAVE::AddPos(float dx, float dy, float dz)
{
	info.pos.x += dx;
	info.pos.y += dy;
	info.pos.z += dz;
}

void WAVE::Rotation(DirectX::XMFLOAT4& quaternion)
{

}
