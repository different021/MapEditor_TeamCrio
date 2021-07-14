#pragma once
#include <DirectXMath.h>

class HEngine_DX12_3D;

namespace UIUtility
{
	float GetPosRatio(int pos, int length);
	void  GetNdcFromScreenPos(DirectX::XMFLOAT3& out, int screenX, int screenY, int screenWidth, int screenHeight);
	void  GetWorldPosFromNDC(DirectX::XMFLOAT3& out, DirectX::XMFLOAT3& NDC, HEngine_DX12_3D* pEngine);
};

