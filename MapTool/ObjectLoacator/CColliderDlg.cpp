// E:\svn_fin\5_Project\MapTool\ObjectLoacator\CColliderControllerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ObjectLoacator.h"
#include "CColliderDlg.h"
#include "afxdialogex.h"
#include "WMDefine.h"
#include "../src/Defines.h"
#include "../src/utility/HGUtility.h"
#include "Center.h"
#include "MapUtil.h"
#include "CColliderManager.h"

Vector3 Default_Collider_Size = { 1.f, 1.f, 1.f };
//Vector3 Default_Collider_Size = { 0.5f, 0.5f, 0.5f };

// CColliderControllerDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderDlg, CDialogEx)

void CColliderDlg::MoveFocusNext()
{
	//edit control 포커스 이동
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus++;
	if ((m_curFocus > IDC_EDIT_COLLIDER_SCALEZ) || (m_curFocus < IDC_EDIT_COLLIDER_POSX))	m_curFocus = IDC_EDIT_COLLIDER_POSX;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

void CColliderDlg::MoveFocusBack()
{
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus--;
	if ((m_curFocus > IDC_EDIT_COLLIDER_SCALEZ) || (m_curFocus < IDC_EDIT_COLLIDER_POSX))	m_curFocus = IDC_EDIT_COLLIDER_SCALEZ;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}


CColliderDlg::CColliderDlg(CWnd* pParent /*=nullptr*/)
	: CEditorDlg()
{
	m_bIsShiftDown = false;
}

CColliderDlg::~CColliderDlg()
{
}

void CColliderDlg::AddColliderToListBox(collider* pCollider)
{
	if (pCollider == NULL) return;
	int idx = m_ColliderListBox.AddString(pCollider->name);
	m_ColliderListBox.SetItemDataPtr(idx, (void*)pCollider);
	m_ColliderListBox.SetCurSel(idx);
}

void CColliderDlg::SetEditBoxByCollider(collider* pCollider)
{
	collider* pCol = pCollider;
	CString temp;

	if (pCollider == NULL) return;

	eCOLLIDER_TYPE type = pCol->type;
	int index = GetComboIndexByColliderType(type);
	m_ColliderTypeBox.SetCurSel(index);

	//index
	temp.Format(L"%d", pCol->index);
	SetDlgItemText(IDC_EDIT_COLLIDER_INDEX, temp);
	temp.Empty();

	//Pos
	temp.Format(L"%0.2f", pCol->pos.x);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSX, temp);
	temp.Empty();

	temp.Format(L"%0.2f", pCol->pos.y);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSY, temp);
	temp.Empty();

	temp.Format(L"%0.2f", pCol->pos.z);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSZ, temp);
	temp.Empty();

	//Rot
	DirectX::XMFLOAT3 oiler = HGUtility::QuaternionToOiler(pCol->quaternion);
	pCol->oiler.x = oiler.x;
	pCol->oiler.y = oiler.y;
	pCol->oiler.z = oiler.z;

	temp.Format(L"%0.2f", oiler.x);
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTX, temp);
	temp.Empty();

	temp.Format(L"%0.2f", oiler.y);
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTY, temp);
	temp.Empty();

	temp.Format(L"%0.2f", oiler.z);
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTZ, temp);
	temp.Empty();

	//scale
	temp.Format(L"%0.2f", pCol->scale.x);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEX, temp);
	temp.Empty();

	temp.Format(L"%0.2f", pCol->scale.y);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEY, temp);
	temp.Empty();

	temp.Format(L"%0.2f", pCol->scale.z);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEZ, temp);
	temp.Empty();

	if (pCol->isVisible == TRUE)
	{
		//((CMyButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(true);
	}
	else
	{
		//((CMyButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(false);
	}


}

void CColliderDlg::SetEditBoxZero()
{
	CString temp;
	int index = GetComboIndexByColliderType(eCOLLIDER_TYPE::eCOLLIDER_NORMAL);
	m_ColliderTypeBox.SetCurSel(index);

	//index
	temp.Format(L"%d", -1);
	SetDlgItemText(IDC_EDIT_COLLIDER_INDEX, temp);

	//Pos
	temp.Format(L"%0.2f", 0.f);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSX, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSY, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_POSZ, temp);

	//Rot
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTX, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTY, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_ROTZ, temp);
	temp.Empty();

	//scale
	temp.Format(L"%0.2f", 100.f);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEX, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEY, temp);
	SetDlgItemText(IDC_EDIT_COLLIDER_SCALEZ, temp);
	temp.Empty();

	//((CMyButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_CHECK_VISIBLE))->SetCheck(false);
}

