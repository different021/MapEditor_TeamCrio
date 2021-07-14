#pragma once
#include "EngineInterface.h"

namespace HG_UI
{
	static const int BUFFERMAX = 256;
	enum eAxis
	{
		eAxisX = 0,
		eAxisY,
		eAxisZ,
		eAxis_MAX,
	};

	struct UI_HEADER
	{
		int screenWidth;			//ȭ�� ���� ũ��
		int screenHeight;			//ȭ�� ���� ũ��
		int numOfUI_INFO;			//IMG_INFO �� ����
		int numOfChildIndexBuffer;	//childIndexBuffer ũ��
		char UI_NAME[BUFFERMAX];
	};


	//�ϳ��� ��������Ʈ�� ���� ����.
	/*
		�ش� UI�̹��������� ������ ����ƽ�ϰ� ����.
		�������� ���ӿ��� ����� ������.
	*/
	struct UI_INFO
	{
		int   index;					//�ε����� �����ؾ� �Ѵ�.
		int	  parentIndex;				//�θ��� �ε���

		int	  AbsolutePos[eAxis_MAX];	//���� ��ǥ(int)	
		int   AbsoluteWidth;			//���� ��ǥ	
		int   AbsoluteHeight;			//���� ��ǥ	

		float RatioPos[eAxis_MAX];		//����� ��ǥ(float)
		float RatioWidth;				//
		float RatioHeight;				//
		float opacity;					//���� -> ����� ��.
		bool  bIsAbsolutePosition;		//false ȭ���� ���� -> �������� false�� ���.(�����θ�)
		
		int   numOfChild;				//�ڽ��� ���� �ִ� �ڽļ�.
		int   startChildIndex;			//���ϵ� �ε��� ���ۿ��� ������ ������ �ε��� �ڽ� ���ڸ�ŭ ����.
		int   rootParent;				//���� ��Ʈ��带 ���� ����.

		int	  screenWidth;				//��ũ�� �ʺ�
		int   screenHeight;				//��ũ�� ����

		WCHAR NameOfSprite[BUFFERMAX];	//��������Ʈ �̸�.
		WCHAR NameOfUI[BUFFERMAX];		//�� UI�̸�.	-> ����?
	};


	enum eBUTTON_EVENT			//���콺 ���� �̺�Ʈ
	{
		eEVENT_DEFAULT = 0,		//�⺻ ����
		eEVENT_INACTIVE,		//��ư ��� �Ұ� ����
		eEVENT_MOUSEIN,			//���콺�� ������ ���� ����
		eEVENT_MOUSEOUT,		//���콺�� ������ �������� ����
		eEVENT_MOUSEON,			//���콺�� �ö��ִ� ����.
		eEVENT_LBUTTONUP,		//�� ��ư UP
		eEVENT_LBUTTONDOWN,		//�� ��ư DOWN
		eEVENT_RBUTTONUP,		//���� ��ư UP
		eEVENT_RBUTTONDOWN,		//���� ��ư DOWN
		
		eEVENT_MAX,
	};



	enum eBUTTON_TYPE
	{
		eBUTTON_NORMAL = 0,		//�⺻ ��ư
		eBUTTON_TOGGLE,			//������� ���°� �ٲ��.
		eBUTTON_SLIDE,			//�����̵�ٸ� �̿��Ͽ� ���� ���� �� �� �ִ�.
		eBUTTON_RADIAL,			//�����޴�, �巡�׾ص߱��� ���� ����
		eBUTTON_MAX,
	};

	enum eACTIVE_STATE
	{
		eACTIVE = 0,			//�ش� ��ư�� Ȱ��ȭ��.
		eINACTIVE,				//�ش� ��ư�� ��Ȱ��ȭ ��(������ �ʴ´�.)
		eACTIVE_MAX,
	};

	enum eVISIBLE_STATE
	{
		eVISIBLE = 0,
		eINVISIBLE,
		eVISIBLE_MAX,
	};

	struct INT_VECTOR2
	{
		int x = 0;
		int y = 0;
	};

	enum eMOUSE_POS
	{
		eMOUSE_POS_CUR = 0,
		eMOUSE_POS_PREVIOUS,
		eMOUSE_POS_CLICKED,
		eMOUSE_POS_MAX,
	};
	
}
