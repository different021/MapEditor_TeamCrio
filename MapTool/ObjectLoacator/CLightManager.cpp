#include "pch.h"
#include "EngineInterface.h"  
#include "CLightManager.h"
#include "MapUtil.h"

CLightManager* CLightManager::m_pInstance = NULL;     //매니저 인스턴스
int			   CLightManager::m_iInstanceCounter = 0;	    //매니저 인스턴스가 참조된 숫자.

void Light::DeleteGraphicInstance()
{
	if (m_pGraphicData)
	{
		m_pGraphicData->Delete();
		m_pGraphicData = NULL;
	}
	if (m_pPositionBox)
	{
		m_pPositionBox->Delete();
		m_pPositionBox = NULL;
	}		
	if (m_pDirectionLine)
	{
		m_pDirectionLine->Delete();
		m_pDirectionLine = NULL;
	}
		
}

//단순 카피. 이 내용으로 그래픽 인스턴스들을 업데이트 하기 위해서는 UpdateLightData를 사용하세요.
void Light::OutData(lightData& dest)
{
	dest.type = m_LightData.type;
	dest.strength[0] = m_LightData.strength[0];
	dest.strength[1] = m_LightData.strength[1];
	dest.strength[2] = m_LightData.strength[2];
	dest.falloffStart = m_LightData.falloffStart;
	dest.direction[0] = m_LightData.direction[0];
	dest.direction[1] = m_LightData.direction[1];
	dest.direction[2] = m_LightData.direction[2];
	dest.falloffEnd = m_LightData.falloffEnd;
	dest.position[0] = m_LightData.position[0];
	dest.position[1] = m_LightData.position[1];
	dest.position[2] = m_LightData.position[2];
	dest.spotPower = m_LightData.spotPower;
	wsprintfW(dest.name, m_LightData.name);
}

void Light::CopyData(lightData& newData)
{
	m_LightData.type = newData.type;
	m_LightData.strength[0] = newData.strength[0];
	m_LightData.strength[1] = newData.strength[1];
	m_LightData.strength[2] = newData.strength[2];
	m_LightData.falloffStart = newData.falloffStart;
	m_LightData.direction[0] = newData.direction[0];
	m_LightData.direction[1] = newData.direction[1];
	m_LightData.direction[2] = newData.direction[2];
	m_LightData.falloffEnd = newData.falloffEnd;
	m_LightData.position[0] = newData.position[0];
	m_LightData.position[1] = newData.position[1];
	m_LightData.position[2] = newData.position[2];
	m_LightData.spotPower = newData.spotPower;
	wsprintfW(m_LightData.name, newData.name);
}

void Light::Update()
{
	UpdateLightData();
	UpdateBox();
	UpdateLine();
}

//입력받은 내용으로 그래픽 데이터 업데이트.
void Light::UpdateLightData()
{
	if (m_pGraphicData != NULL)
	{
		//그래픽 인스턴스는 내가 컨트롤할 수 있는 것이 아니라 memcpy가 위험하다.
		m_pGraphicData->strength.x = m_LightData.strength[0];
		m_pGraphicData->strength.y = m_LightData.strength[1];
		m_pGraphicData->strength.z = m_LightData.strength[2];

		m_pGraphicData->falloffStart = m_LightData.falloffStart;

		m_pGraphicData->direction.x = m_LightData.direction[0];
		m_pGraphicData->direction.y = m_LightData.direction[1];
		m_pGraphicData->direction.z = m_LightData.direction[2];

		m_pGraphicData->falloffEnd = m_LightData.falloffEnd;

		m_pGraphicData->position.x = m_LightData.position[0];
		m_pGraphicData->position.y = m_LightData.position[1];
		m_pGraphicData->position.z = m_LightData.position[2];

		m_pGraphicData->spotPower = m_LightData.spotPower;
	}
}

void Light::UpdateBox()
{
	if (m_pPositionBox != NULL)
	{
		m_pPositionBox->worldTM = GetTM();
	}
}