void CColliderDlg::SetColliderByEditControl(collider* pDest)
{
	if (pDest == NULL)
	{
		SetEditBoxZero();
		return;
	}
	collider* pCollider = pDest;
	eCOLLIDER_TYPE type = GetColliderTypeByComboIndex();

	pCollider->type = type;
	pCollider->pos.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_POSX);
	pCollider->pos.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_POSY);
	pCollider->pos.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_POSZ);

	XMFLOAT3* pOiler = &pCollider->oiler;

	XMFLOAT3 oiler;
	oiler.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_ROTX);
	oiler.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_ROTY);
	oiler.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_ROTZ);
	SetColliderRot(pDest, oiler);


	pCollider->prevRot.x = pCollider->quaternion.x;
	pCollider->prevRot.y = pCollider->quaternion.y;
	pCollider->prevRot.z = pCollider->quaternion.z;
	pCollider->prevRot.w = pCollider->quaternion.w;


	pCollider->scale.x = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_SCALEX);
	pCollider->scale.y = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_SCALEY);
	pCollider->scale.z = MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_COLLIDER_SCALEZ);

	pCollider->MakeName();

	float* pFloat = (float*)(&pCollider->scale);
	for (int i = 0; i < 3; i++)
	{
		if (*(pFloat + i) <= 0.f)
		{
			*(pFloat + i) = 1.f;
		}
	}

	//CMyButton* pCheck = (CMyButton*)GetDlgItem(IDC_CHECK_VISIBLE);
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_VISIBLE);
	int check = pCheck->GetCheck();
	if (check == 1)
	{
		pCollider->isVisible = TRUE;
	}
	else
	{
		pCollider->isVisible = FALSE;
	}

	pCollider->size = Default_Collider_Size;

}

void CColliderDlg::CreateCollider(collider* pSrc)
{

	collider* pCollider = NULL;
	if (pSrc == NULL)
	{
		//메모리 할당.
		//pCollider = new collider;
		pCollider = g_pCenter->CreateCollider();

		pCollider->isVisible = true;
		SetColliderByEditControl(pCollider);
	}
	else
	{
		pCollider = pSrc;
	}

	WPARAM wp = (WPARAM)pCollider;
	::SendMessageW(g_hCenter, WM_CREATE_COLLIDER, wp, NULL);
	AddColliderToListBox(pCollider);
}

int CColliderDlg::GetColliderListBoxIndex(collider* pCollider)
{
	if (!pCollider)
	{
		m_ColliderListBox.SetCurSel(-1);
		return -1;
	}
	collider* pTempCol = NULL;
	int size = m_ColliderListBox.GetCount();
	for (int i = 0; i < size; i++)
	{
		pTempCol = (collider*)m_ColliderListBox.GetItemData(i);
		if (pTempCol == pCollider)
		{
			return i;
		}
	}

	return -1;	//결국 못찾았을 경우.
}

//에딧 컨트롤에 있는 숫자로 인덱스 생성. -> 주의 같은 숫자로 될수 있다. -> 보류
void CColliderDlg::SetColliderIndexByEditControl(collider* pDest)
{
}

void CColliderDlg::SetColliderRot(collider* pDest, DirectX::XMFLOAT3& oiler)
{
	pDest->oiler.x = oiler.x;
	pDest->oiler.y = oiler.y;
	pDest->oiler.z = oiler.z;

	Quaternion quatRot = Quaternion::CreateFromYawPitchRoll(oiler.y * XM_PI / 180, oiler.x * XM_PI / 180, oiler.z * XM_PI / 180);
	pDest->quaternion.x = quatRot.x;
	pDest->quaternion.y = quatRot.y;
	pDest->quaternion.z = quatRot.z;
	pDest->quaternion.w = quatRot.w;
}

void CColliderDlg::SelectCollider(collider* pCollider)
{
	if (pCollider)
	{
		SetEditBoxByCollider(pCollider);
		//g_pCenter->UpdateColliderList();
	}
	else
		SetEditBoxZero();

	int index = GetColliderListBoxIndex(pCollider);
	m_ColliderListBox.SetCurSel(index);
}

void CColliderDlg::UpdateColliderList(std::vector<COLLIDER>* pList)
{
	m_ColliderListBox.ResetContent();
	if (pList == nullptr) return;
	//int heighestIndex = 0;
	int size = pList->size();
	for (int i = 0; i < size; i++)
	{
		collider* pCol = pList->at(i).first;
		int idx = m_ColliderListBox.InsertString(i, pCol->name);
		m_ColliderListBox.SetItemDataPtr(idx, (void*)(pCol));
		int index = pCol->index;
		//heighestIndex = (heighestIndex > pCol->index) ? heighestIndex : pCol->index;
	}
	//m_ColliderHighestIndex = heighestIndex + 1;

	return;
}

eCOLLIDER_TYPE CColliderDlg::GetColliderTypeByComboIndex()
{
	int index = m_ColliderTypeBox.GetCurSel();
	return (eCOLLIDER_TYPE)m_ColliderTypeBox.GetItemData(index);
}

int CColliderDlg::GetComboIndexByColliderType(eCOLLIDER_TYPE type)
{
	int index = -1;
	int count = m_ColliderTypeBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		eCOLLIDER_TYPE val = (eCOLLIDER_TYPE)m_ColliderTypeBox.GetItemData(i);
		if (val == type)
		{
			index = i;
			break;
		}
	}
	return index;
}


