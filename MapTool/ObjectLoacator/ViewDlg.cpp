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

void Viewer::InitailzeGraphics(int iWidth, int iHeight)
{
	g_hWndViewer = m_hWnd;

	MoveWindow(0, 0, iWidth, iHeight);
	InitGraphicEngin();


	m_pCamController = new CamController;
	
	SetMainView(g_vCamPos);

	//DrawGrid(iDEFAULTGRID_W, iDEFAULTGRID_H, fDEFAULTOFFSET);	//그리드 관리자 분리 가능.

	m_Gizmo.Setup(m_pEngine);

	CRect rc;
	GetClientRect(rc);
	m_iScreenWidth = rc.Width();
	m_iScreenHeight = rc.Height();
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

Viewer::Viewer(CWnd* pParent)
{
}

Viewer::~Viewer()
{
	CleanUp();
}

void Viewer::Initialize(CWnd* pParent, UINT id, int iWidth, int iHeight)
{
	CEditorDlg::Initialize(pParent, id);
	
	int iFrameX = GetSystemMetrics(SM_CXFRAME);
	int iFrameY = GetSystemMetrics(SM_CYFRAME);
	int iCaptionY = GetSystemMetrics(SM_CYCAPTION);

	//MoveWindow(0, 0, iWidth, iHeight);

	int width = iWidth + (iFrameX << 2);
	int height = iHeight + (iFrameY << 2) + iCaptionY;

	MoveWindow(0, 0, width + 2, height + 2);

	CRect rc;
	GetClientRect(&rc);
	//ClientToScreen(rc);
	int cWdith = rc.Width();;
	int cHeight = rc.Height();

	m_iScreenWidth  = cWdith;
	m_iScreenHeight = cHeight;

	//InitailzeGraphics(width, height);
}

void Viewer::CleanUp()
{
	//m_pMatManager->DeleteInstance();
	//m_pModelManager->DeleteModelManager();
	DeleteGrid();						//그리드

	if (m_pCamController != NULL)
	{
		delete m_pCamController;
		m_pCamController = NULL;
	}

	m_pEngine->EndEngine();
}


void Viewer::Draw()
{
	//약간의 성능 저하 (노트북 기준 6개 오브젝트 75 -> 72~71)
	//Draw() 에서 다시 메세지를 전달한 꼴...
	::SendMessageW(g_hCenter, WM_REQUEST_CENTER_DRAW, NULL, NULL);

	//g_pCenter->DeleteInDeleteList();		//Center
	//Update();								//Center
	//m_pEngine->Loop();						//Center
	//
											//이동 가능.
	int _iDrawPos = m_iScreenWidth - 250;
	DrawEditMode(_iDrawPos, 10);
	DrawObjectSelectMode(_iDrawPos, 30);
	DrawColliderSelectMode(_iDrawPos, 50);
	DrawLightSelectMode(_iDrawPos, 70);

	
}

//그래픽 인스턴스를 만드는 책임만 있다. -> 그래픽 엔진에 의존적인 작업.
//관리는 매니저 클래스에서.
//virtual로 할 것.
//HInstanceData* Viewer::CreateGraphicInstance(object* pSrc)
//{
//	HInstanceData* hResult = NULL;
//	object* pObj = pSrc;
//
//	DirectX::XMFLOAT4X4 mTm = MapUtil::Identity4x4();
//	pObj->GetTm(mTm);
//
//	int modelIndex = pObj->modelIndex;
//	if (modelIndex < 0)
//	{
//		OutputDebugStringW(L"[CreateGraphicInstance] Wrong MODEL Index");
//		modelIndex = 0;
//	}
//
//	int matIndex = pObj->matIndex;
//	if (matIndex < 0)
//	{
//		OutputDebugStringW(L"[CreateGraphicInstance] Wrong MODEL Index");
//		matIndex = 0;
//	}
//
//	HModelData* pModel = m_pModelManager->GetModel(modelIndex)->hModel;
//
//	HMaterialData* pMat = NULL;
//	if (matIndex == 0)
//	{
//		hResult = pModel->AddInstance(ShaderType::COLORCHIP);
//		hResult->worldTM = mTm;
//	}
//	else
//	{
//		hResult = pModel->AddInstance(ShaderType::DEFAULT);
//		pMat = m_pMatManager->GetMatList()->at(matIndex)->hMat;
//		hResult->SetMaterial(pMat, 0);
//		hResult->worldTM = mTm;
//	}
//
//	return hResult;
//}

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

	//if (m_pInsManager->IsEnableSelectObject())
	if(m_bObjectSelectMode)
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

	if(m_bColliderSelectMode)
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

	if(m_bLightSelectMode)
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

int Viewer::GetWidth()
{
	return m_iScreenWidth;
}

int Viewer::GetHeight()
{
	return m_iScreenHeight;
}

void Viewer::SetGraphicEngine(HEngine_DX12_3D* pEngine)
{
	if (pEngine == nullptr)
	{
		OutputDebugStringW(L"[Viewer::SetGraphicEngine()] GraphicEnigne Is NULL\n");
		assert(false);
		return;
	}

	m_pEngine = pEngine;
	g_hWndViewer = m_hWnd;
	m_pCamController = new CamController;
	SetMainView(g_vCamPos);
	m_Gizmo.Setup(m_pEngine);

	DrawGrid(iDEFAULTGRID_W, iDEFAULTGRID_H, fDEFAULTOFFSET);	//그리드 관리자 분리 가능.
}

void Viewer::SetObjectSelectMode(bool bSelect)
{
	m_bObjectSelectMode = bSelect;
}

void Viewer::SetColliderSelectMode(bool bSelect)
{
	m_bColliderSelectMode = bSelect;
}

void Viewer::SetLightSelectMode(bool bSelect)
{
	m_bLightSelectMode = bSelect;
}


void Viewer::MoveSelectedOffset(CPoint& cur, CPoint& last)
{
	DirectX::XMFLOAT3 move = {};
	CalculateWorldMovementByMouseMove(cur.x, cur.y, last.x, last.y, move);

	::SendMessageW(g_hCenter, WM_OBJECT_MOVE, (WPARAM)&move, NULL);
}


void Viewer::MoveSelected(CPoint& cur, CPoint& last)
{
	DirectX::XMFLOAT3 vWorldMove = {};
	CalculateWorldMovementByMouseMove(cur.x, cur.y, last.x, last.y, vWorldMove);
	MoveSelected(vWorldMove);
}

//마우스 이동량을 대응하는 월드 포지션 이동량 계산.
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

	//역행렬 구하기
	DirectX::XMMATRIX mUnView = DirectX::XMMatrixInverse(nullptr, mView);
	DirectX::XMMATRIX mUnProj = DirectX::XMMatrixInverse(nullptr, mProj);

	XMFLOAT3 temp;
	//m_pInsManager->GetCenterPos(&temp);
	m_Gizmo.GetPos(temp);

	temp = Vector3::Transform(temp, mView);
	temp = Vector3::Transform(temp, mProj);


	//스크린 좌표를 NDC좌표로.
	float ndc_x = ((static_cast<float>(x) * 2.f) / width) - 1;
	float ndc_y = ((static_cast<float>(-y) * 2.f) / height) + 1;

	float ndc_lx = ((static_cast<float>(lastPosX) * 2.f) / width) - 1;
	float ndc_ly = ((static_cast<float>(-lastPosY) * 2.f) / height) + 1;

	//현재 마우스 포지션
	DirectX::XMFLOAT3 curPos = {};
	curPos.x = ndc_x;
	curPos.y = ndc_y;;
	curPos.z = temp.z;

	//과거 마우스 포지션
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

	//월드 포지션 획득
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
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
	{
		move.y = vector.y;
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
	{
		move.z = vector.z;
	}
	::SendMessageW(g_hCenter, WM_OBJECT_MOVE ,(WPARAM)&move, NULL);
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

	int dist = -(cur.y - last.y);
	float ratio = 1.f + static_cast<float>(dist) * 0.01;
	DirectX::XMFLOAT4 scale = {};

	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_X))
	{	
		scale.x = ratio;
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Y))
	{
		scale.y = ratio;
	}
	if (m_Gizmo.CheckAxis(Gizmo::AXIS::AXIS_Z))
	{
		scale.z = ratio;
	}
	
	::SendMessageW(g_hCenter, WM_OBJECT_SCALE ,(WPARAM)&scale, NULL);
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
	float bu = 1.f;	//부호.

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
	float bu = 1.f;	//부호.
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

	//1.선택된 오브젝트 모든 축(x,y,z) 현재 스케일 정보 받아오기

	//2.추가로 더하거나 빼줄 스케일 비율 =  1 + dist*0.01

	//3.1번에서 구한 모든 축의 스케일 정보에 각각 2번을 곱해준다.

	//4.결과값을 다시 스케일로 세팅해준다. 


}

