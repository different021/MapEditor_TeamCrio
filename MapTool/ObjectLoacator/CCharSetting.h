#pragma once
#include <vector>
#include "../src/Defines.h"

// CCharSetting 대화 상자

struct MODEL;
struct MATERIAL;

class CCharSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CCharSetting)
private:
	HWND m_hCenter;

	std::vector<HTREEITEM> m_TreeItems;
	CTreeCtrl m_TreeChar;
	CListBox m_ModelListBox;
	CListBox m_MaterialListBox;

public:
	CCharSetting(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCharSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAR_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void RequestList();

private:
	void GetCharInfo(CHARACTER* pChar);
	void SetDefaultName(CHARACTER* pChar);
	int GetDlgInt(int ID);
	void GetDlgInt(int ID, int& dest);
	float GetDlgFloat(int ID);

	void InitTree();
	void UpdateModelListBox(__in std::vector<MODEL*>* pModelList);
	void UpdateMatListBox(__in std::vector<MATERIAL*>* pMatList);
	void UpdateCharTreeBox();

	MODEL* GetModelByIndex(int index);
	MATERIAL* GetMatByIndex(int index);
	void SetText(int ID, CStringA& str);
	void SetInt(int ID, int index);

	void GetText(int ID, WCHAR* pDest);
	void GetInt(int ID, int& dest);

	void Request_ModelList();
	void Request_MatList();
	
public:
	// 야홍~
	void SetCenter(HWND hCenter);
	void Move(int x, int y);
	afx_msg void OnBnClickedCharCreate();
	afx_msg void OnBnClickedCharDelete();
	afx_msg void OnLbnSelchangeListModel();
	afx_msg void OnLbnSelchangeListMaterial();
	afx_msg void OnTvnSelchangedTreeChar(NMHDR* pNMHDR, LRESULT* pResult);

public:
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT OnUpdateModelList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMatList(WPARAM wParam, LPARAM lParam);
};
