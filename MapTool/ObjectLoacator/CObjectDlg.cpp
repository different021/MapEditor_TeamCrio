
// ObjectLoacatorDlg.cpp: 구현 파일
//
#include "pch.h"
#include <exception>
#include <algorithm>
#include <strsafe.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "CObjectDlg.h"
#include "Center.h"

#include "WMDefine.h"
#include "HGMessage.h"

#include "framework.h"
#include "resource.h"
#include "afxdialogex.h"
#include "MapUtil.h"
#include "CCallbackTimer.h"

#include "ToolDefine.h"


//#include "../src/MapLoader.h"
#include "../src/MAPLOAD.h"
#include "../src/utility/HGUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IsKeyDown(key)	((::GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((::GetAsyncKeyState(key)&0x8001) == 0x8001)

//전역 변수
HINSTANCE g_hIns;
//HWND g_hWndObjController;
HANDLE g_hEvent;

HWND g_hWndObjLocator;

CStringW g_SaveLoadPath = L".\\stage";

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#ifdef DEBUG
DWORD CheckFileDlgError()
{
#include <cderr.h>
	DWORD err = CommDlgExtendedError();

	if (err == CDERR_DIALOGFAILURE)
		__debugbreak();
	else if (err == CDERR_FINDRESFAILURE)
		__debugbreak();
	else if (err == CDERR_INITIALIZATION)
		__debugbreak();
	else if (err == CDERR_LOADRESFAILURE)
		__debugbreak();
	else if (err == CDERR_LOADSTRFAILURE)
		__debugbreak();
	else if (err == CDERR_LOCKRESFAILURE)
		__debugbreak();
	else if (err == CDERR_MEMALLOCFAILURE)
		__debugbreak();
	else if (err == CDERR_MEMLOCKFAILURE)
		__debugbreak();
	else if (err == CDERR_NOHINSTANCE)
		__debugbreak();
	else if (err == CDERR_NOHOOK)
		__debugbreak();
	else if (err == CDERR_NOTEMPLATE)
		__debugbreak();
	else if (err == CDERR_REGISTERMSGFAIL)
		__debugbreak();
	else if (err == CDERR_STRUCTSIZE)
		__debugbreak();

	return err;
}
#endif // DEBUG

//UTILL함수 로 이동? 
void OutputErrorCode(WCHAR* str)
{
	DWORD err = GetLastError();
	WCHAR msg[128];
	swprintf(msg, sizeof(msg) / sizeof(WCHAR), str, err);
	OutputDebugStringW(msg);
}


// CObjectLoacatorDlg 대화 상자
CObjectDlg::CObjectDlg(CWnd* pParent /*=nullptr*/)
	: CEditorDlg()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_curFocus = IDC_POS_X;
}

CObjectDlg::~CObjectDlg()
{
}

void CObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJLIST, m_objListBox);
	DDX_Control(pDX, IDC_MODELLIST, m_ModelListBox);
	DDX_Control(pDX, IDC_MATLIST, m_MatListBox);
	DDX_Control(pDX, IDC_LOG, m_LogBox);
	DDX_Control(pDX, IDC_COMBO_OBJTYPE, m_ComboBox_ObjectType);

	//DDX_Control(pDX, IDC_COMBO_ISCOLLISION, m_ComboBox_IsCollision);
	DDX_Control(pDX, IDC_COMBO_REGEN_COL_INDEX, m_ComboBox_RegenColliderList);
}

