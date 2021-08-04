#pragma once

// CWaveDlg 대화 상자
class WAVE;

class CWaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaveDlg)

public:
	CListBox m_WaveListBox;

public:
	CWaveDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CWaveDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAVE_DLG };
#endif

public:
	//초기화 및 삭제
	static CWaveDlg* CreateWaveDlg(CWnd* pParent);
	void Initialize(CWnd* pParent);
	void SetDocking(CWnd* pParent);
	void Cleanup();

	//Edit Control 관련
	void GetDataFromEditCtrl(waveData* pDest);
	void SetDataToEditCtrl(waveData* pSrc);
	void SetDataToEditCtrlByWave(WAVE* pSrc);

	//ListBox 관련
	void UpdateWaveListBox(const std::vector<WAVE*>* pList);	//리스트 전체 업데이트.
	void AddWaveTolistBox(WAVE* pWave);							//하나의 웨이브 추가
	void ResetWaveListBox();

	//웨이브 선택
	WAVE* GetCurSelectedWaveInListBox();						//리스트 박스에서 현재 선택된 WAVE구하기.
	void SetWaveListBoxCurSel(WAVE* pWave);						//해당 웨이브를 찾아서 리스트박스에 선택해준다.

	//웨이브 삭제.
	void DeleteWaveInListBox(WAVE* pWave);						//리스트 박스에 웨이브를 지우고, 메니저에 삭제 요청.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWaveCreate();		//Create
	afx_msg void OnBnClickedWaveEdit();			//Edit
	afx_msg void OnBnClickedWaveDelete();		//Delete

	afx_msg void OnLbnSelchangeWaveList();		//리스트에서 선택
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
};
