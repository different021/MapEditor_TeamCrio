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

	CObjectDlg		m_ObjLocator;		//������Ʈ
	CColliderDlg	m_ColliderDlg;		//�ݶ��̴�
	Viewer			m_Viewer;			//���
	CGrid			m_GridController;	//�׸��� ����
	CHelpDlg		m_HelpDlg;			//����
	CLightDlg		m_LightDlg;		//����Ʈ 
	CWaveDlg		m_WaveDlg;			//����

private:
	CDlgManager();
	~CDlgManager();
	void InitializeDialogs(CWnd* pParent, int iWidth, int iHeight);

public:
	void Release();								
};