// CObjectLoacatorDlg 메시지 처리기
BEGIN_MESSAGE_MAP(CObjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_KILLFOCUS(IDC_POS_X, &CObjectDlg::OnKillfocusPosX)
	ON_EN_KILLFOCUS(IDC_POS_Y, &CObjectDlg::OnKillfocusPosY)
	ON_EN_KILLFOCUS(IDC_POS_Z, &CObjectDlg::OnKillfocusPosZ)
	ON_EN_KILLFOCUS(IDC_ROT_X, &CObjectDlg::OnKillfocusRotX)
	ON_EN_KILLFOCUS(IDC_ROT_Y, &CObjectDlg::OnKillfocusRotY)
	ON_EN_KILLFOCUS(IDC_ROT_Z, &CObjectDlg::OnKillfocusRotZ)
	ON_EN_KILLFOCUS(IDC_SCALE_X, &CObjectDlg::OnKillfocusScaleX)
	ON_EN_KILLFOCUS(IDC_SCALE_Y, &CObjectDlg::OnKillfocusScaleY)
	ON_EN_KILLFOCUS(IDC_SCALE_Z, &CObjectDlg::OnKillfocusScaleZ)
	ON_EN_KILLFOCUS(IDC_MOVE_OFFSET, &CObjectDlg::OnKillfocusMoveOffset)
	ON_BN_CLICKED(IDC_BTN_OBJEDIT, &CObjectDlg::OnBnClickedBtnObjedit)
	ON_BN_CLICKED(IDC_BTN_OBJDEL, &CObjectDlg::OnBnClickedBtnObjdel)
	ON_BN_CLICKED(IDC_BTN_OBJCREATE, &CObjectDlg::OnBnClickedBtnObjcreate)
	ON_LBN_SELCHANGE(IDC_OBJLIST, &CObjectDlg::OnSelchangeObjlist)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CObjectDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CObjectDlg::OnBnClickedBtnLoad)
	ON_LBN_SELCHANGE(IDC_MODELLIST, &CObjectDlg::OnLbnSelchangeModellist)
	ON_LBN_SELCHANGE(IDC_MATLIST, &CObjectDlg::OnLbnSelchangeMatlist)
	ON_MESSAGE(WM_UPDATE_OBJECT_LIST, &CObjectDlg::OnUpdateObjectList)
	ON_MESSAGE(WM_UPDATE_MODEL_LIST, &CObjectDlg::OnUpdateModelList)
	ON_MESSAGE(WM_UPDATE_MATERIAL_LIST, &CObjectDlg::OnUpdateMatList)
	ON_MESSAGE(WM_LIST_BOX_NONE, &CObjectDlg::OnListBoxNone)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_MAT_SELECTED, &CObjectDlg::OnBnClickedButtonEditMatSelected)
	ON_EN_CHANGE(IDC_POS_X, &CObjectDlg::OnEnChangePosX)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_CBN_SELCHANGE(IDC_COMBO_OBJTYPE, &CObjectDlg::OnCbnSelchangeComboObjtype)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//UpdateModelListBox();		--> 체크

	ClearLogBuffer();		//-> 이동 

	RequestModelList();
	RequestMatList();

	InitComboBox_ObjectType();
	UpdateRegenColiderList(NULL);
	//InitComboBox_IsCollision();

	g_hWndObjLocator = m_hWnd;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CObjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE)
	{
		//ShowWindow(SW_HIDE);
		::SendMessageW(g_hCenter, WM_HIDE_WINDOW, WPARAM(this), eDlgs::eObjDlg);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CObjectDlg::OnPaint()
{
	//Update ListBoxes  
	//처움부터 다시 그려주기때문에 오브젝트가 많으면 느려질 수 있다
	m_objListBox.UpdateWindow();
	m_ModelListBox.UpdateWindow();
	m_MatListBox.UpdateWindow();

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CObjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//세이브 창 띄우기
INT_PTR CObjectDlg::GetNamePath(CStringW& outName, CStringW& outPath)
{
	CFileDialog saveDlg(FALSE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, NULL, this, 0, TRUE);	//| OFN_ALLOWMULTISELECT 다 수 파일 선택	
	INT_PTR nResponse = saveDlg.DoModal();

#ifdef DEBUG
	static DWORD err = CheckFileDlgError();
	int a = 0;
#endif // DEBUG

	if (nResponse == IDOK)
	{
		outName = saveDlg.GetFileName();		//FileName.확장자 전체 이름 리턴
		outPath = saveDlg.GetPathName();		//전체 경로 리턴
		//saveDlg.EndDialog(1);					//호출할 필요없다;.
	}
	else
	{
		int a = 0;
	}

	return nResponse;
}



float CObjectDlg::CheckInput_Float(int controllerID)
{
	WCHAR strValue[64];
	::GetDlgItemTextW(m_hWnd, controllerID, strValue, 64);
	float fValue = _tstof(strValue);

	CString ret;
	ret.Format(L"%.2f", fValue);

	SetDlgItemText(controllerID, ret);

	return fValue;
}

void CObjectDlg::SetDlgFloat(int controllerID, float value)
{
	//WCHAR ret[64];
	CString ret;
	ret.Format(L"%0.2f", value );
	SetDlgItemText(controllerID, ret);
	GetDlgItem(controllerID)->UpdateWindow();
}

void CObjectDlg::GetStringFromIDC(int controllerID, CString& dest)
{
	GetDlgItemText(controllerID, dest);
}

void CObjectDlg::SetObjName(int controllerID, object* pDest)
{
	CString blank = L"";
	CString name;
	GetStringFromIDC(controllerID, name);
	if (name.Compare(blank) != 0)
	{
		//비교한 두문자가 같으면 0리턴.
		//같지 않은경우 들어온다.
		wsprintf(pDest->objectName, name.GetBuffer());
	}
}

void CObjectDlg::SetObjMaterialName(int controllerID, object* pDest)
{
	CString blank = L"";
	CString materialName;
	GetStringFromIDC(controllerID, materialName);
	if (materialName.Compare(blank) != 0)
	{
		//비교한 두문자가 같으면 0리턴.
		//같지 않은경우 들어온다.
		wsprintf(pDest->matName, materialName.GetBuffer());
	}
}

void CObjectDlg::InitComboBox_ObjectType()
{
	LPCTSTR szTypeName[eOBJECT_MAX] = {  L"FIELD_COLLISION_YES", L"FIELD_COLLISION_NO", L"ENEMY_NORMAL", L"ENEMY_RANGE", L"ENEMY_DASH", L"NPC_REINFORCE", L"ENEMY_BOSS" };
	
	for (int i = 0; i < eOBJECT_MAX; i++)
	{
		int index = m_ComboBox_ObjectType.AddString(szTypeName[i]);
		m_ComboBox_ObjectType.SetItemData( index, (eOBJECT_TYPE)i );
	}
	
	//int index = m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_ENEMY_NORMAL]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_ENEMY_NORMAL);

	//index = m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_ENEMY_BOSS]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_ENEMY_BOSS);

	//index	= m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_ENEMY_RANGE]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_ENEMY_RANGE);

	//index	= m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_ENEMY_DASH]);
	//m_ComboBox_ObjectType.SetItemData(index,   eOBJECT_ENEMY_DASH);

	//index = m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_NPC_REINFORCE]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_NPC_REINFORCE);


	//index = m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_FIELD_COLLISION_YES]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_FIELD_COLLISION_YES);

	//index = m_ComboBox_ObjectType.AddString(szTypeName[eOBJECT_FIELD_COLLISION_NO]);
	//m_ComboBox_ObjectType.SetItemData(index, eOBJECT_FIELD_COLLISION_NO);

	m_ComboBox_ObjectType.SetCurSel(0);
}

void CObjectDlg::MoveFocusNext()
{
	UINT editlist[12] =
	{
		IDC_POS_X,
		IDC_POS_Y,
		IDC_POS_Z,
		IDC_ROT_X,
		IDC_ROT_Y,
		IDC_ROT_Z,
		IDC_SCALE_X,
		IDC_SCALE_Y,
		IDC_SCALE_Z,
		IDC_BTN_OBJCREATE,
		IDC_BTN_SAVE,
		IDC_BTN_OBJDEL,
	};

	//edit control 포커스 이동
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus++;
	if ((m_curFocus > IDC_SCALE_Z) || (m_curFocus < IDC_POS_X))	m_curFocus = IDC_POS_X;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

void CObjectDlg::MoveFocusBack()
{
	//edit control 포커스 이동
	m_curFocus = GetFocus()->GetDlgCtrlID();
	m_curFocus--;
	if ((m_curFocus > IDC_SCALE_Z) || (m_curFocus < IDC_POS_X))	m_curFocus = IDC_SCALE_Z;

	CEdit* pEdit = (CEdit*)GetDlgItem(m_curFocus);
	pEdit->SetSel(0, -1);			//글자 파란색 선택
	pEdit->SetFocus();
}

//void CObjectLoacatorDlg::InitComboBox_IsCollision()
//{
//	LPCTSTR szTypeName[eCollisionMax] = { L"YesCollision", L"NoCollision" };
//	int index = m_ComboBox_IsCollision.AddString(szTypeName[eCollisionYes]);
//	m_ComboBox_IsCollision.SetItemData(index, eCollisionYes);
//
//	index = m_ComboBox_IsCollision.AddString(szTypeName[eCollisionNo]);
//	m_ComboBox_IsCollision.SetItemData(index, eCollisionNo);
//
//	m_ComboBox_IsCollision.SetCurSel(0);
//}




void CObjectDlg::SetObjSelectedModel(object* pObj)
{
	pObj->modelIndex = GetSelectedModelIndex();
}

void CObjectDlg::SetObjSelectedMat(object* pObj)
{
	pObj->matIndex = GetSelectedMaterialIndex();
}

void CObjectDlg::SetDefaultName(object* pObj)
{
	static int count = 0;
	int index = m_ModelListBox.FindString(0, pObj->modelName);

	CString number;
	number.Format(_T("%d"), count);
	
	CString name = pObj->modelName + CString(L"_") + number;
	StrCpyW(pObj->objectName, name.GetBuffer());
	count++;

	number.Empty();
	name.Empty();

}

void CObjectDlg::SetModelName(object* pDest, int modelIndex)
{
	int index = m_ModelListBox.GetCurSel();
	CString modelName;
	m_ModelListBox.GetText(index, modelName);
	wsprintfW(pDest->modelName, modelName.GetBuffer());
	modelName.Empty();
}


//리스트 박스에서 선택한 오브젝트를 가져온다.
object* CObjectDlg::GetSelectedObj()
{
	object* pResult = NULL;
	int index = m_objListBox.GetCurSel();

	//LB_ERR 는 -1
	if (index > LB_ERR)
		pResult = (object*)m_objListBox.GetItemDataPtr(index);

	return pResult;
}

object* CObjectDlg::GetObjectByIndex(int index)
{
	int numOfObj = m_objListBox.GetCount();

	if (numOfObj <= 0)
		return NULL;
	else if (numOfObj > index)
		return (object*)m_objListBox.GetItemDataPtr(index);
	else
		return NULL;
}

int CObjectDlg::GetSelectedModelIndex()
{
	int index = m_ModelListBox.GetCurSel();
	MODEL* pModel = NULL;
	if (index == LB_ERR)
	{
		//LB_ERR; 제대로 못찾은 경우	
		OutputDebugStringW(L"[GetSelectedModelIndex] Can not Found Model Index\n");
		return 0;
	}
	pModel = (MODEL*)(m_ModelListBox.GetItemDataPtr(index));
	if (pModel == NULL)
	{
		//모델을 못찾을 경우 
		//경우에따라 -1, -2 등을 리턴해 에러임을ㅇ 나타내자
		OutputDebugStringW(L"[GetSelectedModelIndex] Can not Found Model\n");
		return 0;
	}
	int iResult = pModel->index;

	return iResult;
}

int CObjectDlg::GetSelectedMaterialIndex()
{
	int index = m_MatListBox.GetCurSel();
	if (index == LB_ERR)
	{
		//LB_ERR; 제대로 못찾은 경우	
		OutputDebugStringW(L"[GetSelectedMaterialIndex] Can not Found MATERIAL Index\n");
		return 0;
	}
	
	MATERIAL* pMat = (MATERIAL*)m_MatListBox.GetItemDataPtr(index);
	if (pMat == 0)
	{
		//마테리얼을 제대로 못찾은 경우
		OutputDebugStringW(L"[GetSelectedMaterialIndex] Can not Found MATERIAL Index\n");
		return 0;
	}
	int iResult = pMat->index;

	return iResult;
}


MODEL* CObjectDlg::GetModelFromListBox(int index)
{
	MODEL* pModel = g_pCenter->GetModelByInde(index);
	return pModel;
}

MATERIAL* CObjectDlg::GetMaterialListBox(int index)
{
	MATERIAL* pMat = (MATERIAL*)m_MatListBox.GetItemDataPtr(index);

	return pMat;
}

// 함수를 콜한 순간 삭제하지 않는다.
// Center에서는 삭제할 인스턴스를 삭제리스트에 넣는다.
// 삭제리스트의 인스턴스들은 업데이트 이전에 일괄 삭제 처리한다. 
// -> dx12의 커맨드 큐에 의해서 삭제한 인스턴스를 참조하는 문제가 발생할 수 있다.
void CObjectDlg::DeleteGraphicInstance(object* pObj)
{
	if (pObj == NULL)
		return;

	RequestCenter(WM_OBJECT_DELETE, (WPARAM)pObj, 0);
}

DirectX::XMFLOAT4 CObjectDlg::ConvertMatrixToQuaternion(DirectX::XMFLOAT3& oiler)
{
	XMFLOAT4 rot = {};
	XMMATRIX matrix = XMMatrixRotationRollPitchYaw(oiler.x, oiler.y, oiler.z);
	XMVECTOR vQuaternion = XMQuaternionRotationMatrix(matrix);
	Vector4 quaternion(vQuaternion);

	rot.x = quaternion.x;
	rot.y = quaternion.y;
	rot.z = quaternion.z;
	rot.w = quaternion.w;

	return rot;
}

DirectX::XMFLOAT4X4 CObjectDlg::ConvertQuaternionToMatrix(DirectX::XMFLOAT4& quaternion)
{
	XMMATRIX matrix = XMMatrixRotationQuaternion( XMLoadFloat4(&quaternion) );		//matrix를 리턴해서 앞에 둔다.
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, matrix);		//일반적인 메모리에서쓰는 XMFLOAT4x4
	
	return result;
}

//삭제 예정. -> 단일 오브젝트에 대한 함수로 만들고 생성 객체 마다 함수 호출로 변경 할 것.
void CObjectDlg::UpdateObjListBox(std::vector<DRAW_INSTANCE*>* pList)
{
	m_objListBox.ResetContent();

	if (pList == NULL)
		return;

	int objCnt = pList->size();

	if (objCnt == 0)
	{
		m_objListBox.UpdateWindow();
		return;
	}

	for (int i = 0; i < objCnt; i++)
	{
		AddObjectInComboBox(pList->at(i)->first);
		/*int idx = m_objListBox.AddString(pList->at(i)->first->objectName);
		m_objListBox.SetItemDataPtr(idx, (void*)(pList->at(i)->first));*/
	}

	m_objListBox.UpdateWindow();
}


//ADD Object in ComboBox by object
// return : object combobox Index
// return -1 : when  incaildate parameter
int CObjectDlg::AddObjectInComboBox(object* pNew)
{
	int index = -1;
	object* pObj = pNew;
	if (pObj == nullptr) return index;

	index = m_objListBox.AddString(pObj->objectName);
	m_objListBox.SetItemDataPtr(index, (void*)(pObj));

	return index;
}

//return false When Can't find object in ComboBox
//return true when delete item same object
bool CObjectDlg::DeleteItemInComboBox(object* pDeleteTarget)
{
	bool bResult = false;
	object* pTarget = pDeleteTarget;
	if (pTarget == nullptr) return bResult;

	int count = m_objListBox.GetCount();		//순회해야할 총 횟수 (= 리스트박스 품목 갯수 만큼)
	for (int i = 0; i < count; i++)
	{
		object* pObj = (object*)m_objListBox.GetItemData(i);
		if (pObj == pTarget)
		{
			m_objListBox.DeleteString(i);
			bResult = true;
			break;
		}
	}

	return bResult;
}


void CObjectDlg::UpdateModelListBox(std::vector<MODEL*>* pModelList)
{
	m_ModelListBox.ResetContent();	//ListBox 리셋

	if (pModelList == NULL)
		return;

	std::vector<MODEL*>::iterator it;
	int id = 0;
	for (it = pModelList->begin(); it != pModelList->end(); ++it)
	{
		MODEL* pModel = (*it);
		//CStringA fileName = pModel->fileName;
		//CStringW wName(fileName);
		std::string fileName = pModel->fileName;
		int index = m_ModelListBox.AddString( CString(pModel->fileName.c_str()) );			//메모리 누수 체크

		if (index == LB_ERR)
			DebugBreak();

		m_ModelListBox.SetItemDataPtr(index, (void*)pModel);
		id++;
	}

	m_ModelListBox.UpdateWindow();
	
}

void CObjectDlg::UpdateMatListBox(std::vector<MATERIAL*>* pMatList)
{
	m_MatListBox.ResetContent();	//ListBox 리셋

	if (pMatList == NULL)
		return;

	std::vector<MATERIAL*>::iterator it;
	int i = 0;
	for (it = pMatList->begin(); it != pMatList->end(); ++it)
	{
		MATERIAL* pMat = (*it);
		CStringA fileName = pMat->matName;
		CStringW wName(fileName);
		int index = m_MatListBox.InsertString(i, wName);
		m_MatListBox.SetItemDataPtr(index, (void*)(pMat));
		i++;
	}

	m_MatListBox.UpdateWindow();
}

void CObjectDlg::DeleteObjInListBox(std::vector<object*>* pList)
{
	if (pList == nullptr) return;
	std::vector<object*>::iterator it;

	//삭제할 대상 찾아서 삭제.
	for (it = pList->begin(); it != pList->end(); it++)
	{
		object* pTarget = *it;
		int count = m_objListBox.GetCount();
		//리스트 박스 순회.
		for (int i = 0; i < count; i++)
		{
			object* pObj = (object*)m_objListBox.GetItemData(i);
			if (pObj == pTarget)	
			{
				//타겟을 찾았으니 삭제한다.
				m_objListBox.DeleteString(i);
				break;
			}
		}
	}
}

//종속적인 함수가 아니라 utill등으로 이동. 요망.
//IDC_ editbox의 id 가 연속적이여야 한다.
//ex)	IDC_POS_X 10001
//		IDC_POS_Y IDC_POS_X + 1
//		IDC+POS_Z IDC_POS_X + 2
void CObjectDlg::GetVector(UINT id, DirectX::XMFLOAT3& dest)
{
	dest.x = GetValueFromDlg(id, 0.f);;
	dest.y = GetValueFromDlg(id + 1, 0.f);;
	dest.z = GetValueFromDlg(id + 2, 0.f);;
}

void CObjectDlg::GetPos(DirectX::XMFLOAT3& pos)
{
	pos.x = GetValueFromDlg(IDC_POS_X, 0.f);;
	pos.y = GetValueFromDlg(IDC_POS_Y, 0.f);;
	pos.z = GetValueFromDlg(IDC_POS_Z, 0.f);;
}

void CObjectDlg::GetOiler(DirectX::XMFLOAT3& oil)
{
	oil.x = GetValueFromDlg(IDC_ROT_X, 0.f);		//라디안 X값
	oil.y = GetValueFromDlg(IDC_ROT_Y, 0.f);		//라디안 Y값
	oil.z = GetValueFromDlg(IDC_ROT_Z, 0.f);		//라디안 Z값
}

void CObjectDlg::GetQuaternion(DirectX::XMFLOAT4& rot)
{
	//쿼터니언 변환이 필요하다.
	DirectX::XMFLOAT3 radian;
	radian.x = GetValueFromDlg(IDC_ROT_X, 0.f);		//라디안 X값
	radian.y = GetValueFromDlg(IDC_ROT_Y, 0.f);		//라디안 Y값
	radian.z = GetValueFromDlg(IDC_ROT_Z, 0.f);		//라디안 Z값
	
	//MatrixToFloat4
	
	rot = ConvertMatrixToQuaternion(radian);
}

void CObjectDlg::GetScale(DirectX::XMFLOAT3& scale)
{
	scale.x = GetValueFromDlg(IDC_SCALE_X, 1.0f);
	scale.y = GetValueFromDlg(IDC_SCALE_Y, 1.0f);
	scale.z = GetValueFromDlg(IDC_SCALE_Z, 1.0f);
}

float CObjectDlg::GetValueFromDlg(int id, float defaultValue)
{
	float fReturn;
	CString strValue;
	GetDlgItemText(id, strValue);
	int cpr = strValue.Compare(L"");			//비교값이 같으면 0 리턴
	if (cpr == 0)
		fReturn = defaultValue;
	else
		fReturn = _tstof(strValue);
	return fReturn;
}

eOBJECT_TYPE CObjectDlg::SetObjTypeFromComboBox(object* pDest)
{
	int index = m_ComboBox_ObjectType.GetCurSel();
	eOBJECT_TYPE type;
	if (index == CB_ERR)
	{
		type = (eOBJECT_TYPE)m_ComboBox_ObjectType.GetItemData(0);
		m_ComboBox_ObjectType.SetCurSel(0);
	}
	else
		type = (eOBJECT_TYPE)m_ComboBox_ObjectType.GetItemData(index);

	pDest->type = type;
	return type;
}

//eIsCollision CObjectLoacatorDlg::SetObjIsCollision(object* pResult)
//{
//	int index = m_ComboBox_IsCollision.GetCurSel();
//	eIsCollision isCollision = eIsCollision::eCollisionMax;
//	if (index == CB_ERR)
//	{
//		isCollision = (eIsCollision)m_ComboBox_IsCollision.GetItemData(0);
//		m_ComboBox_IsCollision.SetCurSel(0);
//	}
//	else
//		isCollision = (eIsCollision)m_ComboBox_IsCollision.GetItemData(index);
//
//	pResult->voidSpace = 0;
//	return isCollision;
//}

void CObjectDlg::SetComboBoxType(object* pObj)
{
	if (pObj == NULL)
	{
		m_ComboBox_ObjectType.SetCurSel(-1);
		return;
	}
	
	eOBJECT_TYPE type = pObj->type;
	int IndexOfSel = 0;
	for (int i = 0; i < eOBJECT_MAX; i++)
	{
		if (type == (eOBJECT_TYPE)m_ComboBox_ObjectType.GetItemData(i))
		{
			IndexOfSel = i;
			break;
		}
	}

	if (IndexOfSel < eOBJECT_MAX)
	{
		
		m_ComboBox_ObjectType.SetCurSel(IndexOfSel);
		//m_ComboBox_RegenColliderList.EnableWindow(TRUE);

		if (type == eOBJECT_FIELD_COLLISION_YES || type == eOBJECT_FIELD_COLLISION_NO)
		{
			m_ComboBox_RegenColliderList.EnableWindow(FALSE);
		}
		else
		{
			m_ComboBox_RegenColliderList.EnableWindow(TRUE);
		}
	}
	else
	{
		m_ComboBox_ObjectType.SetCurSel(-1);
	}
		

}

//void CObjectLoacatorDlg::SetComboBoxIsCollision(object* pObj)
//{
//	if (pObj == NULL)
//	{
//		m_ComboBox_IsCollision.SetCurSel(0);
//		return;
//	}
//
//	eIsCollision isCollision = pObj->isCollision;
//	int IndexOfSel = 0;
//	for (int i = 0; i < eCollisionMax; i++)
//	{
//		if (isCollision == (eIsCollision)m_ComboBox_IsCollision.GetItemData(i))
//		{
//			IndexOfSel = i;
//			break;
//		}
//	}
//
//	if (IndexOfSel < eCollisionMax && IndexOfSel >= 0)
//		m_ComboBox_IsCollision.SetCurSel(IndexOfSel);
//	else
//		m_ComboBox_IsCollision.SetCurSel(-1);
//}

void CObjectDlg::SetPosEditBoxText(float x, float y, float z)
{
	SetDlgFloat(IDC_POS_X, x);
	SetDlgFloat(IDC_POS_Y, y);
	SetDlgFloat(IDC_POS_Z, z);
}

void CObjectDlg::SetPosEditBoxText(DirectX::XMFLOAT3& pos)
{
	SetDlgFloat(IDC_POS_X, pos.x);
	SetDlgFloat(IDC_POS_Y, pos.y);
	SetDlgFloat(IDC_POS_Z, pos.z);
}




void CObjectDlg::SetRotEditBoxText(DirectX::XMFLOAT3& olier)
{
	SetDlgFloat(IDC_ROT_X, olier.x);
	SetDlgFloat(IDC_ROT_Y, olier.y);
	SetDlgFloat(IDC_ROT_Z, olier.z);
}

void CObjectDlg::SetScaleEditBoxText(float x, float y, float z)
{
	SetDlgFloat(IDC_SCALE_X, x);
	SetDlgFloat(IDC_SCALE_Y, y);
	SetDlgFloat(IDC_SCALE_Z, z);
}

void CObjectDlg::SetScaleEditBoxText(DirectX::XMFLOAT3& scale)
{
	SetDlgFloat(IDC_SCALE_X, scale.x);
	SetDlgFloat(IDC_SCALE_Y, scale.y);
	SetDlgFloat(IDC_SCALE_Z, scale.z);
}

void CObjectDlg::SetNameEditBoxText(object* pObj)
{
	CString ret;
	if (pObj == NULL)
		ret = L"";
	else
		ret = pObj->objectName;
	SetDlgItemText(IDC_OBJECT_NAME, ret);
	GetDlgItem(IDC_OBJECT_NAME)->UpdateWindow();
}

void CObjectDlg::SetNameEditBoxText(WCHAR* pStr)
{
	CString ret = CString(pStr);
	SetDlgItemText(IDC_OBJECT_NAME, ret);
	GetDlgItem(IDC_OBJECT_NAME)->UpdateWindow();
}

void CObjectDlg::SetModelBoxIndex(object* pObj)
{
	if (pObj == NULL)
	{
		m_ModelListBox.SetCurSel(-1);
		return;
	}
		

	int index = 0;
	int count = m_ModelListBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		MODEL* pTemp = (MODEL*)m_ModelListBox.GetItemDataPtr(i);
		if (pTemp->index == pObj->modelIndex)
		{
			index = i;
			break;
		}
	}

	m_ModelListBox.SetCurSel(index);
}

void CObjectDlg::SetMatBoxIndex(object* pObj)
{
	if (pObj == NULL)
	{
		m_MatListBox.SetCurSel(-1);
		return;
	}
		

	int index = 0;
	int count = m_MatListBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		MATERIAL* pTemp = (MATERIAL*)m_MatListBox.GetItemDataPtr(i);
		if (pTemp->index == pObj->matIndex)
		{
			index = i;
			break;
		}
	}

	m_MatListBox.SetCurSel(index);
}

