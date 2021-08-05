#include "pch.h"
#include "resource.h"
#include "CDlgManager.h"

CDlgManager::~CDlgManager()
{
}

void CDlgManager::InitializeDialogs(CWnd* pParent, int iWidth, int iHeight)
{
	CWnd* pCwndParent = pParent;
	m_Viewer.Initialize(pCwndParent, IDD_VIEW_DLG ,iWidth, iHeight);
	
	m_ObjectDlg.Initialize(pCwndParent, IDD_OBJECTLOACATOR_DIALOG);
	m_ColliderDlg.Initialize(pCwndParent, IDD_COLLIDER_CONTROLLER);
	m_GridDlg.Initialize(pCwndParent, IDD_GRIDCON);
	m_HelpDlg.Initialize(pCwndParent, IDD_HELP_DLG);
	m_LightDlg.Initialize(pCwndParent, IDD_LIGHT_DLG);
	m_WaveDlg.Initialize(pCwndParent, IDD_WAVE_DLG);
}

void CDlgManager::Release()
{
}
