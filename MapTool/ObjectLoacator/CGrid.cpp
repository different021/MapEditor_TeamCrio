// CGrid.cpp: 구현 파일
//

#include "pch.h"
#include "Center.h"
#include "ObjectLoacator.h"
#include "CGrid.h"
#include "afxdialogex.h"
#include "ViewDlg.h"
#include "WMDefine.h"

IMPLEMENT_DYNAMIC(CGrid, CDialogEx)

CGrid::CGrid(CWnd* pParent /*=nullptr*/)
	: CEditorDlg()
{
	m_bIsShiftDown = false;
}

CGrid::~CGrid()
{
}

float CGrid::GetDlgItemFloat(int ID)
{
	CString strValue;
	GetDlgItemText(ID, strValue);
	float fValue = _tstof(strValue);

	CString ret;
	ret.Format(L"%.2f", fValue);

	SetDlgItemText(ID, ret);

	return fValue;
}

int CGrid::GetDlgItemInt(int ID)
{
	CString strValue;
	GetDlgItemText(ID, strValue);
	int iValue = _tstoi(strValue);

	CString ret;
	ret.Format(L"%d", iValue);

	SetDlgItemText(ID, ret);

	strValue.Empty();
	ret.Empty();

	return iValue;
}

void CGrid::SetDlgItemFloat(int ID, float fInput)
{
	CString str;
	str.Format(L"%.2f", fInput);
	SetDlgItemText(ID, str);
	str.Empty();
}

void CGrid::SetDlgItemInt(int iD, int iValue)
{
	CString str;
	str.Format(L"%d", iValue);
	SetDlgItemText(iD, str);
	str.Empty();
}

void CGrid::UpdateEditBox()
{
	SetDlgItemInt(IDC_EDIT_MAP_WIDTH, m_iWidth);
	SetDlgItemInt(IDC_EDIT_MAP_HEIGHT, m_iHeight);
	SetDlgItemFloat(IDC_EDIT_MAP_OFFSET, m_fOffset);
	
	GetDlgItem(IDC_EDIT_MAP_WIDTH)->UpdateWindow();
	GetDlgItem(IDC_EDIT_MAP_HEIGHT)->UpdateWindow();
	GetDlgItem(IDC_EDIT_MAP_OFFSET)->UpdateWindow();
}

int CGrid::GetMapWidth()
{
	return m_iWidth;
}

int CGrid::GetMapHeight()
{
	return m_iHeight;
}

float CGrid::GetMapOffset()
{
	return m_fOffset;
}

void CGrid::SetGrid(int width, int height, float offset)
{
	m_iWidth  = width;
	m_iHeight = height;
	m_fOffset = offset;
	AnnounceChangeGrid();
}

void CGrid::AnnounceChangeGrid()
{
	g_pCenter->ChangeGrid(m_iWidth, m_iHeight, m_fOffset);
}

void CGrid::EditGrid(int width, int height, float offset)
{
	if ((width % 2) == 0)
	{
		m_iWidth = width;
	}
	else
	{
		m_iWidth = width + 1;
	}

	if ((height % 2) == 0)
	{
		m_iHeight = height;
	}
	else
	{
		m_iHeight = height + 1;
	}

	if (offset <= 0.f)
	{
		offset = 10.f;
	}

	m_fOffset = offset;
}

void CGrid::MoveFocusNext()
{
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus++;
	if ((m_curFocus > IDC_EDIT_MAP_OFFSET) || (m_curFocus < IDC_EDIT_MAP_WIDTH) )	m_curFocus = IDC_EDIT_MAP_WIDTH;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);

	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

void CGrid::MoveFocusBack()
{
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus--;
	if ((m_curFocus > IDC_EDIT_MAP_OFFSET) || (m_curFocus < IDC_EDIT_MAP_WIDTH) ) 	m_curFocus = IDC_EDIT_MAP_OFFSET;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

void CGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGrid, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGrid::OnBnClickedOk)
	ON_WM_SYSCOMMAND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CGrid 메시지 처리기


void CGrid::OnBnClickedOk()
{
	int width	 = GetDlgItemInt(IDC_EDIT_MAP_WIDTH);
	int height	 = GetDlgItemInt(IDC_EDIT_MAP_HEIGHT);
	float offset = GetDlgItemFloat(IDC_EDIT_MAP_OFFSET);
	
	EditGrid(width, height, offset);
	AnnounceChangeGrid();
	UpdateEditBox();
}


void CGrid::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else if (nID == SC_CLOSE)
	{
		//ShowWindow(SW_HIDE);
		::SendMessageW(g_hCenter, WM_HIDE_WINDOW, WPARAM(this), eDlgs::eGridCon);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL CGrid::PreTranslateMessage(MSG* pMsg)
{
	UINT msg = pMsg->message;
	WPARAM wp = pMsg->wParam;
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wp == VK_F3)
		{
			::SendMessageW(g_hCenter, WM_SAVE_MAP, pMsg->wParam, pMsg->lParam);
			break;
		}
			
		if (wp == VK_TAB)
		{
			if (!m_bIsShiftDown)
				MoveFocusNext();
			else
				MoveFocusBack();
			break;
		}
		
	}
	
	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CGrid::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//헤더 그리드 기본값
	m_iWidth  = iDEFAULTGRID_W;
	m_iHeight = iDEFAULTGRID_H;
	m_fOffset = fDEFAULTOFFSET;
	AnnounceChangeGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_SHIFT)
		m_bIsShiftDown = true;
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		m_bIsShiftDown = false;
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}