void CObjectDlg::SetObjBoxIndex(object* pObj)
{
	if (pObj == NULL)
	{
		m_objListBox.SetCurSel(-1);
		return;
	}
		

	int index = -1;
	int count = m_objListBox.GetCount();
	for (int i = 0; i < count; i++)
	{
		object* pTemp = (object*)m_objListBox.GetItemDataPtr(i);
		if (pTemp == pObj)
		{
			index = i;
			break;
		}
	}

	m_objListBox.SetCurSel(index);
}


void CObjectDlg::SetModelIndexEditBox(int index)
{
	CString ret;
	ret.Format(L"%d", index);
	SetDlgItemText(IDC_MODEL_INDEX, ret);
	GetDlgItem(IDC_MODEL_INDEX)->UpdateWindow();
}

void CObjectDlg::SetModelNameEditBox(CString strModelName)
{
	SetDlgItemText(IDC_MODEL_NAME, strModelName);
	GetDlgItem(IDC_MODEL_NAME)->UpdateWindow();
	strModelName.Empty();
}

void CObjectDlg::SetMatIndexEditbox(int index)
{
	CString ret;
	ret.Format(L"%d", index);
	SetDlgItemText(IDC_MAT_INDEX, ret);
	GetDlgItem(IDC_MAT_INDEX)->UpdateWindow();
	ret.Empty();
}

