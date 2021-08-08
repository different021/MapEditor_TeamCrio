// CLIGHTDLG.cpp: 구현 파일
//

#include "pch.h"
#include "ObjectLoacator.h"
#include "CLightDlg.h"
#include "afxdialogex.h"

#include "../src/utility/HGUtility.h"
#include "MapUtil.h"
#include "CLightManager.h"
// CLIGHTDLG 대화 상자

IMPLEMENT_DYNAMIC(CLightDlg, CDialogEx)

void CLightDlg::MoveFocusNext()
{
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus++;
	if ((m_curFocus < IDC_EDIT_LIGHT_NAME) || (m_curFocus > IDC_COMBO_LIGHT_TYPE))	m_curFocus = IDC_EDIT_LIGHT_NAME;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

void CLightDlg::MoveFocusBack()
{
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus--;
	if ((m_curFocus < IDC_EDIT_LIGHT_NAME) || (m_curFocus > IDC_COMBO_LIGHT_TYPE))	m_curFocus = IDC_COMBO_LIGHT_TYPE;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

CLightDlg::CLightDlg(CWnd* pParent /*=nullptr*/)
	: CEditorDlg()
{
	m_bShiftKeyDown = false;
}

CLightDlg::~CLightDlg()
{
	Cleanup();
}

void CLightDlg::Cleanup()
{
	if (m_pLightManager)
	{
		m_pLightManager->Release();
		m_pLightManager = NULL;
	}
}

void CLightDlg::SetDlgFloat(UINT uId, float data)
{
	CString ret;
	ret.Format(L"%0.2f", data);
	SetDlgItemText(uId, ret);
	GetDlgItem(uId)->UpdateWindow();
	ret.Empty();
}

void CLightDlg::SetEditBox(Light* pLight)
{
	lightData data;
	//memset(&data, 0, sizeof(data));
	memset(data.name, 0, sizeof(wchar_t) * MAX_LENGTH);
	data.type		  = eDirect;
	data.direction[0] = 0.f;
	data.direction[1] = -1.f;
	data.direction[2] = 0.f;
	data.strength[0] = 1.f;
	data.strength[1] = 1.f;
	data.strength[2] = 1.f;
	data.spotPower	 = 4.f;
	data.falloffStart = 1.f;
	data.falloffEnd  = 100.f;
	data.position[0] = 0.f;
	data.position[1] = 0.f;
	data.position[2] = 0.f;

	if (pLight != NULL)
	{
		memcpy(&data, &pLight->m_LightData, sizeof(lightData));
		//data = pLight->m_LightData;
	}

	CComboBox* pComboType = (CComboBox*)GetDlgItem(IDC_COMBO_LIGHT_TYPE);

	//WCHAR szName[256];
	::SetDlgItemTextW(m_hWnd, IDC_EDIT_LIGHT_NAME, data.name);

	//Light

	//m_pLightType->SetCurSel();
	UpdateTypeByLight(&data);

	SetDlgFloat(IDC_EDIT_STRENGTH_X, data.strength[0]);
	SetDlgFloat(IDC_EDIT_STRENGTH_Y, data.strength[1]);
	SetDlgFloat(IDC_EDIT_STRENGTH_Z, data.strength[2]);

	SetDlgFloat(IDC_EDIT_FALLOFFSTART, data.falloffStart);
	SetDlgFloat(IDC_EDIT_FALLOFFEND, data.falloffEnd);

	SetDlgFloat(IDC_EDIT_DIRECTION_X, data.direction[0]);
	SetDlgFloat(IDC_EDIT_DIRECTION_Y, data.direction[1]);
	SetDlgFloat(IDC_EDIT_DIRECTION_Z, data.direction[2]);

	SetDlgFloat(IDC_EDIT_POSITION_X, data.position[0]);
	SetDlgFloat(IDC_EDIT_POSITION_Y, data.position[1]);
	SetDlgFloat(IDC_EDIT_POSITION_Z, data.position[2]);

	SetDlgFloat(IDC_EDIT_SPOTPOWER, data.spotPower);
}

void CLightDlg::GetDataFromControl(lightData& pDest)
{
	WCHAR szName[256];
	::GetDlgItemTextW(m_hWnd, IDC_EDIT_LIGHT_NAME, szName, 256);
	wsprintfW(pDest.name, szName);

	pDest.type = GetSelectedType();

	pDest.strength[0] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_STRENGTH_X);
	pDest.strength[1] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_STRENGTH_Y);
	pDest.strength[2] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_STRENGTH_Z);

	pDest.falloffStart = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_FALLOFFSTART);
	pDest.falloffEnd = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_FALLOFFEND);

	pDest.direction[0] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_DIRECTION_X);
	pDest.direction[1] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_DIRECTION_Y);
	pDest.direction[2] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_DIRECTION_Z);

	pDest.position[0] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_POSITION_X);
	pDest.position[1] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_POSITION_Y);
	pDest.position[2] = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_POSITION_Z);

	pDest.spotPower = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_SPOTPOWER);
	
}

