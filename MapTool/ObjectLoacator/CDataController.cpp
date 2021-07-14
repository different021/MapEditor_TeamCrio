#include "pch.h"
#include "CDataController.h"
#include "WMDefine.h"


float CDataController::GetDlgItemFloat(int ID)
{
	CStringW strValue;
	::GetDlgItemTextW(m_MyhWnd, ID, strValue.GetBuffer(), 256);
	float fValue = _tstof(strValue);

	CStringW ret;
	ret.Format(L"%.2f", fValue);

	::SetDlgItemTextW(m_MyhWnd, ID, ret);

	return fValue;
}

void CDataController::SetDlgItemFloat(int ID, float fInput)
{
	CString str;
	str.Format(L"%.2f", fInput);
	::SetDlgItemTextW(m_MyhWnd, ID, str);
	str.Empty();
}

int CDataController::GetDlgItemInt(int ID)
{
	CStringW strValue;
	::GetDlgItemTextW(m_MyhWnd, ID, strValue.GetBuffer(), 256);
	int iValue = _tstoi(strValue);

	CString ret;
	ret.Format(L"%d", iValue);

	::SetDlgItemTextW(m_MyhWnd, ID, ret);

	strValue.Empty();
	ret.Empty();

	return iValue;
}

void CDataController::SetDlgItemInt(int iD, int iValue)
{
	CString str;
	str.Format(L"%d", iValue);
	::SetDlgItemText(m_MyhWnd, iD, str);
	str.Empty();
}

CDataController::CDataController()
{
}

CDataController::~CDataController()
{
}

void CDataController::GetRect()
{
	//렉트 초기화
	::GetClientRect(m_MyhWnd, m_Rect);
}

void CDataController::SetMyHWND(HWND hWnd)
{
	m_MyhWnd = hWnd;
}

void CDataController::SetCenter(HWND hCenter)
{
	m_hCenter = hCenter;
}

void CDataController::SendToCenter(UINT message, WPARAM wParam, LPARAM lParam)
{
	::SendMessageW(m_hCenter, message, wParam, lParam);
}

//void CDataController::SendLog(wchar_t* str, eDlgs from)
//{
//	::SendMessageW(m_hCenter, WM_LOG, WPARAM(from), (LPARAM)str);
//}

void CDataController::MoveTo(int x, int y)
{
	::MoveWindow(m_MyhWnd, x, y, m_Rect.Width(), m_Rect.Height(), 1);
}