void CObjectDlg::SetMatNameEditBox(CString strMatName)
{
	SetDlgItemText(IDC_MAT_NAME, strMatName);
	GetDlgItem(IDC_MAT_NAME)->UpdateWindow();
	strMatName.Empty();
}



void CObjectDlg::ClearFileName()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_FILE_NAME);
	pEdit->SetSel(0, -1, TRUE);
	pEdit->Clear();
}


void CObjectDlg::UpdateRegenColiderList(const std::vector<COLLIDER>* pColList)
{
	m_ComboBox_RegenColliderList.ResetContent();
	std::vector<COLLIDER>::const_iterator it;

	int idx = m_ComboBox_RegenColliderList.AddString(L"NONE");
	m_ComboBox_RegenColliderList.SetItemData(idx, (DWORD_PTR)(NULL));

	if (pColList == NULL) return;

	for (it = pColList->begin(); it != pColList->end(); it++)
	{
		collider* pCollider = it->first;
		if (pCollider != NULL)
		{
			if (pCollider->type == eCOLLIDER_TYPE::eCOLLIDER_REGENERATIONPOINT)
			{
				int IndexOfCollider = m_ComboBox_RegenColliderList.AddString(pCollider->name);
				m_ComboBox_RegenColliderList.SetItemData(IndexOfCollider, (DWORD_PTR)(pCollider));
			}
		}
	}

}

