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
		int screenWidth;			//화면 가로 크기
		int screenHeight;			//화면 세로 크기
		int numOfUI_INFO;			//IMG_INFO 의 갯수
		int numOfChildIndexBuffer;	//childIndexBuffer 크기
		char UI_NAME[BUFFERMAX];
	};


	//하나의 스프라이트에 대한 정보.
	/*
		해당 UI이미지에대한 정보를 스태틱하게 저장.
		다음에도 게임에서 사용할 데이터.
	*/
	struct UI_INFO
	{
		int   index;					//인덱스값 고유해야 한다.
		int	  parentIndex;				//부모의 인덱스

		int	  AbsolutePos[eAxis_MAX];	//절대 좌표(int)	
		int   AbsoluteWidth;			//절대 좌표	
		int   AbsoluteHeight;			//절대 좌표	

		float RatioPos[eAxis_MAX];		//상대적 좌표(float)
		float RatioWidth;				//
		float RatioHeight;				//
		float opacity;					//투명도 -> 저장될 값.
		bool  bIsAbsolutePosition;		//false 화면의 비율 -> 툴에서는 false만 사용.(비율로만)
		
		int   numOfChild;				//자신이 갖고 있는 자식수.
		int   startChildIndex;			//차일드 인덱스 버퍼에서 참조를 시작할 인덱스 자식 숫자만큼 참조.
		int   rootParent;				//같은 루트노드를 갖는 구룹.

		int	  screenWidth;				//스크린 너비
		int   screenHeight;				//스크린 높이

		WCHAR NameOfSprite[BUFFERMAX];	//스프라이트 이름.
		WCHAR NameOfUI[BUFFERMAX];		//이 UI이름.	-> 삭제?
	};


	enum eBUTTON_EVENT			//마우스 관련 이벤트
	{
		eEVENT_DEFAULT = 0,		//기본 상태
		eEVENT_INACTIVE,		//버튼 사용 불가 상태
		eEVENT_MOUSEIN,			//마우스가 안으로 들어온 직후
		eEVENT_MOUSEOUT,		//마우스가 밖으로 빠져나간 직후
		eEVENT_MOUSEON,			//마우스가 올라가있는 상태.
		eEVENT_LBUTTONUP,		//왼 버튼 UP
		eEVENT_LBUTTONDOWN,		//왼 버튼 DOWN
		eEVENT_RBUTTONUP,		//오른 버튼 UP
		eEVENT_RBUTTONDOWN,		//오른 버튼 DOWN
		
		eEVENT_MAX,
	};



	enum eBUTTON_TYPE
	{
		eBUTTON_NORMAL = 0,		//기본 버튼
		eBUTTON_TOGGLE,			//누를경우 상태가 바뀐다.
		eBUTTON_SLIDE,			//슬라이드바를 이용하여 값을 변경 할 수 있다.
		eBUTTON_RADIAL,			//원형메뉴, 드래그앤뒹굴로 값을 변경
		eBUTTON_MAX,
	};

	enum eACTIVE_STATE
	{
		eACTIVE = 0,			//해당 버튼이 활성화됨.
		eINACTIVE,				//해당 버튼이 비활성화 됨(사용되지 않는다.)
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
