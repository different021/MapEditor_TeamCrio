#include "pch.h"
#include "Gizmo.h"

BOOL Gizmo::IsSelectAxis(AXIS axis, float ndcX, float ndcY)
{
	float x = ndcX;
	float y = ndcY;

	XMFLOAT3 dot[4];
	XMMATRIX mView = _pEngine->GetCamera()->GetView();
	XMMATRIX mProj = _pEngine->GetCamera()->GetProj();
	XMVECTOR vDot[4];
	XMVECTOR viewDot[4];
	XMVECTOR projDot[4];

	for (int i = 0; i < 4; i++)
	{
		vDot[i] = XMLoadFloat3(&_pRect[axis]->dots[i]);
		viewDot[i] = XMVector3TransformCoord(vDot[i], mView);		//뷰변환
		projDot[i] = XMVector3TransformCoord(viewDot[i], mProj);	//프로젝션
		XMStoreFloat3(&dot[i], projDot[i]);
	}


	float xMax = dot[0].x;
	float xMin = dot[0].x;
	for (int i = 1; i < 4; i++)
	{
		xMax = max(dot[i].x, xMax);
		xMin = min(dot[i].x, xMin);
	}

	float yMax = dot[0].y;
	float yMin = dot[0].y;
	for (int i = 1; i < 4; i++)
	{
		yMax = max(dot[i].y, yMax);
		yMin = min(dot[i].y, yMin);
	}

	WCHAR buf[128];
	if (axis == AXIS_X)
	{
		wsprintf(buf, L"AXIS_X(%d, %d, %d, %d)\n", static_cast<int>(xMax * 10000), static_cast<int>(xMin * 10000), static_cast<int>(yMax * 10000), static_cast<int>(yMin * 10000));
	}
	else if (axis == AXIS_Y)
	{
		wsprintf(buf, L"AXIS_Y(%d, %d, %d, %d)\n", static_cast<int>(xMax * 10000), static_cast<int>(xMin * 10000), static_cast<int>(yMax * 10000), static_cast<int>(yMin * 10000));
	}
	else if (axis == AXIS_Z)
	{
		wsprintf(buf, L"AXIS_Z(%d, %d, %d, %d)\n", static_cast<int>(xMax * 10000), static_cast<int>(xMin * 10000), static_cast<int>(yMax * 10000), static_cast<int>(yMin * 10000));
	}
	OutputDebugString(buf);

	return AABB(xMin, xMax, yMin, yMax, x, y);
}

//사각형 내부에 점이 있는지 판정
BOOL Gizmo::AABB(float xMin, float xMax, float yMin, float yMax, float x, float y)
{
	if (x < xMin) return FALSE;
	if (x > xMax) return FALSE;
	if (y < yMin) return FALSE;
	if (y > yMax) return FALSE;
	return TRUE;
}

float Gizmo::GetNdcX(int x)
{
	float ndcX = ((static_cast<float>(x) * 2.f) / _screenWidth) - 1;
	return ndcX;
}

float Gizmo::GetNdcY(int y)
{
	float ndcY = ((static_cast<float>(-y) * 2.f) / _screenHeight) + 1;
	return ndcY;
}

void Gizmo::CreateCube(Vector3 size)
{
	if (_pCube != NULL)
	{
		_pCube->Delete();
		_pCube = NULL;
	}

	_pCube = _pEngine->CreateBox(size, eNoOption_Primitive);
}

void Gizmo::SetCubePos(DirectX::XMFLOAT3& pos)
{
	XMFLOAT3 myPos = pos;											//
	XMVECTOR vMyPos = XMLoadFloat3(&myPos);							//
	XMMATRIX viewMat = _pCamera->GetView();
	XMVECTOR viewPos = XMVector3TransformCoord(vMyPos, viewMat);
	float zDistance = XMVectorGetZ(viewPos);
	_calculatedCubeSize = zDistance * _cubeSize / _pCamera->GetNearZ();

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(_calculatedCubeSize, _calculatedCubeSize, _calculatedCubeSize);

	DirectX::XMMATRIX mTM = mScale * mTrans;
	DirectX::XMStoreFloat4x4(&_pCube->worldTM, mTM);
	//Vector3 size = Vector3(cubeSize, cubeSize, cubeSize);
}

