// E:\svn_fin\5_Project\MapTool\ObjectLoacator\CCmd.cpp: 구현 파일
//

#include "pch.h"
#include "ObjectLoacator.h"
#include "CCmd.h"
#include "afxdialogex.h"
#include "WMDefine.h"
#include "Center.h"

// CCmd 대화 상자

IMPLEMENT_DYNAMIC(CCmd, CDialogEx)

CCmd::CCmd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMD, pParent)
{

}

CCmd::~CCmd()
{
}

void CCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_LINE, m_CmdInputLine);
}


BEGIN_MESSAGE_MAP(CCmd, CDialogEx)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CCmd 메시지 처리기

void CCmd::Move(int x, int y)
{
	CRect rc;
	this->GetWindowRect(rc);

	this->MoveWindow(x, y, rc.Width(), rc.Height(), 1);
}

void CCmd::MoveRigthBottom(int mainWidth, int mainHeight)
{
	CRect rc;
	this->GetWindowRect(rc);

	this->MoveWindow(mainWidth - rc.Width() , mainHeight - rc.Height(), rc.Width(), rc.Height(), 1);
}

void CCmd::WriteLog(WCHAR* pStr)
{
	wsprintfW(m_LogBuffer, pStr);
}

void CCmd::WriteLog(int i)
{
	wsprintfW(m_LogBuffer, L"%d", i);
}

template<typename T, typename ...Types>
void CCmd::WriteLog(T arg, Types ...args)
{
	std::basic_string<WCHAR> str = L"test";
	std::basic_string<wchar_t> str1 = L"TEST2";
	std::string output;

	output.append(3, '*');
}

void CCmd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else if (nID == SC_CLOSE)
	{
		//ShowWindow(SW_HIDE);
		::SendMessageW(g_hCenter, WM_HIDE_WINDOW, WPARAM(this), NULL);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}

}


BOOL CCmd::PreTranslateMessage(MSG* pMsg)
{
	UINT msg = pMsg->message;
	switch (msg)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_F3)
			::SendMessageW(g_hCenter, WM_SAVE_MAP, pMsg->wParam, pMsg->lParam);
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
