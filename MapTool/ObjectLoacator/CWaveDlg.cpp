// CWaveDlg.cpp: 구현 파일
//
#include "pch.h"
#include "Defines.h"
#include "ObjectLoacator.h"
#include "CWaveDlg.h"
#include "afxdialogex.h"
#include "Center.h"
#include "CWaveManager.h"
#include "MapUtil.h"


waveData g_WaveData;
void InitDefaultData();

void InitDefaultData()
{
	g_WaveData.row		= 128;
	g_WaveData.col		= 128;
	g_WaveData.dx		= 1.f;
	g_WaveData.speed	= 4.0f;
	g_WaveData.damping	= 0.2f;

	g_WaveData.pos.x	= 0.f;
	g_WaveData.pos.y	= 0.f;
	g_WaveData.pos.z	= 0.f;

	g_WaveData.rot.x	= 0.f;
	g_WaveData.rot.y	= 0.f;
	g_WaveData.rot.z	= 0.f;

	g_WaveData.scale.x	= 1.f;
	g_WaveData.scale.y	= 1.f;
	g_WaveData.scale.z	= 1.f;
}
// CWaveDlg 대화 상자

IMPLEMENT_DYNAMIC(CWaveDlg, CDialogEx)

CWaveDlg::CWaveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WAVE_DLG, pParent)
{
	InitDefaultData();
}

CWaveDlg::~CWaveDlg()
{
	//Cleanup();
}

CWaveDlg* CWaveDlg::CreateWaveDlg(CWnd* pParent)
{
	//WaveDlg instance 생성
	//메모리 해제 책임은 사용자에게 있다.
	//메모리 할당 실패시 null 리턴
	CWaveDlg* pWaveDlg = new CWaveDlg;
	if (pWaveDlg != nullptr)
	{
		pWaveDlg->Initialize(pParent);
	}
	return pWaveDlg;
}

void CWaveDlg::Initialize(CWnd* pParent)
{
	Create(IDD_WAVE_DLG);
	SetDocking(pParent);
	ShowWindow(SW_SHOW);
}

void CWaveDlg::SetDocking(CWnd* pParent)
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

void CWaveDlg::Cleanup()
{
	//딱히 해줄게 없다. 
	//데이터는 외부에서 관리.
	m_WaveListBox.ResetContent();
}

void CWaveDlg::GetDataFromEditCtrl(waveData* pDest)
{
	if (pDest == NULL) return;
	
	//EditControl 에서 값을 받아오는 로직.
	pDest->row		= GetDlgItemInt(IDC_EDIT_WAVE_ROW);
	pDest->col		= GetDlgItemInt(IDC_EDIT_WAVE_COL);
	pDest->dx		= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_DX);
	pDest->speed	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SPEED);
	pDest->damping	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_DAMPING);

	//pos
	pDest->pos.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSX);
	pDest->pos.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSY);
	pDest->pos.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSZ);

	//rot, quaternion
	DirectX::XMFLOAT3 rot = {  };
	rot.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTX);
	rot.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTY);
	rot.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTZ);
	pDest->SetOiler(rot);

	//prevRot
	memcpy(&pDest->prevRot, &pDest->quaternion, sizeof(DirectX::XMFLOAT4) );

	//scale
	pDest->scale.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEX);
	pDest->scale.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEY);
	pDest->scale.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEZ);

}

void CWaveDlg::SetDataToEditCtrl(waveData* pSrc)
{
	waveData data;
	if (pSrc == nullptr)
	{
		memcpy(&data, &g_WaveData, sizeof(waveData));
	}
	else
	{
		memcpy(&data, pSrc, sizeof(waveData));
	}
	MapUtil::SetDlgItemInt(m_hWnd, IDC_EDIT_WAVE_ROW, data.row);
	MapUtil::SetDlgItemInt(m_hWnd, IDC_EDIT_WAVE_COL, data.col);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_DX, data.dx);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SPEED, data.speed);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_DAMPING, data.damping);

	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSX, data.pos.x);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSY, data.pos.y);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_POSZ, data.pos.z);

	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTX, data.rot.x);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTY, data.rot.y);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_ROTZ, data.rot.z);

	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEX, data.scale.x);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEY, data.scale.y);
	MapUtil::SetDlgItemFloat(m_hWnd, IDC_EDIT_WAVE_SCALEZ, data.scale.z);
}

void CWaveDlg::SetDataToEditCtrlByWave(WAVE* pWave)
{
	if (pWave == nullptr)
		SetDataToEditCtrl(nullptr);
	else
		SetDataToEditCtrl(&pWave->info);
}

