#pragma once
/*
*	툴 내부에서만 알면 되는 내용들 정의 
*	ViewDlg클래스와 ObjectLoacatorDlg클래스 양쪽에서 알아야하는 내용.
*/
#include "EngineInterface.h"

#define IsKeyDown(key)	( (::GetAsyncKeyState(key) & 0x8000) )
#define IsKeyUp(key)	( (::GetAsyncKeyState(key) & 0x0001) )

#define iDEFAULTGRID_W 50
#define iDEFAULTGRID_H 50
#define fDEFAULTOFFSET 10.f

enum eDirection
{
	eDirection_Up = 0,
	eDirection_Down,
	eDirection_Left,
	eDirection_Right,
	eDirection_Front,
	eDirection_Back,
	eDirection_MAX,
};

enum eSELECT_MODE
{
	eSELECT_ONE = 0,
	eSELECT_MULTI,
	eSELECT_MAX,
};

struct MODEL
{
	bool isField;
	int index;				//모델 고유 인덱스		
	HModelData* hModel;		//모델 핸들
	std::string fileName;	//파일 이름
};

struct MATERIAL
{
	int index;
	HMaterialData* hMat;
	CStringA matName;
};

struct GRID_INFO
{
	int iWidth;
	int iHeight;
	float offset;
};


//extern std::vector<MODEL*> g_hModelList;
//extern std::vector<MATERIAL*> g_MaterialList;

extern HANDLE g_hEvent;

