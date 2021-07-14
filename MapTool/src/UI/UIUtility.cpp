#include "pch.h"
#include "UIUtility.h"
#include "EngineInterface.h"

//return under Zero is Invailded Parameter
float UIUtility::GetPosRatio(int pos, int length)
{
	float fResult = 0.f;
	if (length <= 0)
	{
		//잘못된 길이 값. 
		return -1.f;
	}

	fResult = static_cast<float>(pos) / static_cast<float>(length);
	return fResult;
}

void UIUtility::GetNdcFromScreenPos(DirectX::XMFLOAT3& out, int screenX, int screenY, int screenWidth, int screenHeight)
{
	int width  = screenWidth;
	int height = screenHeight;

	float ndcX = (( 2.f * screenX / width ) - 1.f);
	float ndcY = ((-2.f * screenY / height) + 1.f);

	out = { ndcX, ndcY, 0.999f };
}

void UIUtility::GetWorldPosFromNDC(DirectX::XMFLOAT3& out, DirectX::XMFLOAT3& NDC, HEngine_DX12_3D* pEngine)
{
	Camera* pCam = pEngine->GetCamera();
	XMVECTOR ndcPos;
	XMVECTOR unProj;
	XMVECTOR unProjView;

	ndcPos = DirectX::XMLoadFloat3(&NDC);

	XMMATRIX mProj	 = pCam->GetProj();
	XMVECTOR detP	 = XMMatrixDeterminant(mProj);
	XMMATRIX invProj = XMMatrixInverse(&detP, mProj);

	XMMATRIX mView	 = pCam->GetView();
	XMVECTOR det	 = XMMatrixDeterminant(mView);
	XMMATRIX invView = XMMatrixInverse(&det, mView);

	unProj		= XMVector3TransformCoord(ndcPos, invProj);
	unProjView	= XMVector3TransformCoord(unProj, invView);

	XMStoreFloat3(&out, unProjView);
}