void CWaveDlg::UpdateWaveListBox(const std::vector<WAVE*>* pList)
{	
	//이 과정을 하나하나. 외부에서 한다면. 굳이 리스트를 넘길 필요가 없어진다.
	m_WaveListBox.ResetContent();	//리스트박스 비우기

	//새 리스트로 업데이트.
	const std::vector<WAVE*>* pWaveList = pList;
	std::vector<WAVE*>::const_iterator it;
	for (it = pWaveList->begin(); it != pWaveList->end(); it++)
	{
		WAVE* pWave = *(it);
		AddWaveTolistBox(pWave);
	}
}

//리스트박스에 웨이브 추가.
void CWaveDlg::AddWaveTolistBox(WAVE* pWave)
{
	wchar_t* defaultName = L"SOUNDWAVE";
	if (pWave == NULL) return;
	int index = m_WaveListBox.AddString(defaultName);
	m_WaveListBox.SetItemData(index, (DWORD_PTR)pWave);
}

void CWaveDlg::ResetWaveListBox()
{
	m_WaveListBox.ResetContent();	//리스트박스 비우기
}


WAVE* CWaveDlg::GetCurSelectedWaveInListBox()
{
	int index = m_WaveListBox.GetCurSel();
	if (index == -1)	return nullptr;

	return (WAVE*)m_WaveListBox.GetItemData(index);
}

void CWaveDlg::SetWaveListBoxCurSel(WAVE* pFind)
{
	//모든 리스트 돌면서 찾는다.
	if (pFind == nullptr) m_WaveListBox.SetCurSel(-1);
	int count = m_WaveListBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		WAVE* pWave = (WAVE*)m_WaveListBox.GetItemData(i);
		if (pWave == pFind)
		{
			m_WaveListBox.SetCurSel(i);
			return;
		}
	}

	//못찾으면. 선택없음.
	m_WaveListBox.SetCurSel(-1);
}

void CWaveDlg::DeleteWaveInListBox(WAVE* pWave)
{
	if (pWave == NULL) return;

	int count = m_WaveListBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		WAVE* pTemp = (WAVE*)m_WaveListBox.GetItemData(i);
		if (pTemp == pWave)
		{
			m_WaveListBox.DeleteString(i);
			int nextIndex = (i - 1);
			if (nextIndex < 0)
			{
				nextIndex = 0;
			}
				
			m_WaveListBox.SetCurSel(nextIndex);
			g_pCenter->AddDeleteList(pWave);	
			break;
		}
	}
}

void CWaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAVE_LIST, m_WaveListBox); 
	
}


BOOL CWaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDataToEditCtrl(&g_WaveData);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CWaveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_WAVE_CREATE, &CWaveDlg::OnBnClickedWaveCreate)
	ON_BN_CLICKED(IDC_WAVE_EDIT, &CWaveDlg::OnBnClickedWaveEdit)
	ON_BN_CLICKED(IDC_WAVE_DELETE, &CWaveDlg::OnBnClickedWaveDelete)
	ON_LBN_SELCHANGE(IDC_WAVE_LIST, &CWaveDlg::OnLbnSelchangeWaveList)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWaveDlg 메시지 처리기


void CWaveDlg::OnBnClickedWaveCreate()
{
	waveData info;
	GetDataFromEditCtrl(&info);
	WAVE* pWave = NULL;

	//0이면 정상 출력이 안된다.
	if (info.col <= 0) goto lb_return;
	if (info.row <= 0) goto lb_return;
	if (info.dx <= 0)  goto lb_return;

	pWave = g_pCenter->CreateWave(&info);
	if (pWave != NULL)
	{
		AddWaveTolistBox(pWave);
	}

lb_return:
	return;
}


void CWaveDlg::OnBnClickedWaveEdit()
{
	//1. 창으로부터 값을 얻어온다.
	//2. 수정.(삭제 후 재생성)

	waveData data;
	GetDataFromEditCtrl(&data);

	int index = m_WaveListBox.GetCurSel();
	WAVE* pTarget = (WAVE*)m_WaveListBox.GetItemData(index);


	g_pCenter->EditWave(pTarget, &data);
}


void CWaveDlg::OnBnClickedWaveDelete()
{
	//선택된 것 찾기
	//삭제.
	WAVE* pWave = GetCurSelectedWaveInListBox();
	DeleteWaveInListBox(pWave);
}



void CWaveDlg::OnLbnSelchangeWaveList()
{
	int index = m_WaveListBox.GetCurSel();
	if (index == -1)
	{
		//선택된 것이 없다.	-> 디폴트 셋팅/리턴
		int a = 0;
		SetDataToEditCtrl(&g_WaveData);
		return;
	}

	WAVE* pWave = (WAVE*)m_WaveListBox.GetItemData(index);

	if (pWave != NULL)
	{
		g_pCenter;
		SetDataToEditCtrl(&pWave->info);
		g_pCenter->AddSelectedWaveOnlyOne(pWave);			//선택리스트에 추가.
	}

	
}


BOOL CWaveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


void CWaveDlg::OnClose()
{
	Cleanup();
	CDialogEx::OnClose();
}
