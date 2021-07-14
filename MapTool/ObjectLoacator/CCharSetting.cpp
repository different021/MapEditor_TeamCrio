// E:\svn_fin\5_Project\MapTool\ObjectLoacator\CCharSetting.cpp: 구현 파일
//
#include <crtdbg.h>

#include "pch.h"
#include "ObjectLoacator.h"
#include "CCharSetting.h"
#include "afxdialogex.h"

#include "ToolDefine.h"
#include "WMDefine.h"
#include "MapUtil.h"

// CCharSetting 대화 상자
IMPLEMENT_DYNAMIC(CCharSetting, CDialogEx)

CCharSetting::CCharSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAR_SETTING, pParent)
{

	//  test = 0;
}

CCharSetting::~CCharSetting()
{
}

void CCharSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_CHAR, m_TreeChar);
	DDX_Control(pDX, IDC_LIST_MODEL, m_ModelListBox);
	DDX_Control(pDX, IDC_LIST_MATERIAL, m_MaterialListBox);
}

void CCharSetting::RequestList()
{
	Request_ModelList();
	Request_MatList();
}

void CCharSetting::GetCharInfo(CHARACTER* pChar)
{
	//GetModelInfo();
	GetText(IDC_EDIT_CHAR_MODEL, pChar->modelName);
	GetText(IDC_EDIT_CHAR_MATERIAL, pChar->matName);

	SetDefaultName(pChar);
	/*
	GetText(IDC_EDIT_CHAR_NAME, pChar->name);
	int isSame = StrCmpW(pChar->name, L"");
	if (isSame == 0)
	{
		SetDefaultName(pChar);
		//wsprintfW(pChar->name, pChar->modelName);
	}
	*/

	GetInt(IDC_EDIT_CHAR_MODEL_INDEX, pChar->model_index);
	GetInt(IDC_EDIT_CHAR_MAT_INDEX, pChar->mat_index);
	//GetInt(IDC_EDIT_CHAR_INDEX, pChar->index);

	pChar->STRENGTH	= GetDlgInt(IDC_EDIT_CHAR_GONG);
	pChar->AGILITY	= GetDlgInt(IDC_EDIT_CHAR_MIN);
	pChar->INTELLIGENCE = GetDlgInt(IDC_EDIT_CHAR_JI);
	pChar->LUCK		= GetDlgInt(IDC_EDIT_CHAR_LUCK);

	pChar->HP		= GetDlgInt(IDC_EDIT_HP);
	pChar->MP		= GetDlgInt(IDC_EDIT_MP);
	pChar->DEF		= GetDlgInt(IDC_EDIT_DEF);
	pChar->SPEED	= GetDlgInt(IDC_EDIT_SPEED);
	pChar->ATTACK	= GetDlgInt(IDC_EDIT_ATTACK);
	pChar->ATK_SPD	= GetDlgInt(IDC_EDIT_ATK_SPD);
	pChar->MAGIC	= GetDlgInt(IDC_EDIT_MAGIC);

	/*
	//다른 클래스나 파일에서 윈도우로 리턴값을 던져줄때 문제가 발생된다.
	
	pChar->STRENGTH = MapUtil::GetDlgItemInt(m_hWnd, IDC_EDIT_CHAR_GONG);
	pChar->AGILITY  = MapUtil::GetDlgItemInt(m_hWnd, IDC_EDIT_CHAR_MIN);
	pChar->INTELLIGENCE = MapUtil::GetDlgItemInt(m_hWnd, IDC_EDIT_CHAR_JI);
	pChar->LUCK		= MapUtil::GetDlgItemInt(m_hWnd, IDC_EDIT_CHAR_LUCK);

	pChar->HP		= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_HP);
	pChar->MP		= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_MP);
	pChar->DEF		= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_DEF);
	pChar->SPEED	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_SPEED);
	pChar->ATTACK	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_ATTACK);
	pChar->ATK_SPD	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_ATK_SPD);
	pChar->MAGIC	= MapUtil::GetDlgItemFloat(m_hWnd, IDC_EDIT_MAGIC);
	*/
}

void CCharSetting::SetDefaultName(CHARACTER* pChar)
{
	static int count = 0;

	CString number;
	number.Format(_T("%d"), count);

	CString name = pChar->modelName + CString(L"(") + number + CString(L")");
	StrCpyW(pChar->name, name.GetBuffer());
	count++;

	number.Empty();
	name.Empty();
}

int CCharSetting::GetDlgInt(int ID)
{
	WCHAR strValue[256];
	GetDlgItemTextW(ID, strValue, 256);
	int iValue = _tstoi(strValue);

	CString ret;
	ret.Format(L"%d", iValue);

	SetDlgItemTextW(ID, ret);

	ret.Empty();

	return iValue;
}