void Light::UpdateLine()
{
	if (m_pDirectionLine != NULL)
	{
		Vector3 direction = m_pGraphicData->direction;
		direction.Normalize();

		m_pDirectionLine->color = Color(m_LightData.strength[0], m_LightData.strength[1], m_LightData.strength[2]);
		m_pDirectionLine->dots[0] = { m_LightData.position[0], m_LightData.position[1], m_LightData.position[2] };
		m_pDirectionLine->dots[1] = m_pDirectionLine->dots[0] + (direction * 30.f);	//방향에 따라 길이가 좀 달라지지만 그려려니 하자. 저글링도 대각선 사거리가 더 길었다.
	}
}

XMMATRIX Light::GetTM()
{
	XMFLOAT3 pos = {};
	pos.x = m_LightData.position[0];
	pos.y = m_LightData.position[1];
	pos.z = m_LightData.position[2];

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(oiler.x, oiler.y, oiler.z);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(10.f, 10.f, 10.f);

	DirectX::XMMATRIX mTM = mScale * mTrans;
	DirectX::XMStoreFloat4x4(&m_pPositionBox->worldTM, mTM);

	return mTM;
}

Light::~Light()
{
	DeleteGraphicInstance();
}


CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
	ClearList();
}

void CLightManager::CreatePosBox(Light* pLight, ShaderType_Primitive type)
{
	if (pLight->m_pPositionBox != NULL)
	{
		pLight->m_pPositionBox->Delete();
		pLight->m_pPositionBox = NULL;
	}

	HSimplePrimitive* pPosBox = m_pEngine->CreateBox(Vector3(1.f, 1.f, 1.f), type);
	pLight->m_pPositionBox = pPosBox;
	//UpdateBoxPosTM(pLight, &pPosBox->worldTM);
	//UpdateLine(pLight);
}

void CLightManager::CreateDirectionLine(Light* pLight, ShaderType_Primitive type)
{
	Vector3 direction = pLight->m_pGraphicData->direction;
	direction.Normalize();

	if (pLight->m_pDirectionLine != NULL)
	{
		pLight->m_pDirectionLine->Delete();
		pLight->m_pDirectionLine = NULL;
	}

	HProceduralGeometry_line* pDirectionLine = m_pEngine->CreateLine(eNoOption_Primitive);

	//Update로 이동.
	pDirectionLine->color = Color(pLight->m_LightData.strength[0], pLight->m_LightData.strength[1], pLight->m_LightData.strength[2]);
	pDirectionLine->dots[0] = { pLight->m_LightData.position[0], pLight->m_LightData.position[1],pLight->m_LightData.position[2] };
	pDirectionLine->dots[1] = pDirectionLine->dots[0] + direction * 10.f;
	pLight->m_pDirectionLine = pDirectionLine;
}

CLightManager* CLightManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CLightManager;
	}
	m_iInstanceCounter++;

	return m_pInstance;
}

void CLightManager::Release()
{
	m_iInstanceCounter--;
	//if (m_iInstanceCounter < 1)
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	if (m_iInstanceCounter > 0)
	{
		//LightDlg가 Viewer보다 늦게 제거되어 생기는 문제. 
		//Viwer는 포인트에 할당 변수지만
		//LightDlg는 Center에 그냥 박혀있다.
		//Center가 사라질때 Release가 콜.
		//현재 사용중인 그래픽 엔진은 메모리 해제할때 내부 인스턴스를 알아서 제거한다. 
		OutputDebugStringW(L"LightManager::Release() call smaller than create\n");
	}
}

void CLightManager::SetSelectMode(eSELECT_MODE mode)
{
	m_selectMode = mode;
}

void CLightManager::SetEngine(HEngine_DX12_3D* pEngine)
{
	m_pEngine = pEngine;
}

void CLightManager::SetSelectEnabled(bool bCanISelect)
{
	m_bSelectEnable = bCanISelect;
}

bool CLightManager::IsEnableLightSelect()
{
	return m_bSelectEnable;
}

