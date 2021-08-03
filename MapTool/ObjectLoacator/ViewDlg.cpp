#include "pch.h"
#include <filesystem>

#include "resource.h"
#include "Center.h"

#include "ViewDlg.h"
#include "CGrid.h"

#include "MapUtil.h"
#include "WMDefine.h"
#include "utility/HGUtility.h"

#include "CLightManager.h"

//DirectX::XMFLOAT3 g_vCamPos = DirectX::XMFLOAT3(0.f, 100.f, -50.f);
float g_DistanceRatio = 100;
DirectX::XMFLOAT3 g_vCamPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
DirectX::XMFLOAT3 g_vFront = DirectX::XMFLOAT3(0.f, -40.f, 20.f);
DirectX::XMFLOAT3 g_vUp = DirectX::XMFLOAT3(0.f, 1.f, 0.f);

DirectX::XMFLOAT3 g_vX = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
DirectX::XMFLOAT3 g_vY = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
DirectX::XMFLOAT3 g_vZ = DirectX::XMFLOAT3(0.f, 0.f, 1.f);

HWND g_hWndViewer;

BEGIN_MESSAGE_MAP(Viewer, CDialogEx)
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

DirectX::XMFLOAT3 g_StartingCamPos = DirectX::XMFLOAT3(0.f, 100.f, 0.f);

Viewer::Viewer(CWnd* pParent)
{
	//m_hLoacator = pParent->m_hWnd;
}

Viewer::~Viewer()
{
	CleanUp();
}

void Viewer::CleanUp()
{
	m_pInsManager->Release();
	m_pMatManager->DeleteInstance();
	m_pModelManager->DeleteModelManager();
	m_pColManager->DeleteInstance();
	m_pLightManager->Release();
	m_pWaveManager->Release();
	DeleteGrid();						//�׸���

	if (m_pCamController != NULL)
	{
		delete m_pCamController;
		m_pCamController = NULL;
	}

	m_pEngine->EndEngine();
}

void Viewer::Init(int iWidth, int iHeight)
{
	g_hWndViewer = m_hWnd;

	MoveWindow(0, 0, iWidth, iHeight);
	InitGraphicEngin();
	InitManagers();

	m_pCamController = new CamController;

	//CreateGraphicResource();
	//CreateGraphicResource_TEST();
	//CreaetModelByText(NULL);
	CreateModelByFolder();
	CreateMaterialByText(NULL);
	//CreateMaterialByFolder();
	
	SetMainView(g_vCamPos);

	DrawGrid(iDEFAULTGRID_W, iDEFAULTGRID_H, fDEFAULTOFFSET);

	m_Gizmo.Setup(m_pEngine);

	CRect rc;
	GetClientRect(rc);
	m_iScreenWidth = rc.Width();
	m_iScreenHeight = rc.Height();


	//DebugString dStr;
	//dStr.pos.x = 10;
	//dStr.pos.y = 10;
	//dStr.color.x = 1.0f;
	//dStr.color.y = 0.1f;
	//dStr.color.z = 0.1f;

	//dStr.message = "tEST\n\n\n";

	//m_pEngine->AddDebugString(dStr);

	////default : 128, 128, 1.0f, 0.03f, 4.0f, 0.2f 
	//HWaveData* pWave = m_pEngine->CreateWave(128, 128, 10.0f, 4.0f, 0.2f);
	//pWave->worldTM;
}

void Viewer::Update()
{
	g_pCenter->Update();
}

void Viewer::Draw()
{
	g_pCenter->DeleteInDeleteList();

	Update();

	DrawEditMode(1650, 10);
	DrawObjectSelectMode(1650, 30);
	DrawColliderSelectMode(1650, 50);
	DrawLightSelectMode(1650, 70);
	m_pEngine->Loop();
}

void Viewer::DrawEditMode(int x, int y)
{
	float col = 0.1f;
	DebugString dStr;

	dStr.pos.x = x;
	dStr.pos.y = y;

	switch (m_EditMode)
	{
	case EDIT_POS:
		dStr.color.x = 1.f;
		dStr.color.y = col;
		dStr.color.z = col;
		dStr.message = "MOVE";
		break;
	case EDIT_ROT:
		dStr.color.x = col;
		dStr.color.y = 1.f;
		dStr.color.z = col;
		dStr.message = "ROT";
		break;
	case EDIT_SCALE:
		dStr.color.x = col;
		dStr.color.y = col;
		dStr.color.z = 1.f;
		dStr.message = "SCALE";
		break;
	}
	m_pEngine->AddDebugString(dStr);
}

void Viewer::DrawObjectSelectMode(int x, int y)
{
	float col = 0.1f;
	DebugString dStr;
	dStr.color.x = col;
	dStr.color.y = col;
	dStr.color.z = col;
	dStr.pos.x = x;
	dStr.pos.y = y;

	if (m_pInsManager->IsEnableSelectObject())
	{
		dStr.color.x = 0.05f;
		dStr.color.y = 0.05f;
		dStr.color.z = 0.05f;
		dStr.message = "Object Select: Enabled!";
	}
	else
	{
		dStr.color.x = 0.9f;
		dStr.color.y = 0.05f;
		dStr.color.z = 0.05f;
		dStr.message = "Object Select: Disabled!";
	}

	m_pEngine->AddDebugString(dStr);
}

void Viewer::DrawColliderSelectMode(int x, int y)
{
	float col = 0.1f;
	DebugString dStr;
	dStr.color.x = col;
	dStr.color.y = col;
	dStr.color.z = col;
	dStr.pos.x = x;
	dStr.pos.y = y;

	if (m_pColManager->IsEnableColliderSelect())
	{
		dStr.color.x = 0.05f;
		dStr.color.y = 0.05f;
		dStr.color.z = 0.05f;
		dStr.message = "Collider Select: Enabled!";
	}
	else
	{
		dStr.color.x = 0.05f;
		dStr.color.y = 0.96f;
		dStr.color.z = 0.05f;
		dStr.message = "Collider Select: Disabled!";
	}

	m_pEngine->AddDebugString(dStr);
}