void Gizmo::Setup(HEngine_DX12_3D* pEngine)
{
	_pEngine = pEngine;
	_pCamera = _pEngine->GetCamera();

	//기즈모 끝부분 네모.
	_pRect[AXIS_X] = _pEngine->CreateRect(eDepthOff_Primitive);
	_pRect[AXIS_Y] = _pEngine->CreateRect(eDepthOff_Primitive);
	_pRect[AXIS_Z] = _pEngine->CreateRect(eDepthOff_Primitive);

	_pRect[AXIS_X]->color = XMFLOAT4(1, 0, 0, 1);	//R
	_pRect[AXIS_Y]->color = XMFLOAT4(0.4f, 0.7f, 0.4f, 1);	//G
	_pRect[AXIS_Z]->color = XMFLOAT4(0, 0, 1, 1);	//B

	_pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	_exPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

	_pRect[AXIS_X]->color = XMFLOAT4(1, 0, 0, 0.5);				//R
	_pRect[AXIS_Y]->color = XMFLOAT4(0.4f, 0.7f, 0.4f, 0.5);	//G
	_pRect[AXIS_Z]->color = XMFLOAT4(0, 0, 1, 0.5);				//B

	//ShaderType , ShaderType_Primitive와 햇갈릴때가 있다.
	_pLine[AXIS_X] = _pEngine->CreateLine(eDepthOff_Primitive);
	_pLine[AXIS_Y] = _pEngine->CreateLine(eDepthOff_Primitive);
	_pLine[AXIS_Z] = _pEngine->CreateLine(eDepthOff_Primitive);

	// X
	_pLine[AXIS_X]->color = XMFLOAT4(1, 0, 0, 1);				//R
	_pLine[AXIS_X]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);
	_pLine[AXIS_X]->dots[1] = XMFLOAT3(_gizmoSize, 0.f, 0.f);

	//Y
	_pLine[AXIS_Y]->color = XMFLOAT4(0.4f, 0.7f, 0.4f, 1);	//G
	_pLine[AXIS_Y]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);
	_pLine[AXIS_Y]->dots[1] = XMFLOAT3(0.f, _gizmoSize, 0.f);

	//Z
	_pLine[AXIS_Z]->color = XMFLOAT4(0, 0, 1, 1);				//B
	_pLine[AXIS_Z]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);
	_pLine[AXIS_Z]->dots[1] = XMFLOAT3(0.f, 0.f, _gizmoSize);

	CreateCube(Vector3(1, 1, 1));
}

void Gizmo::Delete()
{
	for (int i = 0; i < AXIS_MAX; i++)
	{
		if (_pLine[i] != NULL)	_pLine[i]->Delete();
	}

}

void Gizmo::SetPos(DirectX::XMFLOAT3& pos)
{
	XMFLOAT3 myPos = pos;
	_exPos = _pos;
	_pos = pos;

	SetArrow(_pos);
	SetCubePos(_pos);
}

Gizmo::~Gizmo()
{
	Delete();
}

void Gizmo::GetPos(DirectX::XMFLOAT3& dest)
{
	dest.x = _pos.x;
	dest.y = _pos.y;
	dest.z = _pos.z;
}