//pushback 하지말것. 내부에서 해주고 있음.
Light* CLightManager::CreateLight(lightData& lightData)
{
	Light* pResult = new Light;
	ZeroMemory(pResult, sizeof(Light));
	pResult->CopyData(lightData);

	LightType type = GetLightType(pResult->m_LightData.type);
	HLightData* pGrphicData = m_pEngine->CreateLight(type);
	if (pGrphicData == NULL)
	{
		OutputDebugString(L"[Fail] CreateLight\n");
		return NULL;
	}

	pResult->m_pGraphicData = pGrphicData;
	pResult->UpdateLightData();

	CreatePosBox(pResult, eNoOption_Primitive);
	CreateDirectionLine(pResult, eNoOption_Primitive);

	//리스트에 추가.
	m_LightList.push_back(pResult);

	return pResult;
}

void CLightManager::Update()
{
	//루프를 각각 돌지만. 일단은...
	UpdateLightType();		//빛 타입 최신화
	UpdatePrimitive();		//기하 정보 최신화(cube + line)
	UpdatePos();			//위치정보 최신화
}

void CLightManager::UpdatePos()
{
	std::vector<Light*>::iterator it;
	for (it = m_LightList.begin(); it != m_LightList.end(); it++)
	{
		Light* pTemp = *it;
		if (pTemp != NULL)
		{
			pTemp->Update();

		}
	}
}

void CLightManager::UpdatePrimitive()
{
	std::vector<CHANGE_PRIMITIVE>::iterator it;
	for (it = m_ChangePrimitiveList.begin(); it != m_ChangePrimitiveList.end(); it++)
	{
		CHANGE_PRIMITIVE light = *it;
		Light* pLight = light.first;
		ShaderType_Primitive prim = light.second;

		CreatePosBox(pLight, prim);
		//CreateDirectionLine(pLight, prim);
	}

	m_ChangePrimitiveList.clear();
}

void CLightManager::UpdateLightType()
{
	std::vector<CHANGE_LIGHTTYPE>::iterator it;
	for (it = m_ChangeTypeList.begin(); it != m_ChangeTypeList.end(); it++)
	{
		CHANGE_LIGHTTYPE light = *it;
		Light* pLight = light.first;
		LightType type = light.second;

		UpdateLightType(pLight, type);
	}

	m_ChangeTypeList.clear();
}

void CLightManager::AddChangePrimitiveList(Light* pLight, ShaderType_Primitive primitive)
{
	if (pLight == NULL) return;
	if (primitive > eAllOption_Primitive) return;

	m_ChangePrimitiveList.push_back(CHANGE_PRIMITIVE(pLight, primitive));
}


void CLightManager::UpdateLightType(Light* pLight, LightType type)
{
	if (pLight->m_pGraphicData != NULL)
	{
		pLight->m_pGraphicData->Delete();
		pLight->m_pGraphicData = NULL;
	}

	//LightType type = GetLightType(pLight->m_LightData.type);
	pLight->m_pGraphicData = m_pEngine->CreateLight(type);

}

void CLightManager::AddChangeLightTypeList(Light* pLight, LightType type)
{
	if(pLight != NULL)
		m_ChangeTypeList.push_back( CHANGE_LIGHTTYPE(pLight, type) );
}


LightType CLightManager::GetLightType(eLightType type)
{
	LightType typeResult = eDirectLight;
	if (type == eDirect)
	{
		typeResult = eDirectLight;
	}
	else if (type == ePoint)
	{
		typeResult = ePointLight;
	}
	else if (type)
	{
		typeResult = eSpotLight;
	}
	else
	{
		OutputDebugString(L"Light Type ERR\n");
		typeResult = eDirectLight;
	}

	return typeResult;
}




void CLightManager::AddDeleteList(Light* pLight)
{
	if (pLight != NULL)
	{
		m_DeleteList.push_back(pLight);
	}
		
}