void Viewer::DrawLightSelectMode(int x, int y)
{
	float col = 0.f;
	DebugString dStr;
	dStr.color.x = col;
	dStr.color.y = col;
	dStr.color.z = col;
	dStr.pos.x = x;
	dStr.pos.y = y;

	if (m_pLightManager->IsEnableLightSelect())
	{
		dStr.color.x = 0.05f;
		dStr.color.y = 0.05f;
		dStr.color.z = 0.05f;
		dStr.message = "Light Select: Enabled!";
	}
	else
	{
		dStr.color.x = 0.05f;
		dStr.color.y = 0.05f;
		dStr.color.z = 0.95f;
		dStr.message = "Light Select: Disabled!";
	}

	m_pEngine->AddDebugString(dStr);
}

void Viewer::SendModelList(HWND hWnd)
{
	::SendMessage(hWnd, WM_LOAD_MODEL, (WPARAM)(m_pModelManager->GetModelList()), 0);
	//::SendMessage(hWnd, WM_LOAD_MODEL, (WPARAM)(&g_hModelList), 0);
}

void Viewer::SendMatList(HWND hWnd)
{
	::SendMessage(hWnd, WM_LOAD_MAT, (WPARAM)(m_pMatManager->GetMatList()), 0);
	//::SendMessage(hWnd, WM_LOAD_MAT, (WPARAM)(&g_MaterialList), 0);
}

BOOL Viewer::InitGraphicEngin()
{
	HWND hwnd = GetSafeHwnd();
	RECT rc;
	GetClientRect(&rc);

	LONG width = rc.right - rc.left;
	LONG height = rc.bottom - rc.top;
	m_pEngine = HEngine_DX12_3D::GetInstance();
	m_pEngine->InitEngine(hwnd, width, height);
	m_pEngine->StartSetting();
	m_pEngine->LoadSkyBox(L"Media/Skybox/skybox.dds");
	m_pEngine->LoadFont(L"Media/Fonts/SegoeUI_18.spritefont");
	m_pEngine->FinishSetting();
	return TRUE;
}

void Viewer::InitManagers()
{
	//���׸��� �Ŵ���
	m_pInsManager = DrawInsManager::GetInstance();
	m_pMatManager = MaterialManager::GetInstance();
	m_pMatManager->SetGraphicEngine(m_pEngine);

	//�� �޴���
	m_pModelManager = ModelManager::GetModelManager();
	m_pColManager = CColliderManager::GetInstance();
	m_pLightManager = CLightManager::GetInstance();
	m_pWaveManager = CWaveManager::GetInstance();
}

void Viewer::MoveSelectedOffset(CPoint& cur, CPoint& last)
{
	DirectX::XMFLOAT3 move = {};
	CalculateWorldMovementByMouseMove(cur.x, cur.y, last.x, last.y, move);

	double integerX;
	modf((move.x / m_fOffset), &integerX);

	double integerY;
	modf((move.x / m_fOffset), &integerY);

	double integerZ;
	modf((move.x / m_fOffset), &integerZ);

	m_pInsManager->MoveSelected(integerX * m_fOffset, integerY * m_fOffset, integerZ * m_fOffset);
	m_pColManager->MoveSelected(move.x, move.y, move.z);
}


void Viewer::MoveSelected(CPoint& cur, CPoint& last)
{
	DirectX::XMFLOAT3 vWorldMove = {};
	CalculateWorldMovementByMouseMove(cur.x, cur.y, last.x, last.y, vWorldMove);
	MoveSelected(vWorldMove);
}

//���콺 �̵����� �����ϴ� ���� ������ �̵��� ���.
DirectX::XMFLOAT3 Viewer::CalculateWorldMovementByMouseMove(int curX, int curY, int lastX, int lastY, DirectX::XMFLOAT3& vector)
{
	using namespace DirectX::SimpleMath;

	int x = curX;
	int y = curY;
	int lastPosX = lastX;
	int lastPosY = lastY;

	int width = m_iScreenWidth;
	int height = m_iScreenHeight;

	//View, proj
	DirectX::XMMATRIX mView = m_pEngine->GetCamera()->GetView();
	DirectX::XMMATRIX mProj = m_pEngine->GetCamera()->GetProj();

	//����� ���ϱ�
	DirectX::XMMATRIX mUnView = DirectX::XMMatrixInverse(nullptr, mView);
	DirectX::XMMATRIX mUnProj = DirectX::XMMatrixInverse(nullptr, mProj);

	Vector3 temp;
	m_pInsManager->GetCenterPos(&temp);

	temp = Vector3::Transform(temp, mView);
	temp = Vector3::Transform(temp, mProj);


	//��ũ�� ��ǥ�� NDC��ǥ��.
	float ndc_x = ((static_cast<float>(x) * 2.f) / width) - 1;
	float ndc_y = ((static_cast<float>(-y) * 2.f) / height) + 1;

	float ndc_lx = ((static_cast<float>(lastPosX) * 2.f) / width) - 1;
	float ndc_ly = ((static_cast<float>(-lastPosY) * 2.f) / height) + 1;

	//���� ���콺 ������
	DirectX::XMFLOAT3 curPos = {};
	curPos.x = ndc_x;
	curPos.y = ndc_y;;
	curPos.z = temp.z;

	//���� ���콺 ������
	DirectX::XMFLOAT3 lastPos = {};
	lastPos.x = ndc_lx;
	lastPos.y = ndc_ly;
	lastPos.z = temp.z;

	DirectX::XMVECTOR vCur = XMLoadFloat3(&curPos);
	DirectX::XMVECTOR vLast = XMLoadFloat3(&lastPos);

	DirectX::XMVECTOR vCur_unProj = XMVector3TransformCoord(vCur, mUnProj);
	DirectX::XMVECTOR vCur_unProjView = XMVector3TransformCoord(vCur_unProj, mUnView);

	DirectX::XMVECTOR vLast_unProj = XMVector3TransformCoord(vLast, mUnProj);
	DirectX::XMVECTOR vLast_unProjView = XMVector3TransformCoord(vLast_unProj, mUnView);

	//���� ������ ȹ��
	DirectX::XMFLOAT3 wCur = {};
	DirectX::XMStoreFloat3(&wCur, vCur_unProjView);

	DirectX::XMFLOAT3 wLast = {};
	DirectX::XMStoreFloat3(&wLast, vLast_unProjView);

	Vector3 result = Vector3(wCur) - Vector3(wLast);

	if (&vector != nullptr)
		vector = result;

	return result;
}

