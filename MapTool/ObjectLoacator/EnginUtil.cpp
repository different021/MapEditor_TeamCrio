#include "pch.h"
#include "EnginUtil.h"


void mapUtil::GetXMFLOAT3(DirectX::XMFLOAT3& dest, float x, float y, float z)
{
	dest.x = x;
	dest.y = y;
	dest.z = z;
}

//일단 더미로 아이덴티티 메트릭스
void mapUtil::ConvertVector3ToMatrix_pos(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4X4& mOut)
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMStoreFloat4x4(&mOut, mTrans);
}

//일단 더미로 아이덴티티 메트릭스 반환
void mapUtil::ConvertVector3ToMatrix_scale(DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4X4& mOut)
{
	/*
	DirectX::XMVECTOR vScale = DirectX::XMLoadFloat3(&scale);
	DirectX::XMMATRIX mScale;
	*/

	//더미데이터다. 추후 수정할 것.
	mOut._11 = 1.0f;
	mOut._12 = 0.0f;
	mOut._13 = 0.0f;
	mOut._14 = 0.0f;

	mOut._21 = 0.0f;
	mOut._22 = 1.0f;
	mOut._23 = 0.0f;
	mOut._24 = 0.0f;

	mOut._31 = 0.0f;
	mOut._32 = 0.0f;
	mOut._33 = 1.0f;
	mOut._34 = 0.0f;

	mOut._41 = 0.0f;
	mOut._42 = 0.0f;
	mOut._43 = 0.0f;
	mOut._44 = 1.0f;
}

DirectX::XMFLOAT4X4 mapUtil::Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

//void mapUtil::PassFocusW(const WCHAR* str)
//{
//	HWND hWnd = ::FindWindowW(NULL, str);
//	if (hWnd)
//	{
//		::ShowWindow(hWnd, SW_RESTORE);
//		::SetForegroundWindow(hWnd);
//	}
//}
//
//void mapUtil::PassFocusA(const char* str)
//{
//	HWND hWnd = ::FindWindowA(NULL, str);
//	if (hWnd)
//	{
//		::ShowWindow(hWnd, SW_RESTORE);
//		::SetForegroundWindow(hWnd);
//	}
//}

void mapUtil::PassFocus(HWND hWnd)
{
	::ShowWindow(hWnd, SW_RESTORE);
	::SetForegroundWindow(hWnd);
}