void CLightManager::Delete(std::vector<Light*>* pList)
{
	std::vector<Light*>::iterator delete_it, list_it;
	for (delete_it = m_DeleteList.begin(); delete_it != m_DeleteList.end(); delete_it++)
	{
		Light* pTarget = *delete_it;
		for (list_it = m_LightList.begin(); list_it != m_LightList.end(); list_it++)
		{
			Light* pTemp = *list_it;
			if (pTemp == pTarget)
			{
				if (pList != NULL)
				{
					//Dialog Box 리스트에 업데이트할 내용.
					pList->push_back(pTemp);
				}

				delete pTemp;
				pTemp = NULL;

				m_LightList.erase(list_it);
				break;
			}
		}
	}

	m_DeleteList.clear();
}

void CLightManager::DelteInDelList(std::vector<Light*>* pList)
{
	std::vector<Light*>::iterator delete_it, list_it;
	for (delete_it = m_DeleteList.begin(); delete_it != m_DeleteList.end(); delete_it++)
	{
		Light* pTarget = *delete_it;
		for (list_it = m_LightList.begin(); list_it != m_LightList.end(); list_it++)
		{
			Light* pTemp = *list_it;
			if (pTemp == pTarget)
			{
				if (pList != NULL)
				{
					//Dialog Box 리스트에 업데이트할 내용.
					pList->push_back(pTemp);
				}

				delete pTemp;
				pTemp = NULL;

				m_LightList.erase(list_it);
				break;
			}
		}
	}

	m_DeleteList.clear();
}




void CLightManager::EditLight(HLightData* pTarget)
{
	if (pTarget == NULL) return;
	Light* pLight = FindLight(pTarget);
	if (pLight == NULL) return;

	pLight->m_LightData.strength[0] = pTarget->strength.x;
	pLight->m_LightData.strength[1] = pTarget->strength.y;
	pLight->m_LightData.strength[2] = pTarget->strength.z;

	pLight->m_LightData.falloffStart = pTarget->falloffStart;
	pLight->m_LightData.falloffEnd = pTarget->falloffEnd;

	pLight->m_LightData.direction[0] = pTarget->direction.x;
	pLight->m_LightData.direction[1] = pTarget->direction.y;
	pLight->m_LightData.direction[2] = pTarget->direction.z;

	pLight->m_LightData.position[0] = pTarget->position.x;
	pLight->m_LightData.position[1] = pTarget->position.y;
	pLight->m_LightData.position[2] = pTarget->position.z;

	pLight->m_LightData.spotPower = pTarget->spotPower;
}

Light* CLightManager::FindLight(HLightData* pTarget)
{
	std::vector<Light*>::iterator it;
	Light* pResult = NULL;
	for (it = m_LightList.begin(); it != m_LightList.end(); it++)
	{
		Light* pTemp = (*it);
		if (pTemp->m_pGraphicData == pTarget)
		{
			pResult = pTemp;
			break;
		}
	}
	return pResult;
}

void CLightManager::ClearList()
{
	m_ChangePrimitiveList.clear();
	m_ChangeTypeList.clear();

	std::vector<Light*>::iterator it;

	for (it = m_LightList.begin(); it != m_LightList.end();)
	{
		Light* pLight = *it;
		delete pLight;
		pLight = NULL;

		it = m_LightList.erase(it);
	}
}

void CLightManager::DuplicateSelected()
{
	if (m_Selected.size() == 0) return;
	std::vector<Light*> newList;
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end(); it++)
	{
		Light* pTemp = *it;
		Light* pLight = CreateLight(pTemp->m_LightData);

		newList.push_back(pLight);
	}
	EraseSelectedAll();

	for (it = newList.begin(); it != newList.end(); it++)
	{
		Light* pTemp = *it;
		AddSelected(pTemp);
	}
	newList.clear();
}

void CLightManager::AddSelected(Light* pLight)
{
	if (!m_bSelectEnable) return;
	if (pLight == NULL) return;

	bool isExist = IsInSelected(pLight);
	if (isExist == false)                   //선택한 것 중 없으면 추가.
	{
		//CreatePosBox(pLight, eWireFrame_Primitive);
		AddChangePrimitiveList(pLight, eNoOption_Primitive);
		m_Selected.push_back(pLight);
	}

}