void Viewer::MoveSelected(DirectX::XMFLOAT3& vector)
{
	DirectX::XMFLOAT3 move = {};

	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_X))
	{
		move.x = vector.x;
		m_fOffset;
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
	{
		move.y = vector.y;
		m_fOffset;
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
	{
		move.z = vector.z;
	}

	m_pInsManager->MoveSelected(move.x, move.y, move.z);
	m_pColManager->MoveSelected(move.x, move.y, move.z);
	m_pLightManager->MoveSelected(move.x, move.y, move.z);
	m_pWaveManager->MoveSelected(move.x, move.y, move.z);
}

void Viewer::ReScaleSelectedXYZ(CPoint& cur, CPoint& last)
{
	int dist = -(cur.y - last.y);
	ReScaleAllAxis(dist);
}

void Viewer::ReScaleSelected(CPoint& cur, CPoint& last)
{
	CPoint curPoint = cur;
	CPoint lastPoint = last;
	//DirectX::XMFLOAT3 vScale = {};
	//CalculatScaleByMouseMove(curPoint.x, curPoint.y, lastPoint.x, lastPoint.y, vScale);
	//ReScaleSelected(vScale);

	int dist = -(cur.y - last.y);
	float ratio = 1.f + static_cast<float>(dist) * 0.01;
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_X))
	{
		m_pInsManager->ReScaleSelectedByRatioX(ratio);
		m_pColManager->RescaleSelectedByRatioX(ratio);		//COLLIDER X
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
	{
		m_pInsManager->ReScaleSelectedByRatioY(ratio);
		m_pColManager->RescaleSelectedByRatioY(ratio);		//COLLIDER Y
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
	{
		m_pInsManager->ReScaleSelectedByRatioZ(ratio);
		m_pColManager->RescaleSelectedByRatioZ(ratio);		//COLLIDER Z
	}
	
}

int Viewer::CalculateScaleByDistance(int curY, int lastY)
{
	int dist = curY - lastY;

	float result = static_cast<float>(dist);// *0.1f;

	return result;
}

DirectX::XMFLOAT3 Viewer::CalculatScaleByMouseMove(int curX, int curY, int lastX, int lastY, DirectX::XMFLOAT3& vector)
{
	float dist = 0.1f;
	float bu = 1.f;	//��ȣ.

	if (curX - lastX < 0)	bu = -1.f;
	else					bu = 1.f;

	if (curY - lastY < 0)
		bu = 1.f;
	else
		bu = -1.f;

	DirectX::XMFLOAT3 result = {};
	result.x = dist * bu;
	result.y = dist * bu;
	result.z = dist * bu;

	if (&vector != NULL)
	{
		vector.x = result.x;
		vector.y = result.y;
		vector.z = result.z;
	}

	return result;
}

DirectX::XMFLOAT3 Viewer::CalculatScaleByMouseMoveVertical(int curY, int lastY, DirectX::XMFLOAT3& vector)
{
	float dist = 0.1f;
	float bu = 1.f;	//��ȣ.
	if (curY - lastY < 0)	bu = -1.f;
	else					bu = 1.f;

	DirectX::XMFLOAT3 result = {};
	result.x = dist * bu;
	result.y = dist * bu;
	result.z = dist * bu;

	if (&vector != NULL)
	{
		vector.x += result.x;
		vector.y += result.y;
		vector.z += result.z;
	}

	return result;
}

void Viewer::ReScaleAllAxis(int dist)
{
	if (dist == 0) return;

	float ratio = 1.f + static_cast<float>(dist) * 0.01;
	g_pCenter->RescaleSelected(ratio);

	//1.���õ� ������Ʈ ��� ��(x,y,z) ���� ������ ���� �޾ƿ���

	//2.�߰��� ���ϰų� ���� ������ ���� =  1 + dist*0.01

	//3.1������ ���� ��� ���� ������ ������ ���� 2���� �����ش�.

	//4.������� �ٽ� �����Ϸ� �������ش�. 


}



void Viewer::ReScaleSelected(DirectX::XMFLOAT3& vScale)
{
	DirectX::XMFLOAT3 scale = {};
	scale.x = 0.f;
	scale.y = 0.f;
	scale.z = 0.f;

	//	if (m_bGizmoCubeTime != TRUE)
	{
		if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_X))
		{
			scale.x = vScale.x;
		}
		if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
		{
			scale.y = vScale.y;
		}
		if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
		{
			scale.z = vScale.z;
		}
	}
	//else
	//{
	//	scale = vScale;
	//}


	m_pInsManager->ReScaleSelected(scale.x, scale.y, scale.z);
	
}

void Viewer::RotationSelected(CPoint& const mouseDownPoint, CPoint& const curPoint)
{
	size_t SizeOfSelectedObj = m_pInsManager->GetSelectedList()->size();
	size_t SizeOfSelectedCollider = m_pColManager->GetSizeOfSelected();;
	size_t SizeOfSelectdWave = m_pWaveManager->GetNumberOfSelected();

	if (SizeOfSelectedObj <= 0 && SizeOfSelectedCollider <= 0 && SizeOfSelectdWave <= 0) return;

	DirectX::XMFLOAT4 rot = CalculateRotationByMouseMovement(mouseDownPoint.x, mouseDownPoint.y, curPoint.x, curPoint.y);
	RotationSelected(rot);
}


