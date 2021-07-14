#pragma once
#include <DirectXMath.h>

//MFC defendency
namespace MapUtil
{
	void GetXMFLOAT3(DirectX::XMFLOAT3& dest, float x, float y, float z);

	void ConvertVector3ToMatrix_pos(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4X4& mOut);
	void ConvertVector3ToMatrix_rot(DirectX::XMFLOAT3& rot, DirectX::XMFLOAT4X4& mOut);
	void ConvertVector3ToMatrix_scale(DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4X4& mOut);
	DirectX::XMFLOAT4X4 Identity4x4();

	void PassFocus(HWND hWnd);

	void DebugString(wchar_t* pStr);

	BOOL CheckDragArea(CPoint p1, CPoint p2, CPoint target);		//삭제
	BOOL AABB(float xMin, float xMax, float yMin, float yMax, float x, float y);

	float GetDlgItemFloat(HWND hWnd, int ID);				//EditControl값 받기 시리즈
	void SetDlgItemFloat(HWND hWnd, int ID, float fInput);
	int  GetDlgItemInt(HWND hWnd, int ID);
	void SetDlgItemInt(HWND hWnd, int iD, int iValue);

	BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam);
	HWND GetHwndFromProcessID(DWORD dwProcessID);

	ULONG ProcIDFromWnd(HWND hWnd);
	HWND  GetWinHandle(ULONG pid);

	//삭제 예정
	void GetDlgTextW(HWND hWnd, int id, WCHAR* pDest, int bufferSize);
	void SetDlgTextW(HWND hWnd, int id, WCHAR* pDest);
	void GetDlgTextA(HWND hWnd, int id, char* pDest, int bufferSize);
	void SetDlgTextA(HWND hWnd, int id, char* pDest);
};

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3, const DirectX::XMFLOAT3);
DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3, const DirectX::XMFLOAT3);