void Gizmo::SetArrow(DirectX::XMFLOAT3& pos)
{
	XMFLOAT3 myPos = pos;											//
	XMVECTOR vMyPos = XMLoadFloat3(&myPos);							//
	XMMATRIX viewMat = _pCamera->GetView();
	XMVECTOR viewPos = XMVector3TransformCoord(vMyPos, viewMat);
	float zDistance = XMVectorGetZ(viewPos);
	float calculatedGizmoLength = zDistance * _gizmoSize / _pCamera->GetNearZ();

	_pLine[AXIS_X]->dots[0] = _pos;
	_pLine[AXIS_X]->dots[1] = XMFLOAT3(_pos.x + calculatedGizmoLength, _pos.y, _pos.z);

	_pLine[AXIS_Y]->dots[0] = _pos;
	_pLine[AXIS_Y]->dots[1] = XMFLOAT3(_pos.x, _pos.y + calculatedGizmoLength, _pos.z);

	_pLine[AXIS_Z]->dots[0] = _pos;
	_pLine[AXIS_Z]->dots[1] = XMFLOAT3(_pos.x, _pos.y, _pos.z + calculatedGizmoLength);

	float rectOneSide = 0.0125;
	float calculatedOneSide = zDistance * rectOneSide / _pCamera->GetNearZ();

	_pRect[AXIS_X]->dots[0] = _pLine[AXIS_X]->dots[1];
	_pRect[AXIS_X]->dots[1] = _pLine[AXIS_X]->dots[1];
	_pRect[AXIS_X]->dots[2] = _pLine[AXIS_X]->dots[1];
	_pRect[AXIS_X]->dots[3] = _pLine[AXIS_X]->dots[1];

	_pRect[AXIS_Y]->dots[0] = _pLine[AXIS_Y]->dots[1];
	_pRect[AXIS_Y]->dots[1] = _pLine[AXIS_Y]->dots[1];
	_pRect[AXIS_Y]->dots[2] = _pLine[AXIS_Y]->dots[1];
	_pRect[AXIS_Y]->dots[3] = _pLine[AXIS_Y]->dots[1];

	_pRect[AXIS_Z]->dots[0] = _pLine[AXIS_Z]->dots[1];
	_pRect[AXIS_Z]->dots[1] = _pLine[AXIS_Z]->dots[1];
	_pRect[AXIS_Z]->dots[2] = _pLine[AXIS_Z]->dots[1];
	_pRect[AXIS_Z]->dots[3] = _pLine[AXIS_Z]->dots[1];

	_pRect[AXIS_X]->dots[0].x = _pRect[AXIS_X]->dots[0].x - calculatedOneSide;
	_pRect[AXIS_X]->dots[0].z = _pRect[AXIS_X]->dots[0].z - calculatedOneSide;
	_pRect[AXIS_X]->dots[1].x = _pRect[AXIS_X]->dots[1].x - calculatedOneSide;
	_pRect[AXIS_X]->dots[1].z = _pRect[AXIS_X]->dots[1].z + calculatedOneSide;
	_pRect[AXIS_X]->dots[2].x = _pRect[AXIS_X]->dots[2].x + calculatedOneSide;
	_pRect[AXIS_X]->dots[2].z = _pRect[AXIS_X]->dots[2].z + calculatedOneSide;
	_pRect[AXIS_X]->dots[3].x = _pRect[AXIS_X]->dots[3].x + calculatedOneSide;
	_pRect[AXIS_X]->dots[3].z = _pRect[AXIS_X]->dots[3].z - calculatedOneSide;

	_pRect[AXIS_Y]->dots[0].x = _pRect[AXIS_Y]->dots[0].x - calculatedOneSide;
	_pRect[AXIS_Y]->dots[0].y = _pRect[AXIS_Y]->dots[0].y - calculatedOneSide;
	_pRect[AXIS_Y]->dots[1].x = _pRect[AXIS_Y]->dots[1].x - calculatedOneSide;
	_pRect[AXIS_Y]->dots[1].y = _pRect[AXIS_Y]->dots[1].y + calculatedOneSide;
	_pRect[AXIS_Y]->dots[2].x = _pRect[AXIS_Y]->dots[2].x + calculatedOneSide;
	_pRect[AXIS_Y]->dots[2].y = _pRect[AXIS_Y]->dots[2].y + calculatedOneSide;
	_pRect[AXIS_Y]->dots[3].x = _pRect[AXIS_Y]->dots[3].x + calculatedOneSide;
	_pRect[AXIS_Y]->dots[3].y = _pRect[AXIS_Y]->dots[3].y - calculatedOneSide;

	_pRect[AXIS_Z]->dots[0].x = _pRect[AXIS_Z]->dots[0].x - calculatedOneSide;
	_pRect[AXIS_Z]->dots[0].z = _pRect[AXIS_Z]->dots[0].z - calculatedOneSide;
	_pRect[AXIS_Z]->dots[1].x = _pRect[AXIS_Z]->dots[1].x - calculatedOneSide;
	_pRect[AXIS_Z]->dots[1].z = _pRect[AXIS_Z]->dots[1].z + calculatedOneSide;
	_pRect[AXIS_Z]->dots[2].x = _pRect[AXIS_Z]->dots[2].x + calculatedOneSide;
	_pRect[AXIS_Z]->dots[2].z = _pRect[AXIS_Z]->dots[2].z + calculatedOneSide;
	_pRect[AXIS_Z]->dots[3].x = _pRect[AXIS_Z]->dots[3].x + calculatedOneSide;
	_pRect[AXIS_Z]->dots[3].z = _pRect[AXIS_Z]->dots[3].z - calculatedOneSide;
}




