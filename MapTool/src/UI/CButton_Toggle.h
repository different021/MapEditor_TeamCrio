#pragma once
#include <Windows.h>
#include "../../UIDesigner/src/CUIManager.h"


//마우스 상태에 따른
//슬라이드(게이지바), 토글, 레디얼 메뉴, 강화 스테이션(슬라이드)



class CMyButton
{
protected:
	bool			m_bUse;
	bool			m_bVisible;			//버튼을 보이게 할 것인가?
	eACTIVE_STATE	m_eActive;			//버튼 사용 가능한 상태
	eBUTTON_TYPE	m_eType;
	eBUTTON_EVENT	m_eEventState;		//현재의 마우스 상태. out/in/on/up/down

	INT_VECTOR2		m_MousePos[eMOUSE_POS_MAX];	//마우스 위치.

	int	 m_iScreenWidth  = 0;			//창 가로 1920
	int	 m_iScreenHeight = 0;			//창 세로 1080	

	IMAGE* m_pImgList[eEVENT_MAX];		// in/out/LBTN_UP/LBTN_DOWN/RBTN_UP/LBTN_DOWN/on

public:
	CMyButton(eBUTTON_TYPE type);
	void Delete();
	virtual void Update();

	//이미지 이동
	void MoveByDeltaRatio(float deltaX, float deltaY);
	void CalculateByRatio();								//CMyButton으로 이동

	void SetType(eBUTTON_TYPE type);
	void SetActive(eACTIVE_STATE act);
	void SetVisible(bool bIsVisible);
	void SetVisible(eBUTTON_EVENT btnEvent, bool bVisible);

	eBUTTON_TYPE  GetButtonType();
	eACTIVE_STATE GetActive();

	void	SetWindowSize(int width, int height);	//제작의 기준이 된 윈도우 사이즈
	void	SetCurMousePos(int x, int y);			//현재 마우스 포인트 좌표 
	void	SetClickedPos(int x, int y);
	
	XMFLOAT2 GetRatio(int x, int y);				//좌표 -> 비율	(윈도우 사이즈가 필요한 이유)
	

	//Image
	void   SetImg(eBUTTON_EVENT btnEvent, IMAGE* pImg);
	IMAGE* GetImg(eBUTTON_EVENT state);
	IMAGE* GetImg();
	IMAGE* ShowMeTheImage(eBUTTON_EVENT eEvent);

	//선택.
	IMAGE* Picking(int x, int y);							//CMyButton으로 이동
	bool   Picking(int x, int y, eBUTTON_EVENT state);		//CMyButton으로 이동

	//상태변화.	-> 변화 예정.
	virtual void OnEvent(eBUTTON_EVENT btnEvent, int x, int y);
	virtual void OnMouseMove(int x, int y);


	void ProcessMessage(UINT message, WPARAM wpram, LPARAM lPram);
};




class CButton_Toggle : public CMyButton
{
	bool m_bValue;		//토글 투르 펄스 값.

private:
	~CButton_Toggle();

public:
	CButton_Toggle(eBUTTON_TYPE type);

	bool	GetValue();
	//bool	Toggle();

};


