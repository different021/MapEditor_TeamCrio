#pragma once
#include <afxdlgs.h>

class SaveDlg :
    public CFileDialog
{
public:
	SaveDlg(BOOL bIn);
	~SaveDlg();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};

