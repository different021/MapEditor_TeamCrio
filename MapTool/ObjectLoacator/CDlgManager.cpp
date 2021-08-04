#include "pch.h"
#include "CDlgManager.h"

CDlgManager::~CDlgManager()
{
}

void CDlgManager::InitializeDialogs(CWnd* pParent, int iWidth, int iHeight)
{
	CWnd* pCwndParent = pParent;
	m_Viewer.Initialize(pCwndParent, iWidth,  iHeight);
	
	m_ObjLocator.Initialize(pCwndParent);
	m_ColliderDlg.Initialize(pCwndParent);
	m_GridController.Initialize(pCwndParent);
	m_HelpDlg.Initialize(pCwndParent);
	m_LightDlg.Initialize(pCwndParent);
	m_WaveDlg.Initialize(pCwndParent);
}

void CDlgManager::Release()
{
}
