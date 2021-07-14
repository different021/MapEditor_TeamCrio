#include "pch.h"
#include "CamController.h"

CamController::CamController()
{
	m_pEngine->GetInstance();
	m_pCamera = m_pEngine->GetCamera();
}

CamController::~CamController()
{
	//m_pEngine->EndEngine();
}

void CamController::Update(int curX, int curY, int lastX, int lastY)
{
	if (m_bCanMove) MoveCam( curX,  curY,  lastX,  lastY);
	if (m_bCanRot)	RotCam(curX, curY, lastX, lastY);
}

void CamController::MoveFront(float fDelta, float deltaTime)
{
	m_pCamera = m_pEngine->GetCamera();
	DirectX::XMFLOAT3 front  = m_pCamera->GetLook3f();
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPosition3f();
	
	camPos.x += front.x * fDelta * deltaTime;
	camPos.y += front.y * fDelta * deltaTime;
	camPos.z += front.z * fDelta * deltaTime;
	m_pCamera->SetPosition(camPos);
}

void CamController::MoveRight(float fDelta, float deltaTime)
{
	m_pCamera = m_pEngine->GetCamera();
	DirectX::XMFLOAT3 right  = m_pCamera->GetRight3f();
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPosition3f();

	camPos.x += right.x * fDelta * deltaTime;
	camPos.y += right.y * fDelta * deltaTime;
	camPos.z += right.z * fDelta * deltaTime;
	m_pCamera->SetPosition(camPos);
}

void CamController::MoveUp(float fDelta, float deltaTime)
{
	m_pCamera = m_pEngine->GetCamera();
	DirectX::XMFLOAT3 up = m_pCamera->GetUp3f();
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPosition3f();

	camPos.x += up.x * fDelta * deltaTime;
	camPos.y += up.y * fDelta * deltaTime;
	camPos.z += up.z * fDelta * deltaTime;
	m_pCamera->SetPosition(camPos);
}

void CamController::MoveCam(int curX, int curY, int lastX, int lastY)
{
	int x = curX;
	int y = curY;
	int lastPosX = lastX;
	int lastPosY = lastY;

	int dx = lastPosX - x;
	int dy = y - lastPosY;
	m_pCamera = m_pEngine->GetCamera();
	DirectX::XMFLOAT3 right = m_pCamera->GetRight3f();
	DirectX::XMFLOAT3 front = m_pCamera->GetLook3f();
	DirectX::XMFLOAT3 up = m_pCamera->GetUp3f();

	DirectX::XMFLOAT3 camPos = m_pCamera->GetPosition3f();

	float ratio = 0.005f * camPos.y;
	if (camPos.y == 0.f)
	{
		ratio = 0.005f;
	}

	camPos.x = (camPos.x - right.x * (dx * ratio));
	camPos.y = (camPos.y - right.y * (dx * ratio));
	camPos.z = (camPos.z - right.z * (dx * ratio));

	camPos.x = (camPos.x - up.x * (dy * ratio));
	camPos.y = (camPos.y - up.y * (dy * ratio));
	camPos.z = (camPos.z - up.z * (dy * ratio));

	//m_pEngine->GetCamera()->SetLens(0.25f * XM_PI, 100.0f, 100.0f, 1.0f, 3000.0f);
	m_pEngine->GetCamera()->SetPosition(camPos);
}

void CamController::RotCam(int curX, int curY, int lastX, int lastY)
{
	int x = curX;
	int y = curY;
	int lastPosX = lastX;
	int lastPosY = lastY;

	int xVec = x - lastPosX;
	int yVec = y - lastPosY;
	lastPosX = x;
	lastPosY = y;

	float dx = XMConvertToRadians(0.25f * static_cast<float>(xVec));
	float dy = XMConvertToRadians(0.25f * static_cast<float>(yVec));

	m_pEngine->GetCamera()->Pitch(dy);
	m_pEngine->GetCamera()->RotateY(dx);
}

void CamController::RotationAvailable(bool isRot)
{
	m_bCanRot = isRot;
}

void CamController::MovementAvailable(bool isMove)
{
	m_bCanMove = isMove;
}
