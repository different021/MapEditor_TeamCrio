#pragma once

class CObjectLoacatorDlg;
class Viewer;

class CGrid : public CDialogEx
{
	DECLARE_DYNAMIC(CGrid)
public:
	int m_iWidth    = iDEFAULTGRID_W;
	int m_iHeight   = iDEFAULTGRID_H;
	float m_fOffset = fDEFAULTOFFSET;

	bool  m_bIsShiftDown;
	int   m_curFocus;

public:
	CGrid(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGrid();

	float GetDlgItemFloat(int ID);				//EditControl값 받기 시리즈
	int  GetDlgItemInt(int ID);
	void SetDlgItemFloat(int ID, float fInput);
	void SetDlgItemInt(int iD, int iValue);
	void UpdateEditBox();						//editBOx값 셋팅	

	void Move(int x, int y);
public:
	int GetMapWidth();
	int GetMapHeight();
	float GetMapOffset();
	void SetGrid(int width, int height, float offset);
	void AnnounceChangeGrid();
	void EditGrid(int width, int height, float offset);

private:
	void MoveFocusNext();
	void MoveFocusBack();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRIDCON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
