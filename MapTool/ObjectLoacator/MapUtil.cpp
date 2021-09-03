#include "pch.h"
#include "MapUtil.h"
#include <limits.h>

void MapUtil::GetXMFLOAT3(DirectX::XMFLOAT3& dest, float x, float y, float z)
{
	dest.x = x;
	dest.y = y;
	dest.z = z;
}

//일단 더미로 아이덴티티 메트릭스
void MapUtil::ConvertVector3ToMatrix_pos(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4X4& mOut)
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMStoreFloat4x4(&mOut, mTrans);
}

//일단 더미로 아이덴티티 메트릭스 반환
void MapUtil::ConvertVector3ToMatrix_scale(DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4X4& mOut)
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

DirectX::XMFLOAT4X4 MapUtil::Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}

void MapUtil::PassFocus(HWND hWnd)
{
	::ShowWindow(hWnd, SW_RESTORE);
	::SetForegroundWindow(hWnd);
}

//디버그 상태에만 동작
void MapUtil::DebugString(wchar_t* pStr)
{
#ifdef DEBUG
	DWORD dwRet = GetLastError();
	WCHAR buffer[256];
	wsprintfW(buffer, L"%s ERROR CODE : %d", pStr, dwRet);
	OutputDebugStringW(buffer);
#endif // DEBUG
}

BOOL MapUtil::CheckDragArea(CPoint p1, CPoint p2, CPoint target)
{
	enum
	{
		MIN = 0,
		MAX,

	};
	float x[2] = { FLT_MAX, FLT_MIN };
	float y[2] = { FLT_MAX, FLT_MIN };

	CPoint point[2] = { p1, p2 };
	for (int i = 0; i < 2; i++)
	{
		x[MAX] = max(point[0].x, x[MAX]);
		x[MIN] = min(point[0].x, x[MIN]);
		y[MAX] = max(point[0].y, y[MAX]);
		y[MIN] = min(point[0].y, y[MIN]);
	}

	if (target.x < x[MIN]) return FALSE;
	if (target.x > x[MAX]) return FALSE;
	if (target.y < y[MIN]) return FALSE;
	if (target.y > y[MAX]) return FALSE;

	return TRUE;
}

/*
	아래 4가지 확인 확인되면 FALSE 리턴.
	
	     *
	---------
	|		|
  *	|		|  
	|		|    *
	|		|
	---------
	   *
	
	false 체크,
	모두 통과하면 true
*/
BOOL MapUtil::AABB(float xMin, float xMax, float yMin, float yMax, float x, float y)
{
	if (x < xMin) return FALSE;
	if (x > xMax) return FALSE;
	if (y < yMin) return FALSE;
	if (y > yMax) return FALSE;
	return TRUE;
}

//최소값은 0.f
float MapUtil::GetDlgItemFloat(HWND hWnd, int ID)
{
	WCHAR strValue[256];
	::GetDlgItemTextW(hWnd, ID, strValue, 256);
	float fValue = _tstof(strValue);

	CStringW ret;
	ret.Format(L"%.2f", fValue);

	::SetDlgItemTextW(hWnd, ID, ret);

	ret.Empty();

	return fValue;
}

void MapUtil::SetDlgItemFloat(HWND hWnd, int ID, float fInput)
{
	CString str;
	str.Format(L"%.2f", fInput);
	::SetDlgItemTextW(hWnd, ID, str);
	str.Empty();
}

int MapUtil::GetDlgItemInt(HWND hWnd, int ID)
{
	WCHAR strValue[256];
	::GetDlgItemTextW(hWnd, ID, strValue, 256);
	int iValue = _tstoi(strValue);

	CString ret;
	ret.Format(L"%d", iValue);

	::SetDlgItemTextW(hWnd, ID, ret);

	ret.Empty();

	return iValue;
}

void MapUtil::SetDlgItemInt(HWND hWnd, int iD, int iValue)
{
	CString str;
	str.Format(L"%d", iValue);
	::SetDlgItemTextW(hWnd, iD, str);
	str.Empty();
	str.ReleaseBuffer();
}

BOOL MapUtil::CallbackEnumWindowProc(HWND hWnd, LPARAM lParam)
{
	HANDLE hTarget = NULL;
	DWORD dwID = 0, dwSrcID = (DWORD)(*(int*)lParam);
	GetWindowThreadProcessId(hWnd, &dwID);
	if (dwID == dwSrcID)
	{
		*((int*)lParam) = (int)hWnd;
		*(((int*)lParam) + 1) = 1;
		return FALSE;
	}

	return TRUE;
}

HWND MapUtil::GetHwndFromProcessID(DWORD dwProcessID)
{
	if (dwProcessID == 0 || dwProcessID == -1) return NULL;
	__int64 temp = 0;
	temp = (int)dwProcessID;

	EnumWindows(CallbackEnumWindowProc, (LPARAM)&temp);		//temp in/out

	if ((int)(temp >> 32))
		return (HWND)(int)temp;

	return NULL;
}

ULONG MapUtil::ProcIDFromWnd(HWND hWnd)
{
	ULONG idProc;
	GetWindowThreadProcessId(hWnd, &idProc);
	return idProc;
}

HWND MapUtil::GetWinHandle(ULONG pid)
{
	HWND tempHwnd = FindWindow(NULL, NULL);

	while (tempHwnd != NULL)
	{
		if (GetParent(tempHwnd) == NULL)
		{
			if (pid == ProcIDFromWnd(tempHwnd))
			{
				return tempHwnd;
			}
		}
		tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT);
	}
	return NULL;
}

void MapUtil::GetDlgTextW(HWND hWnd, int id, WCHAR* pDest, int bufferSize)
{
	::GetDlgItemTextW(hWnd, id, pDest, bufferSize);
}

void MapUtil::SetDlgTextW(HWND hWnd, int id, WCHAR* pDest)
{
	::SetDlgItemTextW(hWnd, id, pDest);
}

void MapUtil::GetDlgTextA(HWND hWnd, int id, char* pDest, int bufferSize)
{
	::GetDlgItemTextA(hWnd, id, pDest, bufferSize);
}

void MapUtil::SetDlgTextA(HWND hWnd, int id, char* pDest)
{
	::SetDlgItemTextA(hWnd, id, pDest);
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2)
{
	DirectX::XMFLOAT3 vRet;
	vRet.x = v1.x - v2.x;
	vRet.y = v1.y - v2.y;
	vRet.z = v1.z - v2.z;

	return vRet;
}

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2)
{
	DirectX::XMFLOAT3 vRet;
	vRet.x = v1.x + v2.x;
	vRet.y = v1.y + v2.y;
	vRet.z = v1.z + v2.z;

	return vRet;
}