DirectX::XMFLOAT4 Viewer::CalculateRotationByMouseMovement(int mouseDownPointX, int mouseDownPointY, int curPointX, int curPointY)
{
	DirectX::XMFLOAT4 result = {};

	CPoint vec = {};
	vec.x = curPointX - mouseDownPointX;
	vec.y = curPointY - mouseDownPointY;
	DirectX::XMVECTOR vAxis = DirectX::XMQuaternionIdentity();

	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_X))
	{
		vAxis = DirectX::XMVectorSet(g_vX.x, g_vX.y, g_vX.z, 0.f);
	}
	else if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
	{
		vAxis = DirectX::XMVectorSet(g_vY.x, g_vY.y, g_vY.z, 0.f);
	}
	else if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
	{
		vAxis = DirectX::XMVectorSet(g_vZ.x, g_vZ.y, g_vZ.z, 0.f);
	}
	else
	{
		DirectX::XMStoreFloat4(&result, vAxis);
		return result;
	}

	float angle = vec.x + vec.y;
	float radian = angle * 3.14 / 180;

	if (angle > 100)
	{
		int x = 3;
	}

	DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(vAxis, radian);
	DirectX::XMStoreFloat4(&result, q);

	return result;
}


void Viewer::RotationSelected(DirectX::XMFLOAT4& rot)
{
	DirectX::XMFLOAT4 pRot = rot;

	//object
	m_pInsManager->RotateSelected(pRot);

	//Collider
	m_pColManager->RotateSelected(pRot);

	//Light
	//m_pLightVector->RotateSelected(pRot);

	//Wave
	m_pWaveManager->RotateSelected(pRot);
}

void Viewer::ToggleOffsetMode()
{
	m_bOffsetMode = !m_bOffsetMode;
}

void Viewer::SetGridInfo(int iWidth, int iHeight, float offset)
{
	m_iGridWidth = iWidth;
	m_iGridHeight = iHeight;
	m_fOffset = offset;
}

//����, ����, offset,  
void Viewer::DrawGrid(int iWidth, int iHeight, float offset)
{
	DirectX::XMFLOAT4 rowColor = DirectX::XMFLOAT4(0.1f, 0.9f, 0.1f, 1.f);
	DirectX::XMFLOAT4 coLColor = DirectX::XMFLOAT4(0.1f, 0.9f, 0.1f, 1.f);
	DirectX::XMFLOAT4 BLACK = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	SetGridInfo(iWidth, iHeight, offset);

	float _offset = offset;
	float _Width = offset * iWidth;
	float _Height = offset * iHeight;

	float _halfWidth = _Width / 2;
	float _halfHeight = _Height / 2;

	int NumOfLine_width = iWidth;
	int NumOfLine_height = iHeight;

	HProceduralGeometry_line* hDLine;

	//row
	for (int i = 0; i < NumOfLine_width + 1; i++)
	{
		hDLine = m_pEngine->CreateLine(eNoOption_Primitive);
		if (i == 0)
		{
			hDLine->color = BLACK;
		}
		else if (i == NumOfLine_width)
		{
			hDLine->color = BLACK;
		}
		else
		{
			hDLine->color = coLColor;
		}
		hDLine->dots[0] = XMFLOAT3(_offset * i - _halfWidth, 0.f, -_halfHeight);
		hDLine->dots[1] = XMFLOAT3(_offset * i - _halfWidth, 0.f, +_halfHeight);


		m_hDebugLineList.push_back(hDLine);
	}

	//col
	for (int i = 0; i < NumOfLine_height + 1; i++)
	{
		hDLine = m_pEngine->CreateLine(eNoOption_Primitive);
		if (i == 0)
		{
			hDLine->color = BLACK;
		}
		else if (i == NumOfLine_height)
		{
			hDLine->color = BLACK;
		}
		else
		{
			hDLine->color = rowColor;
		}
		hDLine->dots[0] = XMFLOAT3(+_halfWidth, 0.f, _offset * i - _halfHeight);
		hDLine->dots[1] = XMFLOAT3(-_halfWidth, 0.f, _offset * i - _halfHeight);

		m_hDebugLineList.push_back(hDLine);
	}
}


void Viewer::DeleteGrid()
{
	size_t size = m_hDebugLineList.size();

	std::vector< HProceduralGeometry_line*>::iterator it;
	for (it = m_hDebugLineList.begin(); it != m_hDebugLineList.end();)
	{
		HProceduralGeometry_line* pTemp = (*it);
		pTemp->Delete();

		it = m_hDebugLineList.erase(it);
	}

}


void Viewer::ChangeGrid(int iWidth, int iHeight, float offset)
{
	DeleteGrid();
	DrawGrid(iWidth, iHeight, offset);
}

void Viewer::MoveCam(eDirection dir)
{
	if (dir > eDirection_MAX)
	{
		::MessageBoxW(NULL, L"DirectionError", L"Direction Out of Range", MB_OK);
		assert(FALSE);
		return;
	}

	switch (dir)
	{
	case eDirection_Front:
		m_pCamController->MoveFront(+1.0, m_fCamVelo);
		break;
	case eDirection_Back:
		m_pCamController->MoveFront(-1.0, m_fCamVelo);
		break;
	case eDirection_Right:
		m_pCamController->MoveRight(+1.0, m_fCamVelo);
		break;
	case eDirection_Left:
		m_pCamController->MoveRight(-1.0, m_fCamVelo);
		break;
	case eDirection_Up:
		m_pCamController->MoveUp(+1.0, m_fCamVelo);
		break;
	case eDirection_Down:
		m_pCamController->MoveUp(-1.0, m_fCamVelo);
		break;
	}

}