void CObjectDlg::SetRegenComboBoxNONE()
{
	int count = m_ComboBox_RegenColliderList.GetCount();
	for (int i = 0; i < count; i++)
	{
		  collider* pCollider = (collider*)m_ComboBox_RegenColliderList.GetItemData(i);
		  if (pCollider == NULL)
		  {
			  m_ComboBox_RegenColliderList.SetCurSel(i);
			  break;
		  }
	}
}

void CObjectDlg::SetRegenColliderComboBox(int index)
{
	if (index < 0)
	{
		SetRegenComboBoxNONE();
		//m_ComboBox_RegenColliderList.SetCurSel(-1);
		return;
	}

	int size = m_ComboBox_RegenColliderList.GetCount();
	for (int i = 0; i < size; i++)
	{
		collider* pCollider = (collider*)m_ComboBox_RegenColliderList.GetItemData(i);
		if (pCollider == NULL) continue;
		if (pCollider->index == index)
		{
			m_ComboBox_RegenColliderList.SetCurSel(i);
			break;
		}
	}

}

//리젠 콜라이더박스 인덱스 셋팅.
void CObjectDlg::SetRegenIndexFromComboBox(object* pDest)
{
	if (pDest == NULL) return;
	collider* pRegenCollider = GetRegenColliderFromCombo();
	int RegenIndex = -1;
	if (pRegenCollider != NULL)
	{
		RegenIndex = pRegenCollider->index;
	}
	pDest->regenIndex = RegenIndex;
}

collider* CObjectDlg::GetRegenColliderFromCombo()
{
	int curIndex = m_ComboBox_RegenColliderList.GetCurSel();
	if (curIndex == -1) return NULL;
	collider* pCollider = (collider*)m_ComboBox_RegenColliderList.GetItemData(curIndex);

	return pCollider;
}

