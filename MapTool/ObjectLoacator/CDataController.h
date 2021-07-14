#pragma once
#include <afxdialogex.h>


class CDataController
{
protected:
    HWND m_MyhWnd;
    HWND m_hCenter;
    CRect m_Rect;

protected:
    float GetDlgItemFloat(int ID);				//EditControl값 받기 시리즈
    void SetDlgItemFloat(int ID, float fInput);
    int  GetDlgItemInt(int ID);
    void SetDlgItemInt(int iD, int iValue);

public:
    CDataController();
    ~CDataController();
    void GetRect();
    void SetMyHWND(HWND hWnd);
    void SetCenter(HWND hCenter);
    void SendToCenter(UINT message, WPARAM wParam, LPARAM lParam);
    //void SendLog(wchar_t* str, eDlgs from);
    void MoveTo(int x, int y);
};