void Viewer::ControlCamVelo(float delta)
{
	m_fCamVelo += delta;
}


void Viewer::SetMainView(DirectX::XMFLOAT3 pos)
{
	RECT size;
	GetClientRect(&size);
	XMFLOAT3 posA = { pos.x, pos.y - 50, pos.z };

	float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_PI / 180.0f;


	XMFLOAT3 newCamPos = { posA.x + 4 * g_DistanceRatio, posA.y + 5 * g_DistanceRatio, posA.z - 4 * g_DistanceRatio };

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	Camera* pCam = m_pEngine->GetCamera();
	//pCam->SetLens(fovAngleY, size.right, size.bottom, 1.0f, 1200.f);
	pCam->SetLens(fovAngleY, size.right, size.bottom, 1.0f, FLT_MAX);
	BoundingFrustum::CreateFromMatrix(m_camFrustum, pCam->GetProj());

	pCam->LookAt(newCamPos, posA, g_vUp);
	pCam->SetPosition(newCamPos);
	//ePerspective, eOrthgraphic
	pCam->SetCameraMode(CameraMode::ePerspective);

}

void Viewer::RollCam(DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp)
{
	DirectX::XMFLOAT3 pos;
	m_Gizmo.GetPos(pos);
	SetCam(pos, vFront, vUp);
}



BOOL Viewer::CreaetModelByText(wchar_t* szFileName)
{
	m_pEngine->StartSetting();

	std::vector<wchar_t*> fileList;

	CTextReader* pTxtReader = new CTextReader;
	wchar_t* pFile = L"Media/Model/modelList.txt";

	int iResult = HGUtility::IsFileExists(pFile);
	if (iResult == 1)
	{
		pTxtReader->OpenTxt(pFile);
	}

	pTxtReader->ReadTxt(NULL);
	pTxtReader->GetFileList(fileList);
	pTxtReader->CloseTxt();
	delete pTxtReader;

	CString pathOfModel = L"Media/Model/";
	//CString pathOfMaterial	= L"Media/Material/";

	static int modelIndex = 0;
	std::vector<wchar_t*>::iterator it;
	for (it = fileList.begin(); it != fileList.end();)
	{
		wchar_t* pName = *it;
		CString temp = CString(pName);
		m_pModelManager->LoadModelFromHModel(pathOfModel, temp, modelIndex);
		temp.Empty();

		delete[] pName;
		pName = NULL;
		it = fileList.erase(it);
	}

	m_pEngine->FinishSetting();

	return 0;
}

BOOL Viewer::CreateMaterialByText(wchar_t* szFileName)
{
	std::vector<wchar_t*> fileList;

	CTextReader* pTxtReader = new CTextReader;
	wchar_t* pFile = L"Media/Material/materialList.txt";

	int iResult = HGUtility::IsFileExists(pFile);
	if (iResult == 1)
	{
		pTxtReader->OpenTxt(pFile);
	}

	pTxtReader->ReadTxt(NULL);
	pTxtReader->GetFileList(fileList);
	pTxtReader->CloseTxt();
	delete pTxtReader;

	m_pEngine->StartSetting();
	int texIndex = -1;
	CString png = L".PNG";
	CString pathOfMaterial = L"Media/Material/";

	//CString colorchip = L"ColorChip";
	//m_pMatManager->LoadTexture(pathOfMaterial, colorchip, png);

	std::vector<wchar_t*>::iterator it;
	for (it = fileList.begin(); it != fileList.end();)
	{
		CString temp = CString(*it);
		texIndex = m_pMatManager->LoadTexture(pathOfMaterial, temp, png);

		delete[] * it;
		*it = NULL;
		it = fileList.erase(it);
	}

	//m_pEngine->LoadSkyBox(L"Media/Skybox/Skybox.dds");			//��ī�̹ڽ�
	//m_pEngine->LoadFont(L"Media/Fonts/SegoeUI_18.spritefont");	//��Ʈ

	m_pEngine->FinishSetting();

	return 0;
}

BOOL Viewer::CreateModelByFolder()
{
	m_pEngine->StartSetting();
	int index = -1;
	for (const auto& entry : std::filesystem::recursive_directory_iterator("Media/Model"))
	{
		const auto& filePath = entry.path().string();

		if (filePath.substr(filePath.length() - 7, 7) == ".hmodel")
		{
			std::size_t slash = filePath.rfind("\\");
			std::size_t formatName = filePath.rfind(".hmodel");
			std::string modelName = filePath.substr(slash + 1, formatName - slash - 1);
			index++;
			m_pModelManager->LoadModel(modelName, filePath);
		}
	}
	m_pEngine->FinishSetting();
	return 0;
}

BOOL Viewer::CreateMaterialByFolder()
{
	m_pEngine->StartSetting();
	int index = -1;
	for (const auto& entry : std::filesystem::recursive_directory_iterator("Media/Material"))
	{
		const auto& filePath = entry.path().string();

		size_t slash = entry.path().string().rfind("\\");
		size_t length = entry.path().string().size();
		std::string materialName = filePath.substr(slash + 1, length);

		TextureSet set;

		for (const auto& matFolder : std::filesystem::recursive_directory_iterator(filePath.c_str()) )
		{
			std::string material = matFolder.path().string();

			if (material.substr(material.length() - 4, 4) == ".png")
			{
				std::size_t slashPoint	= material.rfind("\\");
				std::size_t underBar	= material.rfind("_");
				std::size_t formatName	= material.rfind(".png");
				std::string texKind		= material.substr(underBar, formatName - underBar - 1);

				std::string matName = filePath.substr(slash + 1, formatName - slash - 1);
			}
			
		}
		//if (filePath.substr(filePath.length() - 3, 3) == ".png")
		//{
		//	std::size_t slash = filePath.rfind("\\");
		//	std::size_t formatName = filePath.rfind(".hmodel");
		//	std::string modelName = filePath.substr(slash + 1, formatName - slash - 1);
		//	index++;
		//	m_pModelManager->LoadModel(modelName, filePath);
		//}
	}
	m_pEngine->FinishSetting();
	return 0;
}