void CLightDlg::GetDataFromEditBox(Light* pLight)
{
	lightData data;
	GetDataFromControl(data);

	pLight->CopyData(data);
	pLight->UpdateLightData();
}

eLightType CLightDlg::GetSelectedType()
{
	int idx = m_pLightType->GetCurSel();
	eLightType type = (eLightType)m_pLightType->GetItemData(idx);

	return type;
}

void CLightDlg::SetEditBoxActive(eLightType type)
{
	switch (type)
	{
	case eDirect:
		DirectionInputOFF();
		break;
	case ePoint:
		PointInputOFF();
		break;
	case eSpot:
		SpotInputOFF();
		break;
	case eLightTypeMax:
		break;
	default:
		OutputDebugString(L"[Light Type]Invaild Data\n");
		break;
	}
}

void CLightDlg::AllOn()
{
	GetDlgItem(IDC_EDIT_FALLOFFSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DIRECTION_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DIRECTION_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_DIRECTION_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FALLOFFEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_POSITION_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_POSITION_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_POSITION_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SPOTPOWER)->EnableWindow(TRUE);
}

void CLightDlg::DirectionInputOFF()
{
	AllOn();
	GetDlgItem(IDC_EDIT_FALLOFFSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FALLOFFEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_POSITION_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_POSITION_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_POSITION_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SPOTPOWER)->EnableWindow(FALSE);
}

void CLightDlg::PointInputOFF()
{
	AllOn();
	GetDlgItem(IDC_EDIT_DIRECTION_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DIRECTION_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DIRECTION_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SPOTPOWER)->EnableWindow(FALSE);
}

void CLightDlg::SpotInputOFF()
{
	AllOn();
}

int CLightDlg::AddLightToListBox(Light* pLight)
{
	WCHAR buffer[256];
	memset(buffer, 0, sizeof(buffer));
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	
	wsprintfW(buffer, pLight->m_LightData.name);

	eLightType type = pLight->m_LightData.type;
	switch (type)
	{
		case eLightType::eDirect:
		{
			wcscat_s(buffer, L"(Direction)");
			break;
		}

		case eLightType::ePoint:
		{
			wcscat_s(buffer, L"(Point)");
			break;
		}

		case eLightType::eSpot:
		{
			wcscat_s(buffer, L"(Spot)");
			break;
		}
	}
	
	int idx = pList->AddString(buffer);
	pList->SetItemDataPtr(idx, (void*)pLight);
	pList->SetCurSel(idx);

	return idx;
}

int CLightDlg::UpdateEditBoxByLight(Light* pLight)
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	int iResult = 0;

	if (pLight == NULL)
	{
		pListBox->SetCurSel(iResult);
		return iResult;
	}

	int cnt = pListBox->GetCount();

	for (int i = 0; i < cnt; i++)
	{
		Light* pTemp = (Light*)pListBox->GetItemData(i);
		if (pTemp == pLight)
		{
			pListBox->SetCurSel(i);
			SetEditBox(pTemp);
			iResult = i;
			break;
		}
	}

	return iResult;
}

void CLightDlg::UpdateLightList()
{
	ClearLightList();
	std::vector<Light*>* pList = m_pLightManager->GetList();
	std::vector<Light*>::iterator it;
	for (it = pList->begin(); it != pList->end(); it++)
	{
		Light* pTemp = *it;
		AddLightToListBox(pTemp);
	}

}

void CLightDlg::ClearLightList()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	pList->ResetContent();
}

void CLightDlg::UpdateSelectedLight()
{
	Light* pLight = m_pLightManager->GetLastSelected();
	SetEditBox(pLight);
}

void CLightDlg::UpdateTypeByLight(lightData* pData)
{
	if (pData == NULL) m_pLightType->SetCurSel(0);

	int count = m_pLightType->GetCount();
	for (int i = 0; i < count; i++)
	{
		eLightType type = (eLightType)m_pLightType->GetItemData(i);
		if (type == pData->type)
		{
			m_pLightType->SetCurSel(i);
			break;
		}
	}
}


