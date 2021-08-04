// E:\svn_fin\5_Project\MapTool\ObjectLoacator\CHelpDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ObjectLoacator.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 대화 상자

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP_DLG, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

CHelpDlg* CHelpDlg::CreateHelpDlg(CWnd* pParent)
{
	CHelpDlg* pResult = new CHelpDlg;
	pResult->Initialize(pParent);
	return pResult;
}

void CHelpDlg::Initialize(CWnd* pParent)
{
	//m_HelpDlg = new CHelpDlg;
	Create(IDD_HELP_DLG);
	SetDocking(pParent);
	ShowWindow(SW_HIDE);
}

void CHelpDlg::SetDocking(CWnd* pParent)
{
	CWnd* pCWndParent = pParent;
	if (pCWndParent != nullptr)
	{
		ModifyStyle(WS_POPUP, WS_CHILD);
	}
	else
	{
		ModifyStyle(WS_CHILD, WS_POPUP | WS_BORDER | WS_CAPTION);
	}

	SetParent(pCWndParent);
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHelpDlg 메시지 처리기


void CHelpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		this->ShowWindow(SW_HIDE);
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void CHelpDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}


BOOL CHelpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}