//BOOL Viewer::CreateMaterialByFolder()
//{
//	m_pEngine->StartSetting();
//	int index = -1;
//	for (const auto& entry : std::filesystem::recursive_directory_iterator("Media/Material"))
//	{
//		const auto& filePath = entry.path().string();
//
//		if (filePath.substr(filePath.length() - 7, 7) == ".hmodel")
//		{
//			std::size_t slash = filePath.rfind("\\");
//			std::size_t formatName = filePath.rfind(".hmodel");
//			std::string matName = filePath.substr(slash + 1, formatName - slash - 1);
//			index++;
//			m_pMatManager->LoadTexture(matName, filePath);
//			//m_pModelManager->LoadModel(matName, filePath);
//		}
//	}
//	m_pEngine->FinishSetting();
//	return 0;
//}






//*--------------------------------------------------*//
//					target : ���Դ��				  //
//					front  : ���� ����				  //
//					vUp��  :							  //
//*--------------------------------------------------*//
void Viewer::SetCam(DirectX::XMFLOAT3& vTarget, DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp)
{
	//vFront�� ��ֶ����� �Ǿ ���;��Ѵ�.
	//Ÿ������ ���� ������ �Ÿ�
	float fAway = 50.f;
	DirectX::XMFLOAT3 newCamPos = {};
	newCamPos.x = vTarget.x - vFront.x * fAway;
	newCamPos.y = vTarget.y - vFront.y * fAway;
	newCamPos.z = vTarget.z - vFront.z * fAway;

	m_pEngine->GetCamera()->LookAt(newCamPos, vTarget, vUp);

}


//���, �̸�, Ȯ����
wchar_t* Viewer::MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend)
{
	dest = path + name + L"/" + name + tex + extend;
	return dest.GetBuffer();
}


HInstanceData* Viewer::AddGraphicInstance(object* pObj)
{
	HInstanceData* hIns = NULL;
	HMaterialData* pMat = NULL;

	DirectX::XMFLOAT4X4 mTm = MapUtil::Identity4x4();
	pObj->GetTm(mTm);

	int modelIndex = pObj->modelIndex;
	if (modelIndex < 0)
		modelIndex = 0;

	int matIndex = pObj->matIndex;
	if (matIndex < 0)
		matIndex = 0;


	//���׸��� �ε����� 0�� ��� �÷�Ĩ���� �ν��Ѵ�.
	if (matIndex == 0)
	{
		hIns = m_pModelManager->GetModelList()->at(0)->hModel->AddInstance(ShaderType::COLORCHIP);
	}
	else
	{
		hIns = m_pModelManager->GetModelList()->at(0)->hModel->AddInstance(ShaderType::DEFAULT);
		pMat = m_pMatManager->GetMatList()->at(0)->hMat;
		hIns->SetMaterial(pMat, 0);
	}

	hIns->worldTM = mTm;

	//���� üũ �ʿ��ϴ�. ���� ��Ȯ�� �ʿ�.
	if (hIns == NULL)
		return NULL;
	else
		return hIns;

}



void Viewer::RequestCreateObj(object* pObj)
{
	g_pCenter->CreateObj(pObj);
	//::SendMessageW(g_hCenter, WM_CREATE_OBJ, (WPARAM)(m_hWnd), (LPARAM)(pObj));
}

//������ ����/draw
void Viewer::ControlGizumo()
{
	DirectX::XMFLOAT3  ObjPos = {};
	m_pInsManager->GetCenterPos(&ObjPos);
	size_t ObjSize = m_pInsManager->GetSizeOfSelected();

	DirectX::XMFLOAT3 ColliderPos = m_pColManager->GetSelectedPos();
	size_t colSize = m_pColManager->GetSizeOfSelected();

	DirectX::XMFLOAT3 LightPos = m_pLightManager->GetSelectedCenterPos();
	size_t LightSize = m_pLightManager->GetNumberOfSelected();

	DirectX::XMFLOAT3 WavePos = m_pWaveManager->GetSelectedCenterPos();
	size_t WaveSize = m_pWaveManager->GetNumberOfSelected();


	DirectX::XMFLOAT3 result = {};

	if (ObjSize == 0 && colSize == 0 && LightSize == 0 && WaveSize == 0)
	{
		goto lb_return;
	}

	ObjPos.x = ObjPos.x * ObjSize;
	ObjPos.y = ObjPos.y * ObjSize;
	ObjPos.z = ObjPos.z * ObjSize;

	ColliderPos.x = ColliderPos.x * colSize;
	ColliderPos.y = ColliderPos.y * colSize;
	ColliderPos.z = ColliderPos.z * colSize;

	LightPos.x = LightPos.x * LightSize;
	LightPos.y = LightPos.y * LightSize;
	LightPos.z = LightPos.z * LightSize;

	WavePos.x = WavePos.x * WaveSize;
	WavePos.y = WavePos.y * WaveSize;
	WavePos.z = WavePos.z * WaveSize;

	result.x = (ObjPos.x + ColliderPos.x + LightPos.x + WavePos.x) / (ObjSize + colSize + LightSize + WaveSize);
	result.y = (ObjPos.y + ColliderPos.y + LightPos.y + WavePos.y) / (ObjSize + colSize + LightSize + WaveSize);
	result.z = (ObjPos.z + ColliderPos.z + LightPos.z + WavePos.z) / (ObjSize + colSize + LightSize + WaveSize);

lb_return:
	m_Gizmo.SetPos(result);
}