void CObjectDlg::SetObjRegenIndexByComboBox(object* pDest)
{
	if (pDest == NULL) return;
	collider* pCollider = GetRegenColliderFromCombo();
	if (pCollider == NULL)
		pDest->regenIndex = -1;
	else
		pDest->regenIndex = pCollider->index;
}

void CObjectDlg::UpdateEditBoxesByObject(object* pObj)
{
	if (pObj == NULL)
	{
		DirectX::XMFLOAT3 zero3 = {};
		DirectX::XMFLOAT4 zero4 = {};
		DirectX::XMFLOAT3 scale = { 1.f, 1.f, 1.f };

		SetPosEditBoxText(zero3);
		SetRotEditBoxText(zero3);
		SetScaleEditBoxText(scale);
		
		SetModelIndexEditBox(0);
		SetModelNameEditBox(L"");
		SetMatIndexEditbox(0);
		SetMatNameEditBox(L"");

		m_objListBox.SetCurSel(-1);
		SetRegenColliderComboBox(-1);
	}
	else
	{
		SetPosEditBoxText(pObj->pos);
		//쿼터니언을 오일러로 변경하여 넘겨줄것.
		XMFLOAT3 oiler = HGUtility::QuaternionToOiler(pObj->quaternion);
		SetRotEditBoxText(oiler);
		SetScaleEditBoxText(pObj->scale);

		SetModelIndexEditBox(pObj->modelIndex);
		SetModelNameEditBox(pObj->modelName);
		SetMatIndexEditbox(pObj->matIndex);
		SetMatNameEditBox(pObj->matName);

		SetRegenColliderComboBox(pObj->regenIndex);
	}

	SetComboBoxType(pObj);
	
	SetNameEditBoxText(pObj);
	SetObjBoxIndex(pObj);
	SetModelBoxIndex(pObj);
	SetMatBoxIndex(pObj);
}

void CObjectDlg::SetIndex(int index)
{
	int num = m_objListBox.GetCount();
	if (num < index || index < 0)
	{
		OutputDebugStringW(L"[ERROR]SetObjectEditBoxes\t");
		OutputDebugStringW(L"m_objListBox. Out of index\n");
		return;
	}

	m_objListBox.SetCurSel(index);
	object* pObj = GetObjectByIndex(index);

	if (pObj == NULL)
	{
		float fValue = 0.f;
		DirectX::XMFLOAT3 zero3 = {};
		DirectX::XMFLOAT4 zero4 = {};

		SetPosEditBoxText(zero3);
		SetRotEditBoxText(zero3);
		SetScaleEditBoxText(1.f, 1.f, 1.f);
		SetNameEditBoxText(L"");

		SetModelIndexEditBox(0);
		SetModelNameEditBox(L"");
		SetMatIndexEditbox(0);
		SetMatNameEditBox(L"");
	}
	else
	{
		SetPosEditBoxText(pObj->pos);
		SetRotEditBoxText(pObj->oiler);
		SetScaleEditBoxText(pObj->scale);
		SetNameEditBoxText(pObj);

		SetModelIndexEditBox(pObj->modelIndex);
		SetModelNameEditBox(pObj->modelName);
		SetMatIndexEditbox(pObj->matIndex);
		SetMatNameEditBox(pObj->matName);
	}
}

int CObjectDlg::GetIndex()
{
	return m_objListBox.GetCurSel();
}



void CObjectDlg::GetMapNameFromDlg(int id, CString& dest)
{
	GetDlgItemTextW(id, dest);
	dest = dest + L".map";
}

//폴더가 없을 경우 생성.
void CObjectDlg::CreateFolder(CStringW& folderName)
{
	//파일 경로 생성.
	if ((_waccess(folderName.GetBuffer(), 0)) == -1)
	{
		OutputDebugString(L"Create Folder\n");
		CreateDirectory(folderName.GetBuffer(), NULL);
	}
}

void CObjectDlg::OutputDebugCode(wchar_t* pStr)
{
	//에러 코드 출력
	DWORD dwRet = GetLastError();
	WCHAR buffer[256];
	wsprintfW(buffer, L"%s ERROR CODE : %d", pStr, dwRet);
	OutputDebugStringW(buffer);
}

void CObjectDlg::RequestModelList()
{
	::SendMessageW(g_hCenter, WM_REQUEST_MODEL_LIST, (WPARAM)m_hWnd, NULL);
}

void CObjectDlg::RequestMatList()
{
	::SendMessageW(g_hCenter, WM_REQUEST_MAT_LIST, (WPARAM)m_hWnd, NULL);
}


void CObjectDlg::GetObjectDataFromEditBox(object* pDest)
{
	object* pObj = pDest;
	if (pObj != NULL)
	{
		XMFLOAT3 pos = {  };
		XMFLOAT3 oiler = {  };
		XMFLOAT3 scale = {  };

		GetVector(IDC_POS_X, pos);		
		GetVector(IDC_ROT_X, oiler);
		GetVector(IDC_SCALE_X, scale);

		pObj->SetPos(pos);
		pObj->SetOiler(oiler);
		pObj->SetScale(scale);

		SetObjTypeFromComboBox(pObj);
		SetObjRegenIndexByComboBox(pObj);

		int curModelIndex = GetSelectedModelIndex();
		int prvModelIndex = pObj->modelIndex;

		int curMatIndex = GetSelectedMaterialIndex();
		int prvMatIndex = pObj->matIndex;

		SetObjName(IDC_OBJECT_NAME, pObj);
		SetObjMaterialName(IDC_MAT_NAME, pObj);

		if ((prvModelIndex != curModelIndex) || (curMatIndex != prvMatIndex))
		{
			pObj->modelIndex = curModelIndex;
			pObj->SetMatIndex(curMatIndex);

			//위치 변경.
			g_pCenter->UpdateMaterial(pObj);

		}
		else
		{
			//변화가 없으면 아무것도 하지 않는다.
		}

		int index = m_objListBox.GetCurSel();
		SetIndex(index);

		wchar_t buf[256];
		wsprintfW(buf, L"[EDIT OBJECT] %s", pObj->objectName);	//로그
		AddLogStr(buf);
	}

}


void CObjectDlg::HideAndShow()
{
	m_bIsShow = !m_bIsShow;

	int para;

	if (m_bIsShow)
		para = SW_SHOW;
	else
		para = SW_HIDE;

	ShowWindow(para);
}

bool CObjectDlg::IsDocking()
{
	return m_bIsDocking;
}

void CObjectDlg::ToggleDocking_()
{
	m_bIsDocking = !m_bIsDocking;
}

void CObjectDlg::ClearLogBuffer()
{
	ZeroMemory(m_strLog, sizeof(m_strLog));
}

void CObjectDlg::AddLogStr(wchar_t* strLog)
{
	int logSize = sizeof(strLog);
	if (logSize > STR_MAX * 2)
	{
		MapUtil::DebugString(L"Log String Length Is Too Long\n");
		return;
	}

	wsprintfW(m_strLog, strLog);
	UpdateLogBox();
}

