#pragma once
#include <string>

// CCmd 대화 상자

class CCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CCmd)
public:
	CEdit m_CmdInputLine;	//CommandLine
	WCHAR m_LogBuffer[256];
	std::string m_Buffer;

public:
	CCmd(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCmd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Move(int x, int y);
	void MoveRigthBottom(int x, int y);
	void WriteLog(WCHAR* pStr);
	void WriteLog(int i);

	template <typename T, typename... Types>
	void WriteLog(T arg, Types... args);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


