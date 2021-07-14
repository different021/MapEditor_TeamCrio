#pragma once
#include <vector>
#include "CButton_Toggle.h"
#include "CButton_Slide.h"

struct IMAGE;
class  HSpriteData;


class CButtonManager
{
	static CButtonManager*  m_pInstance;
	int	 m_iScreenWidth		= 0;			//창 가로 1920
	int	 m_iScreenHeight	= 0;			//창 세로 1080	
	std::vector<CMyButton*> m_ButtonList;	//전체 리스트.
	std::vector<CMyButton*> m_SelectedList;	//선택된 리스트

	CButtonManager();

public:
	~CButtonManager();
	static  CButtonManager* GetInstance();
	void    CleanUp();

	void	Update();
	void	Calculate();
	void	SetWidthHeight(int width, int height);
	int		GetWidth();
	int		GetHeight();
	void	GetRatio(float &outRatioWidth, float &outRatioHeight, float width, float height);
	void	AddButton(CMyButton* pBtn);
	void	DeleteButton(CMyButton* pBtn);
	
	size_t  GetSize();
	
	IMAGE* GetLastSelectedImg();
	std::vector<CMyButton*>* GetSelectedList();
	void   DeleteImg(IMAGE* pImg);
	
	CMyButton* GetMousePos(int x, int y);
	CMyButton* CreateButton(eBUTTON_TYPE type, IMAGE* pImg);
	CMyButton* CheckClick(int x, int y);
	
	void ToggleSelected();

	virtual void OnEvent(eBUTTON_EVENT btnEvent, int x, int y);

//	LRESULT CALLBACK ProcessButtonWndProc(HWND hWnd, UINT Msg, WPARAM  wParam, LPARAM  lParam);
//
//private:
//	static LRESULT CALLBACK ProcessButtonWndProc(HWND hWnd, UINT Msg, WPARAM  wParam, LPARAM  lParam);
};