void CCharSetting::GetDlgInt(int ID, int& dest)
{
	WCHAR strValue[256];
	GetDlgItemTextW(ID, strValue, 256);
	int iValue = _tstoi(strValue);

	CString ret;
	ret.Format(L"%d", iValue);

	SetDlgItemTextW(ID, ret);

	ret.Empty();

	dest = iValue;
}

float CCharSetting::GetDlgFloat(int ID)
{
	WCHAR strValue[256];
	GetDlgItemTextW(ID, strValue, 256);
	float fValue = _tstof(strValue);

	CStringW ret;
	ret.Format(L"%.2f", fValue);

	SetDlgItemTextW(ID, ret);

	ret.Empty();

	return fValue;
}

void CCharSetting::InitTree()
{

	/*HTREEITEM top_0 = m_TreeChar.InsertItem(L"동물", TVI_ROOT, TVI_LAST);
	HTREEITEM top_1 = m_TreeChar.InsertItem(L"식물", TVI_ROOT, TVI_LAST);
	HTREEITEM depth1_1 = m_TreeChar.InsertItem(L"고양이", top_0, TVI_LAST);
	HTREEITEM depth1_2 = m_TreeChar.InsertItem(L"선인장", top_1, TVI_LAST);
	*/
	HTREEITEM top = m_TreeChar.InsertItem(L"CHARACTER", TVI_ROOT, TVI_LAST);
	m_TreeChar.SetItemData(top, NULL);
	m_TreeItems.push_back(top);
	
}

void CCharSetting::UpdateModelListBox(__in std::vector<MODEL*>* pModelList)
{
	m_ModelListBox.ResetContent();	//ListBox 리셋

	std::vector<MODEL*>::iterator it;
	int i = 0;
	for (it = pModelList->begin(); it != pModelList->end(); ++it)
	{
		MODEL* pModel = (*it);
		CStringA fileName = pModel->fileName;
		CStringW wName(fileName);
		int index = m_ModelListBox.AddString(wName);

		if (index == LB_ERR)
			DebugBreak();

		m_ModelListBox.SetItemDataPtr(index, (void*)pModel);
		i++;
	}

	m_ModelListBox.UpdateWindow();
}

void CCharSetting::UpdateMatListBox(std::vector<MATERIAL*>* pMatList)
{
	m_MaterialListBox.ResetContent();	//ListBox 리셋

	std::vector<MATERIAL*>::iterator it;
	int i = 0;
	for (it = pMatList->begin(); it != pMatList->end(); ++it)
	{
		MATERIAL* pMat = (*it);
		CStringA fileName = pMat->matName;
		CStringW wName(fileName);
		int index = m_MaterialListBox.AddString(wName);

		if (index == LB_ERR)
			DebugBreak();

		m_MaterialListBox.SetItemDataPtr(index, (void*)pMat);
		i++;
	}

	m_MaterialListBox.UpdateWindow();
}

void CCharSetting::UpdateCharTreeBox()
{
}

MODEL* CCharSetting::GetModelByIndex(int index)
{
	MODEL* pMat = (MODEL*)m_ModelListBox.GetItemData(index);
	return pMat;
}

MATERIAL* CCharSetting::GetMatByIndex(int index)
{
	MATERIAL* pMat = (MATERIAL*)m_MaterialListBox.GetItemData(index);
	return pMat;
}

void CCharSetting::SetText(int ID, CStringA& str)
{
	CStringW matName(str);
	SetDlgItemTextW(ID, matName);
	matName.Empty();
}

void CCharSetting::SetInt(int ID, int index)
{
	CStringW mdx;
	mdx.Format(L"%d", index);
	SetDlgItemTextW(ID, mdx);
	mdx.Empty();
}

void CCharSetting::GetText(int ID, WCHAR* pDest)
{
	GetDlgItemTextW(ID, pDest, MAX_LENGTH);
	//GetDlgItemTextW(ID, pStr, sizeof(pStr) / sizeof(WCHAR) );
	//GetDlgItemTextW(ID, pStr, _countof(pStr) );
}

void CCharSetting::GetInt(int ID, int& dest)
{
	dest = MapUtil::GetDlgItemInt(m_hWnd, ID);
}

void CCharSetting::Request_ModelList()
{
	//LPARAM으로 파라 미터 전달.
	//특정 모델 리스트만 불러올수 잇다.
	::SendMessageW(m_hCenter, WM_REQUEST_MODEL_LIST, (WPARAM)(m_hWnd), NULL);
}

