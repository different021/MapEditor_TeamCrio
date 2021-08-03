﻿#pragma once
#include "../src/Defines.h"

struct lightData;
class  CLightManager;
struct Light;

class CLIGHTDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CLIGHTDLG)
	bool m_bShiftKeyDown;
	UINT m_curFocus;
	CComboBox* m_pLightType;
	CLightManager* m_pLightManager;

private:
	void MoveFocusNext();
	void MoveFocusBack();

public:
	CLIGHTDLG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLIGHTDLG();
	void LightDlgCleanup();

	void SetDlgFloat(UINT uId, float data);

	void SetEditBox(Light* pLight);
	void GetDataFromControl(lightData& pDest);
	void GetDataFromEditBox(Light* pDest);
	eLightType GetSelectedType();

	void SetEditBoxActive(eLightType type);
	void AllOn();
	void DirectionInputOFF();
	void PointInputOFF();
	void SpotInputOFF();

	int  AddLightToListBox(Light* pLight);
	int  SetSelIndexInListBox(Light* pLight);

	void UpdateLightList();
	void ClearLightList();

	//에딧박스 업데이트
	void UpdateSelectedLight();
	void UpdateTypeByLight(lightData* pData);
	void DeleteLightInListBox(std::vector<Light*>* pList);
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLightType();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLbnSelchangeListLight();

	afx_msg void OnClose();
};