// CColliderControllerDlg 메시지 처리기
BEGIN_MESSAGE_MAP(CColliderDlg, CDialogEx)
	ON_BN_CLICKED(ID_CREATE_COLLIDER, &CColliderDlg::OnBnClickedCreateCollider)
	ON_BN_CLICKED(ID_DELETE_COLLIDER, &CColliderDlg::OnBnClickedDeleteCollider)
	ON_LBN_SELCHANGE(IDC_COLLIDER_LISTBOX, &CColliderDlg::OnLbnSelchangeColliderListbox)
	ON_BN_CLICKED(ID_EDIT_COLLIDER, &CColliderDlg::OnBnClickedEditCollider)
	ON_MESSAGE(WM_CREATE_COLLIDER, &CColliderDlg::OnCreateCollider)
	ON_MESSAGE(WM_CLEAR_COLLIDER_LIST, &CColliderDlg::OnClearColliderList)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CColliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLLIDER_LISTBOX, m_ColliderListBox);
	DDX_Control(pDX, IDC_COMBO_COL_TYPE, m_ColliderTypeBox);
}

afx_msg LRESULT CColliderDlg::OnCreateCollider(WPARAM wParam, LPARAM lParam)
{
	collider* pCol = (collider*)wParam;
	CreateCollider(pCol);

	return 0;
}


afx_msg LRESULT CColliderDlg::OnClearColliderList(WPARAM wParam, LPARAM lParam)
{
	m_ColliderListBox.ResetContent();
	return 0;
}


void CColliderDlg::OnBnClickedCreateCollider()
{
	//InsertColliderList -> 같은 로직이 두번 쓰인다. 
	//버튼 클릭
	//맵 로드.
	CreateCollider();
	g_pCenter->UpdateRegenColliderListInObjLocator();
}


void CColliderDlg::OnBnClickedDeleteCollider()
{
	int index = m_ColliderListBox.GetCurSel();
	if (index == LB_ERR) return;

	collider* pCollider = (collider*)m_ColliderListBox.GetItemDataPtr(index);
	
	g_pCenter->DeleteCollider(pCollider);
	
	m_ColliderListBox.DeleteString(index);
	m_ColliderListBox.SetCurSel(index);

	g_pCenter->UpdateRegenColliderListInObjLocator();
}


void CColliderDlg::OnBnClickedEditCollider()
{
	int index = m_ColliderListBox.GetCurSel();
	int count = m_ColliderListBox.GetCount();
	if (index != LB_ERR && count > 0)
	{
		collider* pCol = (collider*)m_ColliderListBox.GetItemDataPtr(index);
		SetColliderByEditControl(pCol);
		WPARAM wp = (WPARAM)pCol;

		::SendMessageW(g_hCenter, WM_EDIT_COLLIDER, wp, NULL);
		g_pCenter->UpdateRegenColliderListInObjLocator();
	}


}


void CColliderDlg::OnLbnSelchangeColliderListbox()
{
	int index = m_ColliderListBox.GetCurSel();

	if (index >= 0)
	{
		collider* pCollider = (collider*)m_ColliderListBox.GetItemDataPtr(index);
		SetEditBoxByCollider(pCollider);
		g_pCenter->SelectCollider(pCollider);
		g_pCenter->SetGizmo(pCollider);
	}

}


BOOL CColliderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Init ColliderTypeBox
	int StartIndex = m_ColliderTypeBox.InsertString(0, L"START POINT");
	m_ColliderTypeBox.SetItemData(StartIndex, eCOLLIDER_TYPE::eCOLLIDER_STARTPOINT);

	int endIndex = m_ColliderTypeBox.InsertString(1, L"END POINT");
	m_ColliderTypeBox.SetItemData(endIndex, eCOLLIDER_TYPE::eCOLLIDER_ENDPOINT);

	int RegenIndex = m_ColliderTypeBox.InsertString(2, L"REGENERATION POINT");
	m_ColliderTypeBox.SetItemData(RegenIndex, eCOLLIDER_TYPE::eCOLLIDER_REGENERATIONPOINT);

	int nColIndex = m_ColliderTypeBox.InsertString(3, L"NORMAL COLLIDER");
	m_ColliderTypeBox.SetItemData(nColIndex, eCOLLIDER_TYPE::eCOLLIDER_NORMAL);
	m_ColliderTypeBox.SetCurSel(nColIndex);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CColliderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UINT msg = pMsg->message;
	WPARAM wp = pMsg->wParam;
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wp == VK_F3)
		{
			::SendMessageW(g_hCenter, WM_SAVE_MAP, pMsg->wParam, pMsg->lParam);
			break;
		}

		if (wp == VK_TAB)
		{
			if (!m_bIsShiftDown)
				MoveFocusNext();
			else
				MoveFocusBack();
			break;
		}

	}

	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


void CColliderDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		m_bIsShiftDown = true;
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CColliderDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		m_bIsShiftDown = false;
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CColliderDlg::OnClose()
{

	CDialogEx::OnClose();
}