void CLightManager::AddSelected_public(Light* pLight)
{
	if (!m_bSelectEnable) return;
	if (pLight == NULL) return;

	switch (m_selectMode)
	{
	case eSELECT_ONE:           //하나만 선택 가능
		ClearSelected();
		AddSelected(pLight);
		break;

	case eSELECT_MULTI:         //복수 선택 가능.
		AddSelected(pLight);
		break;

	default:
		break;
	}

}

void CLightManager::AddSelected_onlyOne(Light* pLight)
{
	if (!m_bSelectEnable) return;
	ClearSelected();
	AddSelected(pLight);
}

void CLightManager::DeleteInSelectedList(Light* pLight)
{
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end(); it++)
	{
		Light* pTemp = *it;
		if (pTemp == pLight)
		{
			//CreatePosBox(pLight, eNoOption_Primitive);
			AddChangePrimitiveList(pLight, eNoOption_Primitive);
			it = m_Selected.erase(it);
			break;
		}
	}
}


void CLightManager::DeleteSelectedLight()
{
	//선택 리스트에 있는 모든 라이트를 삭제.
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end();)
	{
		Light* pLight = *it;
		it = m_Selected.erase(it);
		AddDeleteList(pLight);
		//DeleteLight(pLight);
	}

}

void CLightManager::EraseSelectedAll()
{
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end();)
	{
		Light* pTemp = *it;
		//CreatePosBox(pTemp, eNoOption_Primitive);
		AddChangePrimitiveList(pTemp, eNoOption_Primitive);
		it = m_Selected.erase(it);
	}
}


void CLightManager::ClearSelected()
{
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end();)
	{
		Light* pTemp = *it;
		if (pTemp == NULL) continue;
		//CreatePosBox(pTemp, eNoOption_Primitive);
		AddChangePrimitiveList(pTemp, eNoOption_Primitive);
		it = m_Selected.erase(it);
	}

	m_Selected.clear();
}

bool CLightManager::IsInSelected(Light* pLight)
{
	bool bExist = false;
	std::vector<Light*>::iterator it;
	for (it = m_Selected.begin(); it != m_Selected.end(); it++)
	{
		Light* pTemp = *it;
		if (pTemp == pLight)
		{
			bExist = true;     //이미 선택한 항목중에 있다는 뜻.
			break;
		}
	}

	return bExist;
}


DirectX::XMFLOAT3 CLightManager::GetSelectedCenterPos()
{
	DirectX::XMFLOAT3 pos = {};

	int size = m_Selected.size();
	if (size == 0) goto lb_return;

	for (int i = 0; i < size; i++)
	{
		pos.x += m_Selected.at(i)->m_LightData.position[0];
		pos.y += m_Selected.at(i)->m_LightData.position[1];
		pos.z += m_Selected.at(i)->m_LightData.position[2];
	}

	//WCHAR buf[256];
	//memset(buf, 0, sizeof(buf));
	//wsprintf(buf, L"size = %d\n", size);
	//OutputDebugString(buf);

	pos.x = pos.x / static_cast<float>(size);
	pos.y = pos.y / static_cast<float>(size);
	pos.z = pos.z / static_cast<float>(size);

lb_return:
	return pos;
}

size_t CLightManager::GetNumberOfSelected()
{
	return m_Selected.size();
}




