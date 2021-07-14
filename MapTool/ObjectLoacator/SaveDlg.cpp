#include "pch.h"
#include "SaveDlg.h"

SaveDlg::SaveDlg(BOOL bIn) : CFileDialog(bIn)
{

}

SaveDlg::~SaveDlg()
{
}
BEGIN_MESSAGE_MAP(SaveDlg, CFileDialog)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


void SaveDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	//CFileDialog::OnWindowPosChanged(lpwndpos);
}