void Gizmo::Reset()
{
	_pRect[AXIS_X]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);	//각 축의 길이 1
	_pRect[AXIS_X]->dots[1] = XMFLOAT3(_gizmoSize, 0.f, 0.f);

	_pRect[AXIS_Y]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);
	_pRect[AXIS_Y]->dots[1] = XMFLOAT3(0.f, _gizmoSize, 0.f);

	_pRect[AXIS_Z]->dots[0] = XMFLOAT3(0.f, 0.f, 0.f);
	_pRect[AXIS_Z]->dots[1] = XMFLOAT3(0.f, 0.f, _gizmoSize);

	//SetDirection(XMFLOAT3(0, 0, 0));
}


void Gizmo::SetSize(float size)
{
	_gizmoSize = size;
}

//세 축중 하나라도 클릭되면 true;
BOOL Gizmo::CheckSelectedIndex(unsigned int screenX, unsigned int screenY, int screenWidth, int screenHeight)
{
	//BOOL bResult = FALSE;
	int x = screenX;
	int y = screenY;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	float ndcX = GetNdcX(x);
	float ndcY = GetNdcY(y);

	_bSelectAxis[AXIS_X] = IsSelectAxis(AXIS_X, ndcX, ndcY);
	_bSelectAxis[AXIS_Y] = IsSelectAxis(AXIS_Y, ndcX, ndcY);
	_bSelectAxis[AXIS_Z] = IsSelectAxis(AXIS_Z, ndcX, ndcY);

	return (_bSelectAxis[AXIS_X] || _bSelectAxis[AXIS_Y] || _bSelectAxis[AXIS_Z]);
}

BOOL Gizmo::CheckAxis(AXIS axis)
{
	if (axis > AXIS_MAX)
	{
		OutputDebugStringW(L"GIZMO: Out of range\n");
		return FALSE;
	}

	return _bSelectAxis[axis];
}

//해당 큐브가 선택되었는지 검사.
BOOL Gizmo::IsPickCube(unsigned int screenX, unsigned int screenY)
{
	Camera* pCam = _pEngine->GetCamera();
	DirectX::XMFLOAT4X4 mProj = pCam->GetProj4x4f();

	//광선 구하기
	float vx	= (( 2.f * screenX / _screenWidth ) - 1.f) / mProj(0, 0);       //나눠주는것이 무슨 의미가 있었는데 까먹음.
	float vy	= ((-2.f * screenY / _screenHeight) + 1.f) / mProj(1, 1);

	//w값은? (vector와 position에서 w값의 차이가 생기는 원인)
	//Position  = 1.f; 딱히 뺄셈이 아니라 남아 있음
	//Direction = 0.f; vEnd - vStart = w성분이 삭제됨.
	DirectX::XMVECTOR vCamPos_view = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
	DirectX::XMVECTOR vRayDir_view = XMVectorSet(vx, vy, 1.0f, 0.0f);;

	XMMATRIX mView	 = pCam->GetView();
	XMVECTOR det	 = XMMatrixDeterminant(mView);
	XMMATRIX invView = XMMatrixInverse(&det, mView);

	float fMin = FLT_MAX;	//1.175494351 E - 38

	XMVECTOR mCam_world = XMVector3TransformCoord(vCamPos_view, invView);
	XMVECTOR mRay_world = XMVector3TransformNormal(vRayDir_view, invView);

	mRay_world = XMVector3Normalize(mRay_world);

	DirectX::BoundingBox box = BoundingBox();
	box.Center.x = _pos.x;
	box.Center.y = _pos.y;
	box.Center.z = _pos.z;

	box.Extents.x = _calculatedCubeSize;
	box.Extents.y = _calculatedCubeSize;
	box.Extents.z = _calculatedCubeSize;

	float lengthOfRay = 0;
	bool isHit = box.Intersects(mCam_world, mRay_world, lengthOfRay);
	_bIsCubeTime = isHit;

	return _bIsCubeTime;
}


//선택된 모든 축 해제.
void Gizmo::ReleaseAxis()
{
	for (int i = 0; i < AXIS_MAX; i++)
	{
		_bSelectAxis[i] = FALSE;
	}
}

void Gizmo::ReleaseCube()
{
	_bIsCubeTime = FALSE;
}

HProceduralGeometry_rect* Gizmo::GetDebugRect(AXIS axis)
{
	return _pRect[axis];
}

