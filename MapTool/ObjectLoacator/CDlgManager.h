#pragma once
#include "CObjectDlg.h"
#include "ViewDlg.h"
#include "CGrid.h"
#include "CCmd.h"
#include "CColliderDlg.h"
#include "CHelpDlg.h"
#include "CLIGHTDLG.h"
#include "CWaveDlg.h"

class Center;

class CDlgManager
{
private:
	HWND	g_hCenter;
	Center* g_pCenter = NULL;

	CObjectDlg		m_ObjLocator;		//오브젝트
	CColliderDlg	m_ColliderDlg;		//콜라이더
	Viewer			m_Viewer;			//뷰어
	CGrid			m_GridController;	//그리드 조절
	CHelpDlg		m_HelpDlg;			//도움말
	CLightDlg		m_LightDlg;		//라이트 
	CWaveDlg		m_WaveDlg;			//물결

private:
	CDlgManager();
	~CDlgManager();
	void InitializeDialogs(CWnd* pParent, int iWidth, int iHeight);

public:
	void Release();								
};