Light* CLightManager::Picking(unsigned int screenX, unsigned int screenY, int inWidth, int inHeight)
{
	int vectorSize = m_LightList.size();
	if (vectorSize == 0) return NULL;
	Camera* pCam = m_pEngine->GetCamera();
	DirectX::XMFLOAT4X4 mProj = pCam->GetProj4x4f();

	//광선 구하기
	int width = inWidth;		//받아올것.
	int height = inHeight;
	float vx = ((2.f * screenX / width) - 1.f) / mProj(0, 0);       //나눠주는것이 무슨 의미가 있었는데 까먹음.
	float vy = ((-2.f * screenY / height) + 1.f) / mProj(1, 1);

	//w값은?
	//Position = 1.f; 딱히 뺄셈이 아니라 남아 있음
	//Direction = 0.f; vEnd - vStart = w성분이 삭제됨.
	DirectX::XMVECTOR vCamPos_view = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
	DirectX::XMVECTOR vRayDir_view = XMVectorSet(vx, vy, 1.0f, 0.0f);;

	XMMATRIX mView = pCam->GetView();
	XMVECTOR det = XMMatrixDeterminant(mView);
	XMMATRIX invView = XMMatrixInverse(&det, mView);

	Light* pResult = NULL;

	float fMin = FLT_MAX;	//1.175494351 E - 38
	for (int i = 0; i < vectorSize; i++)
	{
		Light* plight = m_LightList.at(i);
		XMVECTOR mCam_world = XMVector3TransformCoord(vCamPos_view, invView);
		XMVECTOR mRay_world = XMVector3TransformNormal(vRayDir_view, invView);

		mRay_world = XMVector3Normalize(mRay_world);

		DirectX::BoundingBox box = BoundingBox();
		box.Center.x = plight->m_LightData.position[0];									//world Position
		box.Center.y = plight->m_LightData.position[1];
		box.Center.z = plight->m_LightData.position[2];

		box.Extents.x = 5.f;
		box.Extents.y = 5.f;
		box.Extents.z = 5.f;

		float lengthOfRay = 0;
		bool isHit = box.Intersects(mCam_world, mRay_world, lengthOfRay);
		if (isHit == true)
		{
			fMin = min(fMin, lengthOfRay);
			if (fMin == lengthOfRay)
			{
				pResult = plight;
			}
		}
	}

	return pResult;
}

void CLightManager::GetLightInRect(float ndcX1, float ndcY1, float ndcX2, float ndcY2)
{
	float xMax = max(ndcX1, ndcX2);
	float xMin = min(ndcX1, ndcX2);
	float yMax = max(ndcY1, ndcY2);
	float yMin = min(ndcY1, ndcY2);

	Camera* pCam = m_pEngine->GetCamera();
	DirectX::XMMATRIX mView = pCam->GetView();
	DirectX::XMMATRIX mProj = pCam->GetProj();

	//object에 관한 검사.
	std::vector<Light*>::iterator it;
	for (it = m_LightList.begin(); it != m_LightList.end(); it++)
	{
		Light* pLight = *it;
		DirectX::XMMATRIX mVP = mView * mProj;
		DirectX::XMFLOAT3 CenterPos = {};
		CenterPos.x = pLight->m_LightData.position[0];
		CenterPos.y = pLight->m_LightData.position[1];
		CenterPos.z = pLight->m_LightData.position[2];

		DirectX::XMVECTOR vCenterPos = DirectX::XMLoadFloat3(&CenterPos);

		DirectX::XMFLOAT3 screenPos;
		DirectX::XMVECTOR pos = XMVector3TransformCoord(vCenterPos, mVP);
		DirectX::XMStoreFloat3(&screenPos, pos);

		bool isInRect = MapUtil::AABB(xMin, xMax, yMin, yMax, screenPos.x, screenPos.y);
		if (isInRect == true)
		{
			AddSelected(pLight);
		}
		else
		{
			DeleteInSelectedList(pLight);
		}
	}
}

void CLightManager::MoveSelected(float x, float y, float z)
{
	size_t size = m_Selected.size();
	if (size == 0) return;
	for (size_t i = 0; i < size; i++)
	{
		Light* pLight = m_Selected.at(i);
		MoveLight(pLight, x, y, z);
	}
}

void CLightManager::MoveLight(Light* pLight, float dx, float dy, float dz)
{
	pLight->m_LightData.position[0] += dx;
	pLight->m_LightData.position[1] += dy;
	pLight->m_LightData.position[2] += dz;
}



Light* CLightManager::GetLastSelected()
{
	Light* pResult = NULL;

	if (m_Selected.size() != 0)
	{
		pResult = m_Selected.back();
	}

	return pResult;
}

std::vector<Light*>* CLightManager::GetList()
{
	return &m_LightList;
}