//drag
void Viewer::GetObjInRect()
{
	bool bResult = false;
	DrawInsList* pList = (DrawInsList*)m_pInsManager->GetDrawInsList();

	if (pList == NULL) return;
	if (pList->size() <= 0) return;

	float ndc_x = GetNDC_X(m_DragPoint[0]);
	float ndc_y = GetNDC_Y(m_DragPoint[0]);
	float ndc_lx = GetNDC_X(m_DragPoint[1]);
	float ndc_ly = GetNDC_Y(m_DragPoint[1]);

	float xMax = max(ndc_x, ndc_lx);
	float xMin = min(ndc_x, ndc_lx);
	float yMax = max(ndc_y, ndc_ly);
	float yMin = min(ndc_y, ndc_ly);

	DirectX::XMMATRIX mView = m_pEngine->GetCamera()->GetView();
	DirectX::XMMATRIX mProj = m_pEngine->GetCamera()->GetProj();

	//object�� ���� �˻�.
	DrawInsList::iterator it;
	for (it = pList->begin(); it != pList->end(); it++)
	{
		DRAW_INSTANCE* pDrawIns = *it;
		DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 TM;
		pDrawIns->first->GetTm(TM);

		DirectX::XMMATRIX mTM = DirectX::XMLoadFloat4x4(&TM);

		matrix = mTM * mView * mProj;

		int modelIndex = pDrawIns->first->modelIndex;
		MODEL* pMODEL = m_pModelManager->GetModel(modelIndex);
		if (pMODEL == nullptr)
		{
			int a = 0;
			return;
		}
		DirectX::XMFLOAT3 CenterPos = pMODEL->hModel->pRawData->boundingBox.Center;
		DirectX::XMVECTOR vCenterPos = DirectX::XMLoadFloat3(&CenterPos);

		DirectX::XMFLOAT3 screenPos;
		DirectX::XMVECTOR pos = XMVector3TransformCoord(vCenterPos, matrix);
		DirectX::XMStoreFloat3(&screenPos, pos);

		bool isInRect = MapUtil::AABB(xMin, xMax, yMin, yMax, screenPos.x, screenPos.y);
		if (isInRect == true)
		{
			m_pInsManager->AddSelected(pDrawIns->second);
		}
		else
		{
			object* pObj = pDrawIns->first;
			if (m_pInsManager->IsSelected(pObj) == true)
			{
				m_pInsManager->AddSelected(pObj);
			}
		}
	}
}

float Viewer::GetNDC_X(CPoint& inPoint)
{
	float ndc_x = ((static_cast<float>(inPoint.x) * 2.f) / m_iScreenWidth) - 1;

	return ndc_x;
}

float Viewer::GetNDC_Y(CPoint& inPoint)
{
	float ndc_y = ((-static_cast<float>(inPoint.y) * 2.f) / m_iScreenHeight) + 1;

	return ndc_y;
}

void Viewer::GetKeyUp(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_CONTROL:
	{
		m_isCtrlDown = true;
		break;
	}

	//����� ��Ʈ�� ��� ����
	case 'W':
		m_EditMode = EDIT_POS;
		break;
	case 'E':
		m_EditMode = EDIT_ROT;
		break;
	case 'R':
		m_EditMode = EDIT_SCALE;
		break;


		//�� ����
	case 'T':
	{
		RollCam(-1 * g_vY, g_vZ);
		break;
	}
	case 'F':
	{
		RollCam(g_vZ, g_vY);
		break;
	}

	case 'Y':		//
	{
		RollCam(g_vX, g_vY);
		break;
	}

	case '1':
		SetMainView(g_vCamPos);
		break;
	}

}



void Viewer::UpdateObjectEditBox()
{
	object* pResult = m_pInsManager->GetLastSelected();
	g_pCenter->SelectObj(pResult);

	g_pCenter->UpdateSelectedLight();	//����Ʈ ������Ʈ
}

void Viewer::UpdateColliderEditBox()
{
	collider* pCol = m_pColManager->GetLastSelected();
	g_pCenter->UpdateColliderEditControl(pCol);
}

void Viewer::UpdateWaveEditBox()
{
	WAVE* pWave = m_pWaveManager->GetLastSelected();
	g_pCenter->UpdateWaveEditControl(pWave);
}

//������ �׷��� �ν��Ͻ����� ��� ó�� �� ���ΰ�?
void Viewer::SelectGraphicInstance(unsigned int x, unsigned int y)
{
	HInstanceData* hGraphicIns = m_pEngine->Picking(x, y);
	if (hGraphicIns == NULL)
	{
		int x = 3;
	}
	m_pInsManager->AddSelected_public(hGraphicIns);

	UpdateObjectEditBox();

	m_pInsManager->SetSelectedPrvRot();
}

void Viewer::SelectColliderInstance(CPoint& clickPoint)
{
	collider* pCol = g_pCenter->PickingCollider(clickPoint.x, clickPoint.y);

	m_pColManager->SetSelectedPrvRot();
}

void Viewer::SelectLightInstance(CPoint& clickPoint)
{
	Light* pLight = g_pCenter->PickingLight(clickPoint.x, clickPoint.y);

}

void Viewer::SelectWaveInstance(CPoint& clickPoint)
{
	g_pCenter->DeleteSelectedListAll();
}




//WM_KEYDOWN
void Viewer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == VK_CONTROL)
	{
		m_isCtrlDown = true;
		OutputDebugString(L"[KEY_DOWN] Contorl\n");
	}

	switch (nChar)
	{
	case 'W':
		m_EditMode = EDIT_POS;
		break;
	case 'E':
		m_EditMode = EDIT_ROT;
		break;
	case 'R':
		m_EditMode = EDIT_SCALE;
		break;
	}

lb_return:
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////
//https://blog.daum.net/jyhpsycho/10	//
//////////////////////////////////////////
void Viewer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
	{
		m_isCtrlDown = false;
		OutputDebugString(L"[KEY_UP] Contorl\n");
	}
	GetKeyUp(nChar);

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

