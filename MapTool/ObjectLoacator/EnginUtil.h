#pragma once
#include <DirectXMath.h>



namespace mapUtil
{


	void GetXMFLOAT3(DirectX::XMFLOAT3& dest, float x, float y, float z);

	void ConvertVector3ToMatrix_pos(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4X4& mOut);
	void ConvertVector3ToMatrix_rot(DirectX::XMFLOAT3& rot, DirectX::XMFLOAT4X4& mOut);
	void ConvertVector3ToMatrix_scale(DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4X4& mOut);
	DirectX::XMFLOAT4X4 Identity4x4();

	/*
	void PassFocusW(const WCHAR* str);
	void PassFocusA(const char* str);
	*/
	void PassFocus(HWND hWnd);

//#ifdef UNICODE
//#define PassFocus PassFocusW
//#else
//#define PassFocus PassFocusA
//#endif // !UNICODE

};