void CObjectDlg::AddLogTest(LPCWSTR pStr, ...)
{
	//CString str;
	LPCWSTR str = pStr;

	va_list list;
	va_start(list, pStr);

	while (str != NULL)
	{
		str = va_arg(list, LPCWSTR);
		OutputDebugStringW(str);
	}
	va_end(list);

	return;
}

void CObjectDlg::OutCurDirAfterStr(wchar_t* strLog)
{
	GetCurrentDirectoryW(STR_MAX, m_strLog);
	wcscat_s(m_strLog, strLog);
	UpdateLogBox();
}

void CObjectDlg::UpdateLogBox()
{
	int index =m_LogBox.AddString(m_strLog);
	m_LogBox.UpdateWindow();
	m_LogBox.SetTopIndex(index);
	ClearLogBuffer();
}

//Graphic instance가 있는 오브젝트에 대해서만 추가 할 것.
void CObjectDlg::addObjectInCombobox(object* pObj)
{
	if (pObj == nullptr) return;

	int idx = m_objListBox.AddString(pObj->objectName);
	m_objListBox.SetItemDataPtr(idx, (void*)pObj);

	m_objListBox.UpdateWindow();
}


//선택된 오브젝트 값 수정
void CObjectDlg::OnBnClickedBtnObjedit()
{
	object* pObj = GetSelectedObj();
	object* pTemp = new object;
	size_t size = g_pCenter->GetNumberOfSelectedObj();
	if (size == 1)
	{
		GetObjectDataFromEditBox(pTemp);
		HGMessage* pMsg = new HGMessage;
		pMsg->msg = WM_OBJECT_EDIT;
		pMsg->objType = eObjectType::eObject;
		pMsg->instanceType = eInstanceType::eCpuInstance;
		pMsg->numOfInstance = 1;							//2개 이상일 경우 LPARAM을 쓰는 것을 원칙으로 하자.
		pMsg->pObj0 = (void*)(pObj);
		pMsg->pObj1 = (void*)(pTemp);

		bool bRespond = ::SendMessageW(g_hCenter, WM_OBJECT_EDIT, (WPARAM)pMsg, NULL);
		if (bRespond == true)
		{
			UpdateEditBoxesByObject(pObj);
		}
		
	}
	else
	{
		OutputDebugString(L"[수정버튼]\n 이 버튼은 하나의 오브젝트를 수정할는데 사용됩니다.");
	}
	
	OnPaint();
	//NULL일 경우 처리 추가.
}

//기존 오브젝트를 삭제.
void CObjectDlg::OnBnClickedBtnObjdel()
{
	object* pTemp = GetSelectedObj();
	if (pTemp == NULL) return;
	
	DeleteGraphicInstance(pTemp);

	int index = m_objListBox.GetCurSel();
	index -= 1;
	if (index > -1)
	{
		SetIndex(index);
	}

	//로그 박스로 수정.
	//wchar_t buf[256];
	//wsprintfW(buf, L"[DELETE OBJECT] %s", pTemp->objectName);
	//AddLogStr(buf);
}

void CObjectDlg::OnBnClickedBtnObjcreate()
{
	XMFLOAT3 pos	= { 0.f, 0.f, 0.f };
	XMFLOAT3 oiler  = { 0.f, 0.f, 0.f };
	XMFLOAT4 quaternion = { 0.f, 0.f, 0.f, 1.f  };
	XMFLOAT3 scale	= { 1.f, 1.f, 1.f };
	int iModelIdx	= GetSelectedModelIndex();
	int iMatIdx		= GetSelectedMaterialIndex();
	
	//생성
	object* pObj = new object;
	ZeroMemory(pObj, sizeof(object));

	//리젠 콜라이더박스 인덱스 셋팅.
	SetRegenIndexFromComboBox(pObj);

	//TRS
	GetPos(pos);
	GetQuaternion(quaternion);			//쿼터니언 값 입력
	GetScale(scale);

	//모델 이름&인덱스
	CStringW modelName;
	MODEL* pModel = g_pCenter->GetModelByInde(iModelIdx);
	modelName = CStringW(pModel->fileName.c_str());
	
	//메테리얼 이름&인덱스
	CStringW matName;
	MATERIAL* pMat = g_pCenter->GetMaterialByIndex(iMatIdx);
	if(pMat != NULL)
		matName = CStringW(pMat->matName);
	
	//에딧 박스로부터 값을 받아오는 과정.
	SetObjTypeFromComboBox(pObj);
	//SetObjIsCollision(pObj);
	
	pObj->SetPos(pos);
	pObj->SetRot(quaternion);
	pObj->SetScale(scale);
	pObj->modelIndex	= iModelIdx;
	pObj->matIndex		= iMatIdx;
	
	wsprintfW(pObj->modelName, modelName.GetBuffer());
	SetDefaultName(pObj);
	wsprintfW(pObj->matName, matName.GetBuffer());


	//clear temp string buffer
	modelName.Empty();
	matName.Empty();
	modelName.ReleaseBuffer();
	matName.ReleaseBuffer();

	
	//생성
	//메세지 생성 함수화?
	HGMessage* pMsgCreateObj = new HGMessage;
	pMsgCreateObj->msg = WM_OBJECT_CREATE;
	pMsgCreateObj->objType = eObjectType::eObject;
	pMsgCreateObj->numOfInstance = 1;
	pMsgCreateObj->instanceType = eInstanceType::eCpuInstance;

	MsgObjectArray* pMsgObjArray = new MsgObjectArray;
	pMsgObjArray->count = 1;
	pMsgObjArray->objectType = eObjectType::eObject;
	pMsgObjArray->pArray = (void*)pObj;

	eMsgResult msgResult = (eMsgResult)::SendMessageW(g_hCenter, WM_OBJECT_CREATE, (WPARAM)pMsgCreateObj, (LPARAM)pMsgObjArray);


	if (msgResult == eMsgResult::eMsgSuccess)
	{
		SetObjBoxIndex(pObj);
		addObjectInCombobox(pObj);
		UpdateEditBoxesByObject(pObj);
	}
	else
	{
		delete pObj;
		pObj = nullptr;
		OutputDebugStringW(L"[FAIL]ObjectDlg::OnBnClickedBtnObjcreate(), Fail to Create Object\n");
		assert(false);
	}

}

void CObjectDlg::OnSelchangeObjlist()
{
	//리스트 박스에 오브젝트를 선택했을 경우
	object* pTemp = GetSelectedObj();
	UpdateEditBoxesByObject(pTemp);
	
	if (pTemp != NULL)
	{
		g_pCenter->SelectChangeObject(pTemp);
		//g_pCenter->SetGizmo(pTemp);
	}
}


void CObjectDlg::OnBnClickedBtnSave()
{
	
	CStringW fileName;
	CreateFolder(g_SaveLoadPath);				//폴더가 없을 경우 생성.
	GetMapNameFromDlg(IDC_FILE_NAME, fileName);	//저장할 파일 이름/패스 받아오기	

	::SendMessageW(g_hCenter, WM_SAVE_MAP, (WPARAM)(m_hWnd), (LPARAM)(L"TEST"));

	fileName.Empty();

	ClearFileName();
}