void Viewer::OnRButtonDown(UINT nFlags, CPoint point)
{
	//m_bIsRButtonClick = TRUE;
	m_pCamController->MovementAvailable(true);
	m_RBtnDownPos = point;
	//OutputDebugString(L"Mouse Down\n");
	//CDialogEx::OnRButtonDown(nFlags, point);
}

void Viewer::OnRButtonUp(UINT nFlags, CPoint point)
{
	//m_bIsRButtonClick = FALSE;
	m_pCamController->MovementAvailable(false);
	m_RBtnDownPos.x = 0;
	m_RBtnDownPos.y = 0;
	//OutputDebugString(L"Mouse Up\n");
	//CDialogEx::OnRButtonUp(nFlags, point);
}

void Viewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLbuttonDown = true;

	unsigned int x = point.x;
	unsigned int y = point.y;

	m_DragPoint[0].x = x;
	m_DragPoint[0].y = y;

	m_bGizmoTime = m_Gizmo.CheckSelectedIndex(x, y, m_iScreenWidth, m_iScreenHeight);		//���ý� 
	m_bGizmoCubeTime = m_Gizmo.IsPickCube(x, y);

	if ((m_bGizmoTime == FALSE) && (m_bGizmoCubeTime == FALSE))
	{
		SelectGraphicInstance(x, y);
		SelectColliderInstance(point);
		SelectLightInstance(point);
		SelectWaveInstance(point);
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}

void Viewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	//��
	m_Gizmo.ReleaseAxis();
	m_bLbuttonDown = false;

	//������ ť��
	m_Gizmo.ReleaseCube();
	m_bGizmoCubeTime = FALSE;

	m_pInsManager->SetSelectedPrvRot();
	m_pColManager->SetSelectedPrvRot();
	m_pWaveManager->SetSelectedPrvRot();

	UpdateObjectEditBox();
	UpdateColliderEditBox();
	UpdateWaveEditBox();

	CDialogEx::OnLButtonUp(nFlags, point);
}

void Viewer::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint last = { 0, };
	CPoint cur = point;

	m_DragPoint[1].x = point.x;
	m_DragPoint[1].y = point.y;

	if (m_bGizmoCubeTime == TRUE)
	{
		ReScaleSelectedXYZ(cur, last);
	}
	else if (m_bGizmoTime)
	{
		switch (m_EditMode)
		{
		case EDIT_POS:
			MoveSelected(cur, last);
			break;
		case EDIT_ROT:
			RotationSelected(m_DragPoint[0], m_DragPoint[1]);
			break;
		case EDIT_SCALE:
			ReScaleSelected(cur, last);
			//ReScaleSelectedXYX(cur, last);
			break;
		}

	}
	else
	{
		m_pCamController->Update(cur.x, cur.y, last.x, last.y);

		if (m_bLbuttonDown)
		{
			float ndcX1 = GetNDC_X(m_DragPoint[0]);			//�Ϲ�ȭ �� �Լ��� ���� �ʿ�
			float ndcY1 = GetNDC_Y(m_DragPoint[0]);
			float ndcX2 = GetNDC_X(m_DragPoint[1]);
			float ndcY2 = GetNDC_Y(m_DragPoint[1]);

			GetObjInRect();
			m_pColManager->GetColliderInRect(ndcX1, ndcY1, ndcX2, ndcY2);
			m_pLightManager->GetLightInRect(ndcX1, ndcY1, ndcX2, ndcY2);
			g_pCenter->UpdateSelectedLight();	//Update Dialog
			
			//Light* pLight = m_pLightManager->GetLastSelected();
			//object* pLast = m_pInsManager->GetLastSelectedImg();
		}
	}

	last.x = cur.x;
	last.y = cur.y;

	//ī�޶� ������Ʈ
	m_pEngine->GetCamera()->UpdateViewMatrix();

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL Viewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//���콺 ���� �̿��� ���̰� ����

	/*DirectX::XMFLOAT3 curPos = m_pEngine->GetCamera()->GetPosition3f();
	curPos.y -= zDelta * 0.05;
	m_pEngine->GetCamera()->SetPosition(curPos);*/

	m_pCamController->MoveFront(zDelta, 1.0f);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void Viewer::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_pCamController->RotationAvailable(TRUE);

	CDialogEx::OnMButtonDown(nFlags, point);
}


void Viewer::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_pCamController->RotationAvailable(FALSE);
	//m_bIsWheel = FALSE;

	CDialogEx::OnMButtonUp(nFlags, point);
}

void Viewer::OnPaint()
{
	static bool isDraw = false;
	if (!isDraw)
	{
		isDraw = true;
		ControlGizumo();
		
		Draw();
		isDraw = false;
	}
	else
		return;
}


void Viewer::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		ShowWindow(SW_HIDE);
		::SendMessageW(g_hCenter, WM_HIDE_WINDOW, WPARAM(m_hWnd), eDlgs::eViewer);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}

}


BOOL Viewer::PreTranslateMessage(MSG* pMsg)
{

	UINT message = pMsg->message;
	WPARAM wPram = pMsg->wParam;
	switch (message)
	{
	case WM_KEYDOWN:
		break;

	case WM_KEYUP:
	{
		switch (wPram)
		{
		case VK_CONTROL:
		{
			m_isCtrlDown = false;
			OutputDebugStringW(L"[CONTROL KEY UP]\n");
			break;
		}
		case VK_F3:
		{
			g_pCenter->SaveMap();
			//::PostMessageW(g_hCenter, WM_SAVE_MAP, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
		case VK_F4:
		{
			g_pCenter->LoadMap();
			//::PostMessageW(g_hCenter, WM_LOAD_MAP, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}

		}
		GetKeyUp(wPram);
	}

	}

	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL Viewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}




void Viewer::OnClose()
{
	int a = 0;
	CDialogEx::OnClose();
}