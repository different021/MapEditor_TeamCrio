#include "pch.h"
#include "resource.h"
#include "CEditorDlg.h"

//CEditorDlg* CEditorDlg::CreateDlgInstance(CWnd* pParent, UINT id)
//{
//	//인스턴스를 생성해서 리턴한다.
//	//인스턴스 삭제의무는 사용자에게 있다.
//	CEditorDlg* pResult = new CEditorDlg;
//	if (pResult != nullptr)
//		pResult->Initialize(pParent, id);
//
//	return pResult;
//}

CEditorDlg::CEditorDlg()
{
}

void CEditorDlg::Initialize(CWnd* pParent, UINT id)
{
	Create(id);
	//SetDocking(nullptr);
	SetDocking(pParent);
	ShowWindow(SW_SHOW);
}

void CEditorDlg::SetDocking(CWnd* pParent)
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

void CEditorDlg::Move(int x, int y)
{
	CRect rc;
	GetWindowRect(rc);

	MoveWindow(x, y, rc.Width(), rc.Height());
}
