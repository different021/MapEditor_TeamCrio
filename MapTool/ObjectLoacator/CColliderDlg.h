#pragma once
#include <directxmath.h>
#include <vector>
#include "Defines.h"
#include "CColliderManager.h"

// CColliderControllerDlg 대화 상자
struct collider;

class CColliderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderDlg)
	bool m_bIsShiftDown;
	int  m_curFocus;
	int  m_ColliderHighestIndex = 0;
	CListBox m_ColliderListBox;
	CComboBox m_ColliderTypeBox;

private:
	void MoveFocusNext();	//에딧컨트롤 조작
	void MoveFocusBack();

public:
	CColliderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderDlg();
	
	static CColliderDlg* CreateColliderDlg(CWnd* pParent);
	void Initialize(CWnd* pParent);
	void SetDocking(CWnd* pParent);

	void Move(int x, int y);
	void AddColliderToListBox(collider* pCollider);
	void SetEditBoxByCollider(collider* pCollider);
	void SetEditBoxZero();
	void SetColliderByEditControl(collider* pCollider);
	void CreateCollider(collider* pSrc = NULL);
	int  GetColliderListBoxIndex(collider* pCollider);

	void SetColliderIndexByEditControl(collider* pDest);
	void SetColliderRot(collider* pDest, DirectX::XMFLOAT3& oiler);
	void SelectCollider(collider* pCollider);

	//ColliderBox 관련
	void UpdateColliderList(std::vector<COLLIDER>* pList);

	//ColliderType
	eCOLLIDER_TYPE GetColliderTypeByComboIndex();
	int GetComboIndexByColliderType(eCOLLIDER_TYPE type);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER_CONTROLLER };
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg LRESULT OnCreateCollider(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnUpdateColliderList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearColliderList(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnBnClickedCreateCollider();			//생성
	afx_msg void OnBnClickedDeleteCollider();			//삭제
	afx_msg void OnBnClickedEditCollider();				//수정
	afx_msg void OnLbnSelchangeColliderListbox();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnClose();
};
