#include "pch.h"
#include "resource.h"
#include "CEditorDlg.h"

//CEditorDlg* CEditorDlg::CreateDlgInstance(CWnd* pParent, UINT id)
//{
//	//�ν��Ͻ��� �����ؼ� �����Ѵ�.
//	//�ν��Ͻ� �����ǹ��� ����ڿ��� �ִ�.
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