void CObjectDlg::OnBnClickedBtnLoad()
{
	CStringW fileName;

	GetMapNameFromDlg(IDC_FILE_NAME, fileName);

	::SendMessageW(g_hCenter, WM_LOAD_MAP, (WPARAM)(L"TEST_OnBnClickedBtnLoad()"), NULL);

	ClearFileName();

}

void CObjectDlg::OnKillfocusPosX()
{
	float f1 = CheckInput_Float(IDC_POS_X);

}

void CObjectDlg::OnKillfocusPosY()
{
	float f1 = CheckInput_Float(IDC_POS_Y);

}

void CObjectDlg::OnKillfocusPosZ()
{
	float f1 = CheckInput_Float(IDC_POS_Z);
}

void CObjectDlg::OnKillfocusRotX()
{
	float f1 = CheckInput_Float(IDC_ROT_X);
}

void CObjectDlg::OnKillfocusRotY()
{
	float f1 = CheckInput_Float(IDC_ROT_Y);

}

void CObjectDlg::OnKillfocusRotZ()
{
	float f1 = CheckInput_Float(IDC_ROT_Z);
}

void CObjectDlg::OnKillfocusScaleX()
{
	float f1 = CheckInput_Float(IDC_SCALE_X);


}

void CObjectDlg::OnKillfocusScaleY()
{
	float f1 = CheckInput_Float(IDC_SCALE_Y);

}

void CObjectDlg::OnKillfocusScaleZ()
{
	float f1 = CheckInput_Float(IDC_SCALE_Z);

	int index = GetIndex();
	if (index < 0)
		return;
	object* pObj = GetObjectByIndex(index);
	GetObjectDataFromEditBox(pObj);
}

void CObjectDlg::OnKillfocusMoveOffset()
{
	//킬포커스를 쓰는 경우 다른 컨트롤을 클릭하여 포커스가 넘어가야 진입.
	float f1 = CheckInput_Float(IDC_MOVE_OFFSET);
	m_fMovingOffset = f1;
}

void CObjectDlg::OnLbnSelchangeModellist()
{
	//MapUtil::PassFocus(m_hWndViewer);
	
	int iSel = m_ModelListBox.GetCurSel();
	if (iSel != LB_ERR)
	{
		CString ItemSelected;
		m_ModelListBox.GetText(iSel, ItemSelected);
		MODEL* pModel = GetModelFromListBox(iSel);
		
		SetDlgItemText(IDC_MODEL_NAME, ItemSelected);
		SetDlgItemInt(IDC_MODEL_INDEX, pModel->index, TRUE);		//마지막 TRUE는 부호가 있다는 뜻
	}

	OnPaint();
}

void CObjectDlg::OnLbnSelchangeMatlist()
{
	//MapUtil::PassFocus(m_hWndViewer);
	int iSel = m_MatListBox.GetCurSel();
	if (iSel != LB_ERR)
	{
		CString ItemSelected;
		m_MatListBox.GetText(iSel, ItemSelected);
		MATERIAL* pMat = GetMaterialListBox(iSel);
		
		SetDlgItemText(IDC_MAT_NAME, ItemSelected);
		SetDlgItemInt(IDC_MAT_INDEX, pMat->index, TRUE);		//마지막 TRUE는 부호가 있다는 뜻
	}

	OnPaint();
}

BOOL CObjectDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM wp = pMsg->wParam;
	if (pMsg->message == WM_KEYUP)
	{
		if (wp == VK_CONTROL)
		{
			::SendMessageW(g_hCenter, WM_KEYUP, pMsg->wParam, pMsg->lParam);
		}
		if (wp == VK_TAB)
		{
			if (!m_bShiftKeyDown)
				MoveFocusNext();
			else
				MoveFocusBack();
		}
	}

	return 0;
	//return CDialogEx::PreTranslateMessage(pMsg);
}


afx_msg LRESULT CObjectDlg::OnUpdateObjectList(WPARAM wParam, LPARAM lParam)
{
	std::vector<DRAW_INSTANCE*>* pList = (std::vector<DRAW_INSTANCE*>*)(wParam);
	UpdateObjListBox(pList);
	object* pObj = NULL;

	return 0;
}


afx_msg LRESULT CObjectDlg::OnUpdateModelList(WPARAM wParam, LPARAM lParam)
{
	std::vector<MODEL*>* pList = (std::vector<MODEL*>*)wParam;
	UpdateModelListBox(pList);
	return 0;
}


afx_msg LRESULT CObjectDlg::OnUpdateMatList(WPARAM wParam, LPARAM lParam)
{
	std::vector<MATERIAL*>* pList = (std::vector<MATERIAL*>*)wParam;
	UpdateMatListBox(pList);
	return 0;
}


afx_msg LRESULT CObjectDlg::OnListBoxNone(WPARAM wParam, LPARAM lParam)
{
	UpdateEditBoxesByObject(NULL);

	m_objListBox.SetCurSel(-1);
	m_MatListBox.SetCurSel(-1);
	m_ModelListBox.SetCurSel(-1);

	return 0;
}



void CObjectDlg::OnBnClickedButtonEditMatSelected()
{
	int index = m_MatListBox.GetCurSel();
	if (index > -1)
		g_pCenter->EditMaterialSelectedALL(index);
	else
		OutputDebugString(L"[OnBnClickedButtonEditMatSelected]Worng MATERIAL index\n");
}



void CObjectDlg::OnEnChangePosX()
{
	
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_SHIFT)
		m_bShiftKeyDown = true;
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CObjectDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		m_bShiftKeyDown = false;
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CObjectDlg::OnCbnSelchangeComboObjtype()
{
	int index = m_ComboBox_ObjectType.GetCurSel();
	eOBJECT_TYPE type = (eOBJECT_TYPE)m_ComboBox_ObjectType.GetItemData(index);

	if (type == eOBJECT_FIELD_COLLISION_NO)
	{
		SetRegenComboBoxNONE();
		//m_ComboBox_RegenColliderList.SetCurSel(-1);
		m_ComboBox_RegenColliderList.EnableWindow(FALSE);
	}
	else if (type == eOBJECT_FIELD_COLLISION_YES)
	{
		SetRegenComboBoxNONE();
		//m_ComboBox_RegenColliderList.SetCurSel(-1);
		m_ComboBox_RegenColliderList.EnableWindow(FALSE);
	}
	else
	{
		m_ComboBox_RegenColliderList.EnableWindow();
	}
}


void CObjectDlg::OnClose()
{
	//여기부터.
	if (m_objListBox.m_hWnd != NULL)
	{
		int num = m_objListBox.GetCount();
		for (int i = 0; i < num; i++)
		{
			m_objListBox.DeleteString(i);
		}

	}

	if (m_ModelListBox.m_hWnd != NULL)
	{
		int num = m_ModelListBox.GetCount();
		for (int i = 0; i < num; i++)
		{
			m_ModelListBox.DeleteString(i);
		}
	}

	if (m_MatListBox.m_hWnd != NULL)
	{
		int num = m_MatListBox.GetCount();
		for (int i = 0; i < num; i++)
		{
			m_MatListBox.DeleteString(i);
		}
	}

	CDialogEx::OnClose();
}