void CLightDlg::DeleteLightInListBox(std::vector<Light*>* pList)
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	
	std::vector<Light*>::iterator it;
	for (it = pList->begin(); it != pList->end(); it++)
	{
		Light* pTarget = *it;

		int count = pListBox->GetCount();
		for (int i = 0; i < count; i++)
		{
			Light* pTemp = (Light*)pListBox->GetItemData(i);
			if (pTemp == pTarget)
			{
				pListBox->DeleteString(i);
				break;
			}
		}

	}

}


void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLightDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT_TYPE, &CLightDlg::OnCbnSelchangeComboLightType)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CLightDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CLightDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CLightDlg::OnBnClickedButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_LIGHT, &CLightDlg::OnLbnSelchangeListLight)
	//ON_WM_KEYDOWN()
	//ON_WM_KEYUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLIGHTDLG 메시지 처리기
 



BOOL CLightDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM wp = pMsg->wParam;
	if (pMsg->message == WM_KEYUP)
	{
		if (wp == VK_SHIFT)
		{
			m_bShiftKeyDown = false;
		}

		if (wp == VK_TAB)
		{
			if (!m_bShiftKeyDown)
				MoveFocusNext();
			else
				MoveFocusBack();
		}
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if (wp == VK_SHIFT)
		{
			m_bShiftKeyDown = true;
		}
	}

	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CLightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pLightManager = CLightManager::GetInstance();

	m_pLightType = (CComboBox*)GetDlgItem(IDC_COMBO_LIGHT_TYPE);
	int idx =m_pLightType->AddString(L"Direction");
	m_pLightType->SetItemData(idx, eDirect);

	idx = m_pLightType->AddString(L"Point");
	m_pLightType->SetItemData(idx, ePoint);

	idx = m_pLightType->AddString(L"Spot");
	m_pLightType->SetItemData(idx, eSpot);

	m_pLightType->SetCurSel(0);
	DirectionInputOFF();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLightDlg::OnCbnSelchangeComboLightType()
{
	
	int idx = m_pLightType->GetCurSel();
	eLightType type = (eLightType)m_pLightType->GetItemData(idx);
	SetEditBoxActive(type);
}

void CLightDlg::OnBnClickedButtonCreate()
{
	lightData lightData;
	memset(&lightData, 0, sizeof(lightData));
	GetDataFromControl(lightData);
	
	Light* pLight = m_pLightManager->CreateLight(lightData);

	AddLightToListBox(pLight);
}


void CLightDlg::OnBnClickedButtonEdit()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	int idx = pListBox->GetCurSel();
	if (idx == LB_ERR) return;
	Light* pLight = (Light*)pListBox->GetItemDataPtr(idx);
	lightData data;

	if (pLight != NULL)
	{
		LightType type = m_pLightManager->GetLightType(pLight->m_LightData.type);
		GetDataFromControl(data);
		LightType newType = m_pLightManager->GetLightType(data.type);

		if (type != data.type)
		{
			m_pLightManager->AddChangeLightTypeList(pLight, newType);
		}

		pLight->CopyData(data);
		//pLight->UpdateLightData(); -> 업데이트에서 한번에 업데이트.
	}

	//에딧 버튼 눌러도 키보드로 위아래 이동 가능 하도록.
}


void CLightDlg::OnBnClickedButtonDelete()
{
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	int idx = pList->GetCurSel();
	if (idx == LB_ERR) return;
	Light* pLight = (Light*)pList->GetItemDataPtr(idx);

	//m_pLightManager->DeleteLight(pLight);
	m_pLightManager->AddDeleteList(pLight);
	pList->DeleteString(idx);

	idx--;
	if (idx < 0)
	{
		idx = 0;
	}
	pList->SetCurSel(idx);
}


void CLightDlg::OnLbnSelchangeListLight()
{
	Light* pLight = NULL;
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_LIGHT);
	int idx = pList->GetCurSel();
	
	if (idx > -1)
	{
		pLight = (Light*)pList->GetItemDataPtr(idx);
		if (pLight == nullptr) return;

		SetEditBox(pLight);
		eLightType type = pLight->m_LightData.type;
		SetEditBoxActive(type);

		//센터로 이동 -> 매니저 제거 가능.
		m_pLightManager->AddSelected_onlyOne(pLight);
	}

	
}


void CLightDlg::OnClose()
{
	int a = 0;
	CDialogEx::OnClose();
}