void CCharSetting::Request_MatList()
{
	//LPARAM으로 파라 미터 전달.
	//특정 마테리얼 리스트만 불러올수 잇다.
	::SendMessageW(m_hCenter, WM_REQUEST_MAT_LIST, (WPARAM)(m_hWnd), NULL);
}

void CCharSetting::SetCenter(HWND hCenter)
{
	m_hCenter = hCenter;
}

void CCharSetting::Move(int x, int y)
{
	CRect rc;
	this->GetWindowRect(rc);

	this->MoveWindow(x, y, rc.Width(), rc.Height(), 1);
}

BEGIN_MESSAGE_MAP(CCharSetting, CDialogEx)
	ON_BN_CLICKED(IDC_CHAR_CREATE, &CCharSetting::OnBnClickedCharCreate)
	ON_BN_CLICKED(IDC_CHAR_DELETE, &CCharSetting::OnBnClickedCharDelete)
	ON_LBN_SELCHANGE(IDC_LIST_MODEL, &CCharSetting::OnLbnSelchangeListModel)
	ON_LBN_SELCHANGE(IDC_LIST_MATERIAL, &CCharSetting::OnLbnSelchangeListMaterial)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CHAR, &CCharSetting::OnTvnSelchangedTreeChar)
	ON_MESSAGE(WM_UPDATE_MODEL_LIST, &CCharSetting::OnUpdateModelList)
	ON_MESSAGE(WM_UPDATE_MATERIAL_LIST, &CCharSetting::OnUpdateMatList)
END_MESSAGE_MAP()

// CCharSetting 메시지 처리기
void CCharSetting::OnBnClickedCharCreate()
{
	
	CHARACTER* pChar = new CHARACTER;
	ZeroMemory(pChar, sizeof(CHARACTER));
	GetCharInfo(pChar);

	::SendMessageW(m_hCenter, WM_CREATE_CHARACTER, (WPARAM)(pChar), NULL);

	HTREEITEM newItem = m_TreeChar.InsertItem(pChar->name, m_TreeItems[0], TVI_LAST);
	m_TreeChar.SetItemData(newItem, (DWORD_PTR)pChar);
	m_TreeItems.push_back(newItem);
}


void CCharSetting::OnBnClickedCharDelete()
{
	int a = 0;
}


void CCharSetting::OnLbnSelchangeListModel()
{
	int index = m_ModelListBox.GetCurSel();
	MODEL* pModel = GetModelByIndex(index);

	SetText(IDC_EDIT_CHAR_MODEL, pModel->fileName);
	SetInt(IDC_EDIT_CHAR_MODEL_INDEX, pModel->index);
}


void CCharSetting::OnLbnSelchangeListMaterial()
{
	//sendMessage로 변경.
	//해당 인덱스의 마테리얼을 요청하는 방식으로 변경?
	int index = m_MaterialListBox.GetCurSel();
	MATERIAL* pMat = GetMatByIndex(index);
	
	SetText(IDC_EDIT_CHAR_MATERIAL, pMat->matName);
	SetInt(IDC_EDIT_CHAR_MAT_INDEX, pMat->index);
}


void CCharSetting::OnTvnSelchangedTreeChar(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hTreeItem = m_TreeChar.GetSelectedItem();
	CHARACTER* pChar = (CHARACTER*)m_TreeChar.GetItemData(hTreeItem);

	if (pChar == NULL)
		goto lb_return;

	SetText(IDC_EDIT_CHAR_MATERIAL, CStringA(pChar->matName));
	SetInt(IDC_EDIT_CHAR_MAT_INDEX, pChar->mat_index);

	SetText(IDC_EDIT_CHAR_MODEL, CStringA(pChar->modelName) );
	SetInt(IDC_EDIT_CHAR_MODEL_INDEX, pChar->model_index );

	SetText(IDC_EDIT_CHAR_NAME, CStringA(pChar->name) );
	SetInt(IDC_EDIT_CHAR_INDEX, pChar->index);

lb_return:
	*pResult = 0;
}


BOOL CCharSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitTree();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


afx_msg LRESULT CCharSetting::OnUpdateModelList(WPARAM wParam, LPARAM lParam)
{
	std::vector<MODEL*>* pModelList = (std::vector<MODEL*>*)wParam;
	if (pModelList != NULL)
	{
		UpdateModelListBox(pModelList);
	}
	return 0;
}


afx_msg LRESULT CCharSetting::OnUpdateMatList(WPARAM wParam, LPARAM lParam)
{
	std::vector<MATERIAL*>* pMatList = (std::vector<MATERIAL*>*)wParam;
	if (pMatList != NULL)
	{
		UpdateMatListBox(pMatList);
	}
	return 0;
}
