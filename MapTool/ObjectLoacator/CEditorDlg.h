#pragma once
#include <afxdialogex.h>
class CEditorDlg :
    public CDialogEx
{
public:
	CEditorDlg();
	virtual void Initialize(CWnd* pParent, UINT id);
	virtual void SetDocking(CWnd* pParent);
	virtual void Move(int x, int y);
};

