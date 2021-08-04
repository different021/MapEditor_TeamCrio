
// ObjectLoacatorDlg.h: 헤더 파일
#pragma once
#include <vector>
#include <utility>

#include "Defines.h"
#include "ViewDlg.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "../src/utility/CTextReader.h"

class SaveDlg;
class Camera;
class CCallbackTimer;
class CFileDialog;
class CGrid;
class CColliderManager;

static const int STR_MAX = 512;

// CObjectLoacatorDlg 대화 상자
class CObjectDlg : public CDialogEx
{
private:
	bool m_bIsShow		 = true;
	bool m_bIsDocking	 = true;
	bool m_bShiftKeyDown = false;

	//카메라 관련
	std::vector<Camera*> m_pCamList;

	CListBox m_objListBox;			//오브젝트 리스트 박스
	CListBox m_ModelListBox;		//모델 리스트 박스
	CListBox m_MatListBox;			//마테리얼 리스트 박스
	CListBox m_LogBox;				//사용자가 입력한 행위들이 로그로 기록된다.
	CComboBox m_ComboBox_ObjectType;
	CComboBox m_ComboBox_RegenColliderList;		//콜라이젼용

	float m_fMovingOffset = 10.f;
	wchar_t m_strLog[STR_MAX];

	UINT m_curFocus;
public:
	CObjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CObjectDlg();


	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTLOACATOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	INT_PTR GetNamePath(CStringW& outName, CStringW& outPath);		//이동 ->CSaver 함수 형식 변경.
	float CheckInput_Float(int ctrller);			//edit 박스에서 받은 입력 값이 float인지 확인하고 아닐 경우 0을 리턴한다.
	void SetDlgFloat(int controllerID, float value);
	void GetStringFromIDC(int controllerID, CString& dest);
	void SetObjName(int controllerID, object* pDest);
	void SetObjMaterialName(int controllerID, object* pDest);

	void MoveFocusNext();
	void MoveFocusBack();
	void InitComboBox_ObjectType();
	//void InitComboBox_IsCollision();

	void SetObjSelectedModel(object* pObj);
	void SetObjSelectedMat(object* pObj);
	void SetDefaultName(object* pObj);
	void SetModelName(object* pDest, int modelIndex);
								
	object* GetSelectedObj();
	object* GetObjectByIndex(int index);

	int GetSelectedModelIndex();
	int GetSelectedMaterialIndex();
	MODEL* GetModelFromListBox(int index);
	MATERIAL* GetMaterialListBox(int index);

	void DeleteGraphicInstance(object* pObj);

	DirectX::XMFLOAT4 ConvertMatrixToQuaternion(DirectX::XMFLOAT3& inRadian);
	DirectX::XMFLOAT4X4 ConvertQuaternionToMatrix(DirectX::XMFLOAT4& inRadian);

	//콘트롤러 수치Get/Set/Update
public:
	//생성 및 코기화
	static CObjectDlg* CreateObjectDlg(CWnd* pParent);
	void Initialize(CWnd* pParent);
	void SetDocking(CWnd* pParent);									//부모 연결.

	void UpdateObjListBox(std::vector<DRAW_INSTANCE*>* pList);									//dlg박스의 다이얼로그 인스턴스 업데이트
	void UpdateModelListBox(std::vector<MODEL*>* pModelList);
	void UpdateMatListBox(std::vector<MATERIAL*>* pMatList);
	
	//받은 리스트내에 오브젝트 삭제.
	void DeleteObjInListBox(std::vector<object*>* pList);

private:
	void GetVector(UINT id, DirectX::XMFLOAT3& dest);
	void GetPos(DirectX::XMFLOAT3& pos);
	void GetOiler(DirectX::XMFLOAT3& oil);
	void GetQuaternion(DirectX::XMFLOAT4& rot);
	void GetScale(DirectX::XMFLOAT3& scale);
	float GetValueFromDlg(int id, float defaultValue);
	eOBJECT_TYPE SetObjTypeFromComboBox(object* pDest);
	//eIsCollision SetObjIsCollision(object* pDest);

	void SetComboBoxType(object* pObj);
	//void SetComboBoxIsCollision(object* pObj);
	void SetPosEditBoxText(float x, float y, float z);
	void SetPosEditBoxText(DirectX::XMFLOAT3& pos);
	void SetRotEditBoxText(DirectX::XMFLOAT3& rot);
	void SetScaleEditBoxText(float x, float y, float z);
	void SetScaleEditBoxText(DirectX::XMFLOAT3& pos);
	void SetNameEditBoxText(object* pObj);
	void SetNameEditBoxText(WCHAR* pStr);
	void SetModelBoxIndex(object* pObj);
	void SetMatBoxIndex(object* pObj);
	void SetObjBoxIndex(object* pObj);
	
	void SetModelIndexEditBox(int index);			//수정권한이 없는 에딧 박스들. 
	void SetModelNameEditBox(CString strModelName);
	void SetMatIndexEditbox(int index);
	void SetMatNameEditBox(CString strMatName);
	

	void ClearFileName();


public:
	//리젠 콜라이더 관련.
	void UpdateRegenColiderList(const std::vector<COLLIDER>* pColList);
	void SetRegenComboBoxNONE();
	void SetRegenColliderComboBox(int index);
	void SetRegenIndexFromComboBox(object* pDest);
	collider* GetRegenColliderFromCombo();
	void SetObjRegenIndexByComboBox(object* pDest);

public:
	void SetObjectEditBoxes(object* pObj);

private:
	void SetIndex(int index);
	int	 GetIndex();

	//콘트롤 초기화
	void GetMapNameFromDlg(int id, CString& dest);
	void CreateFolder(CStringW& folderName);

	void OutputDebugCode(wchar_t* pStr);
	void RequestModelList();
	void RequestMatList();

public:
	void EditObject(object* pObj);


	void HideAndShow();					//창 컨트롤
	bool IsDocking();
	void ToggleDocking_();

	void ClearLogBuffer();
	void AddLogStr(wchar_t* strLog);
	void AddLogTest(LPCWSTR pStr, ...);
	void OutCurDirAfterStr(wchar_t* strLog);
	void UpdateLogBox();

public:
	afx_msg void OnBnClickedBtnObjedit();				// 수정 버튼
	afx_msg void OnBnClickedBtnObjdel();				// 삭제 버튼
	afx_msg void OnBnClickedBtnObjcreate();				// 생성 버튼
	afx_msg void OnSelchangeObjlist();					//ObjectListBox를 선택하였다.
	
	//이동  사유: 세이브 기능 ObjManager로 이관
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();

protected:
	afx_msg void OnKillfocusPosX();
	afx_msg void OnKillfocusPosY();
	afx_msg void OnKillfocusPosZ();
	afx_msg void OnKillfocusRotX();
	afx_msg void OnKillfocusRotY();
	afx_msg void OnKillfocusRotZ();
	afx_msg void OnKillfocusScaleX();
	afx_msg void OnKillfocusScaleY();
	afx_msg void OnKillfocusScaleZ();
	afx_msg void OnKillfocusMoveOffset();
	afx_msg void OnLbnSelchangeModellist();
	afx_msg void OnLbnSelchangeMatlist();

	BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnUpdateObjectList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateModelList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMatList(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnListBoxNone(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnBnClickedButtonEditMatSelected();
	afx_msg void OnEnChangePosX();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnCbnSelchangeComboObjtype();
	afx_msg void OnClose();
};

extern HINSTANCE g_hIns; 
