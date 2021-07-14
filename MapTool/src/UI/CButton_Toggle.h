#pragma once
#include <Windows.h>
#include "../../UIDesigner/src/CUIManager.h"


//���콺 ���¿� ����
//�����̵�(��������), ���, ����� �޴�, ��ȭ �����̼�(�����̵�)



class CMyButton
{
protected:
	bool			m_bUse;
	bool			m_bVisible;			//��ư�� ���̰� �� ���ΰ�?
	eACTIVE_STATE	m_eActive;			//��ư ��� ������ ����
	eBUTTON_TYPE	m_eType;
	eBUTTON_EVENT	m_eEventState;		//������ ���콺 ����. out/in/on/up/down

	INT_VECTOR2		m_MousePos[eMOUSE_POS_MAX];	//���콺 ��ġ.

	int	 m_iScreenWidth  = 0;			//â ���� 1920
	int	 m_iScreenHeight = 0;			//â ���� 1080	

	IMAGE* m_pImgList[eEVENT_MAX];		// in/out/LBTN_UP/LBTN_DOWN/RBTN_UP/LBTN_DOWN/on

public:
	CMyButton(eBUTTON_TYPE type);
	void Delete();
	virtual void Update();

	//�̹��� �̵�
	void MoveByDeltaRatio(float deltaX, float deltaY);
	void CalculateByRatio();								//CMyButton���� �̵�

	void SetType(eBUTTON_TYPE type);
	void SetActive(eACTIVE_STATE act);
	void SetVisible(bool bIsVisible);
	void SetVisible(eBUTTON_EVENT btnEvent, bool bVisible);

	eBUTTON_TYPE  GetButtonType();
	eACTIVE_STATE GetActive();

	void	SetWindowSize(int width, int height);	//������ ������ �� ������ ������
	void	SetCurMousePos(int x, int y);			//���� ���콺 ����Ʈ ��ǥ 
	void	SetClickedPos(int x, int y);
	
	XMFLOAT2 GetRatio(int x, int y);				//��ǥ -> ����	(������ ����� �ʿ��� ����)
	

	//Image
	void   SetImg(eBUTTON_EVENT btnEvent, IMAGE* pImg);
	IMAGE* GetImg(eBUTTON_EVENT state);
	IMAGE* GetImg();
	IMAGE* ShowMeTheImage(eBUTTON_EVENT eEvent);

	//����.
	IMAGE* Picking(int x, int y);							//CMyButton���� �̵�
	bool   Picking(int x, int y, eBUTTON_EVENT state);		//CMyButton���� �̵�

	//���º�ȭ.	-> ��ȭ ����.
	virtual void OnEvent(eBUTTON_EVENT btnEvent, int x, int y);
	virtual void OnMouseMove(int x, int y);


	void ProcessMessage(UINT message, WPARAM wpram, LPARAM lPram);
};




class CButton_Toggle : public CMyButton
{
	bool m_bValue;		//��� ���� �޽� ��.

private:
	~CButton_Toggle();

public:
	CButton_Toggle(eBUTTON_TYPE type);

	bool	GetValue();
	//bool	Toggle();

};