void Viewer::RotationSelected(CPoint& const mouseDownPoint, CPoint& const curPoint)
{
	//드래그한 거리를 바탕으로 회전 거리 계산. -> 일단 계산.
	DirectX::XMFLOAT4 rot = CalculateRotationByMouseMovement(mouseDownPoint.x, mouseDownPoint.y, curPoint.x, curPoint.y);

	//메세지 변경 고려.
	::SendMessageW(g_hCenter, WM_OBJECT_ROTATION, (WPARAM)&rot, NULL);

}

//마우스 클릭 후 드래그한 거리를 바탕으로 회전량을 계산한다.
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

void Viewer::SetGridInfo(int iWidth, int iHeight, float offset)
{
	m_iGridWidth = iWidth;
	m_iGridHeight = iHeight;
	m_fOffset = offset;
}

//가로, 세로, offset,  
void Viewer::DrawGrid(int iWidth, int iHeight, float offset)
{
	if (m_pEngine == nullptr) return;
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
	if (m_pEngine == nullptr) return;
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


//
//BOOL Viewer::CreaetModelByText(wchar_t* szFileName)
//{
//	m_pEngine->StartSetting();
//
//	std::vector<wchar_t*> fileList;
//
//	CTextReader* pTxtReader = new CTextReader;
//	wchar_t* pFile = L"Media/Model/modelList.txt";
//
//	int iResult = HGUtility::IsFileExists(pFile);
//	if (iResult == 1)
//	{
//		pTxtReader->OpenTxt(pFile);
//	}
//
//	pTxtReader->ReadTxt(NULL);
//	pTxtReader->GetFileList(fileList);
//	pTxtReader->CloseTxt();
//	delete pTxtReader;
//
//	CString pathOfModel = L"Media/Model/";
//	//CString pathOfMaterial	= L"Media/Material/";
//
//	static int modelIndex = 0;
//	std::vector<wchar_t*>::iterator it;
//	for (it = fileList.begin(); it != fileList.end();)
//	{
//		wchar_t* pName = *it;
//		CString temp = CString(pName);
//		m_pModelManager->LoadModelFromHModel(pathOfModel, temp, modelIndex);
//		temp.Empty();
//
//		delete[] pName;
//		pName = NULL;
//		it = fileList.erase(it);
//	}
//
//	m_pEngine->FinishSetting();
//
//	return 0;
//}

//BOOL Viewer::CreateMaterialByText(wchar_t* szFileName)
//{
//	std::vector<wchar_t*> fileList;
//
//	CTextReader* pTxtReader = new CTextReader;
//	wchar_t* pFile = L"Media/Material/materialList.txt";
//
//	int iResult = HGUtility::IsFileExists(pFile);
//	if (iResult == 1)
//	{
//		pTxtReader->OpenTxt(pFile);
//	}
//
//	pTxtReader->ReadTxt(NULL);
//	pTxtReader->GetFileList(fileList);
//	pTxtReader->CloseTxt();
//	delete pTxtReader;
//
//	m_pEngine->StartSetting();
//	int texIndex = -1;
//	CString png = L".PNG";
//	CString pathOfMaterial = L"Media/Material/";
//
//	//CString colorchip = L"ColorChip";
//	//m_pMatManager->LoadTexture(pathOfMaterial, colorchip, png);
//
//	std::vector<wchar_t*>::iterator it;
//	for (it = fileList.begin(); it != fileList.end();)
//	{
//		CString temp = CString(*it);
//		texIndex = m_pMatManager->LoadTexture(pathOfMaterial, temp, png);
//
//		delete[] * it;
//		*it = NULL;
//		it = fileList.erase(it);
//	}
//
//	//m_pEngine->LoadSkyBox(L"Media/Skybox/Skybox.dds");			//스카이박스
//	//m_pEngine->LoadFont(L"Media/Fonts/SegoeUI_18.spritefont");	//폰트
//
//	m_pEngine->FinishSetting();
//
//	return 0;
//}

//BOOL Viewer::CreateModelByFolder()
//{
//	m_pEngine->StartSetting();
//	int index = -1;
//	for (const auto& entry : std::filesystem::recursive_directory_iterator("Media/Model"))
//	{
//		const auto& filePath = entry.path().string();
//
//		if (filePath.substr(filePath.length() - 7, 7) == ".hmodel")
//		{
//			std::size_t slash = filePath.rfind("\\");
//			std::size_t formatName = filePath.rfind(".hmodel");
//			std::string modelName = filePath.substr(slash + 1, formatName - slash - 1);
//			index++;
//			m_pModelManager->LoadModel(modelName, filePath);
//		}
//	}
//	m_pEngine->FinishSetting();
//	return 0;
//}

//BOOL Viewer::CreateMaterialByFolder()
//{
//	m_pEngine->StartSetting();
//	int index = -1;
//	for (const auto& entry : std::filesystem::recursive_directory_iterator("Media/Material"))
//	{
//		const auto& filePath = entry.path().string();
//
//		size_t slash = entry.path().string().rfind("\\");
//		size_t length = entry.path().string().size();
//		std::string materialName = filePath.substr(slash + 1, length);
//
//		TextureSet set;
//
//		for (const auto& matFolder : std::filesystem::recursive_directory_iterator(filePath.c_str()) )
//		{
//			std::string material = matFolder.path().string();
//
//			if (material.substr(material.length() - 4, 4) == ".png")
//			{
//				std::size_t slashPoint	= material.rfind("\\");
//				std::size_t underBar	= material.rfind("_");
//				std::size_t formatName	= material.rfind(".png");
//				std::string texKind		= material.substr(underBar, formatName - underBar - 1);
//
//				std::string matName = filePath.substr(slash + 1, formatName - slash - 1);
//			}
//			
//		}
//		//if (filePath.substr(filePath.length() - 3, 3) == ".png")
//		//{
//		//	std::size_t slash = filePath.rfind("\\");
//		//	std::size_t formatName = filePath.rfind(".hmodel");
//		//	std::string modelName = filePath.substr(slash + 1, formatName - slash - 1);
//		//	index++;
//		//	m_pModelManager->LoadModel(modelName, filePath);
//		//}
//	}
//	m_pEngine->FinishSetting();
//	return 0;
//}

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
//					target : 관촬대상				  //
//					front  : 정면 방향				  //
//					vUp위  :							  //
//*--------------------------------------------------*//
void Viewer::SetCam(DirectX::XMFLOAT3& vTarget, DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp)
{
	//vFront가 노멀라이즈 되어서 들어와야한다.
	//타겟으로 부터 떨어진 거리
	float fAway = 50.f;
	DirectX::XMFLOAT3 newCamPos = {};
	newCamPos.x = vTarget.x - vFront.x * fAway;
	newCamPos.y = vTarget.y - vFront.y * fAway;
	newCamPos.z = vTarget.z - vFront.z * fAway;

	m_pEngine->GetCamera()->LookAt(newCamPos, vTarget, vUp);

}


//경로, 이름, 확장자
wchar_t* Viewer::MakeFullPath(CString& dest, CString& path, CString& name, CString& tex, CString& extend)
{
	dest = path + name + L"/" + name + tex + extend;
	return dest.GetBuffer();
}

//
//HInstanceData* Viewer::AddGraphicInstance(object* pObj)
//{
//	HInstanceData* hIns = NULL;
//	HMaterialData* pMat = NULL;
//
//	DirectX::XMFLOAT4X4 mTm = MapUtil::Identity4x4();
//	pObj->GetTm(mTm);
//
//	int modelIndex = pObj->modelIndex;
//	if (modelIndex < 0)
//		modelIndex = 0;
//
//	int matIndex = pObj->matIndex;
//	if (matIndex < 0)
//		matIndex = 0;
//
//
//	//메테리얼 인덱스가 0일 경우 컬러칩으로 인식한다.
//	if (matIndex == 0)
//	{
//		hIns = m_pModelManager->GetModelList()->at(0)->hModel->AddInstance(ShaderType::COLORCHIP);
//	}
//	else
//	{
//		hIns = m_pModelManager->GetModelList()->at(0)->hModel->AddInstance(ShaderType::DEFAULT);
//		pMat = m_pMatManager->GetMatList()->at(0)->hMat;
//		hIns->SetMaterial(pMat, 0);
//	}
//
//	hIns->worldTM = mTm;
//
//	//예외 체크 필요하다. 리턴 값확인 필요.
//	if (hIns == NULL)
//		return NULL;
//	else
//		return hIns;
//
//}



void Viewer::RequestCreateObj(object* pObj)
{
	g_pCenter->CreateObj(pObj);
	//::SendMessageW(g_hCenter, WM_CREATE_OBJ, (WPARAM)(m_hWnd), (LPARAM)(pObj));
}

//포지션 셋팅/draw
//->SetPos(DirectX::XMFLOAT3& centerPos); 변경할 것.
void Viewer::ControlGizumo()
{
	//인스턴스관리자의 관리자를 만들면 모든 인스턴스의 중심좌표를 구하는 과정을 그것에 넣을 수 있다.
	DirectX::XMFLOAT3 centerPos = {};
	::SendMessageW(g_hCenter, WM_REQUEST_CENTER_POSITION, (WPARAM)&centerPos, NULL);

	m_Gizmo.SetPos(centerPos);
}

//드래그 범위 내의 오브젝트들 검사. 
void Viewer::WhenDragMouse()
{
	float point2[4] = {};
	
	float ndc_x = GetNDC_X(m_DragPoint[0]);
	float ndc_y = GetNDC_Y(m_DragPoint[0]);
	float ndc_lx = GetNDC_X(m_DragPoint[1]);
	float ndc_ly = GetNDC_Y(m_DragPoint[1]);

	point2[0] = max(ndc_x, ndc_lx);		//maxX
	point2[1] = min(ndc_x, ndc_lx);		//minX
	point2[2] = max(ndc_y, ndc_ly);		//maxY
	point2[3] = min(ndc_y, ndc_ly);		//minY

	::SendMessageW(g_hCenter, WM_OBJECT_SELECT_IN_RECT, (WPARAM)point2, NULL);
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

	//기즈모 컨트롤 모드 변경
	case 'W':
		m_EditMode = EDIT_POS;
		break;
	case 'E':
		m_EditMode = EDIT_ROT;
		break;
	case 'R':
		m_EditMode = EDIT_SCALE;
		break;


		//뷰 변경
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
	CDialogEx::OnRButtonDown(nFlags, point);
}

void Viewer::OnRButtonUp(UINT nFlags, CPoint point)
{
	//m_bIsRButtonClick = FALSE;
	m_pCamController->MovementAvailable(false);
	m_RBtnDownPos.x = 0;
	m_RBtnDownPos.y = 0;
	CDialogEx::OnRButtonUp(nFlags, point);
}

void Viewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLbuttonDown = true;

	unsigned int x = point.x;
	unsigned int y = point.y;

	m_DragPoint[0].x = x;
	m_DragPoint[0].y = y;

	m_bGizmoTime = m_Gizmo.CheckSelectedIndex(x, y, m_iScreenWidth, m_iScreenHeight);		//기르모를 클릭했는가? 
	m_bGizmoCubeTime = m_Gizmo.IsPickCube(x, y);											//큐브를 클릭했는가
	

	if ((m_bGizmoTime == FALSE) && (m_bGizmoCubeTime == FALSE))
	{
		::SendMessageW(g_hCenter, WM_VIEWER_LBUTTONDOWN, (WPARAM)point.x, (LPARAM)point.y);
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void Viewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	//OutputDebugStringW(L"Viewer::LButtonDown()\n");
	 
	//축
	m_Gizmo.ReleaseAxis();
	m_bLbuttonDown = false;
	
	//기지모 큐브
	m_Gizmo.ReleaseCube();
	m_bGizmoCubeTime = FALSE;

	//Viewer에서 마우스 왼번튼을 떼었을 때.
	::SendMessageW(g_hCenter, WM_VIEWER_LBUTTONUP, NULL, NULL);
	
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
			float ndcX1 = GetNDC_X(m_DragPoint[0]);			//일반화 된 함수로 개선 필요
			float ndcY1 = GetNDC_Y(m_DragPoint[0]);
			float ndcX2 = GetNDC_X(m_DragPoint[1]);
			float ndcY2 = GetNDC_Y(m_DragPoint[1]);

			WhenDragMouse();	//드래그 범위 내위 내의 오브젝트를 선택한다.
			
		}
	}

	last.x = cur.x;
	last.y = cur.y;

	//카메라 업데이트
	m_pEngine->GetCamera()->UpdateViewMatrix();

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL Viewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//마우스 휠을 이용해 높이값 조절

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void Viewer::OnClose()
{
	int a = 0;
	CDialogEx::OnClose();
}
