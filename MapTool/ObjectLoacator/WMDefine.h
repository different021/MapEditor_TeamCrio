#pragma once
//******************************************************//
//사용자 지정 윈도우 메세지를 저장하고 잇는 헤더
//******************************************************//

enum eMsgReturn
{
	eMsgSuccess = 0,
	eMsgFail,
	eMsgMax,
};

#define WM_OBJECT						10000
#define WM_COLLIDER						11000
#define WM_LIGHT						12000
#define WM_WAVE							13000

#define WM_RESOURCE						20000
#define WM_UTIL							21000


//WM_OBJECT
#define WM_OBJECT_CREATE                WM_OBJECT + 1
#define WM_OBJECT_DELETE                WM_OBJECT + 2
#define WM_OBJECT_EDIT					WM_OBJECT + 3

#define WM_MOVE_SELECTED				WM_OBJECT + 5		
#define WM_SELECT_OBJ					WM_OBJECT + 6		
#define WM_DELETE_GRAPHIC_INS			WM_OBJECT + 7		
#define WM_LIST_BOX_NONE				WM_OBJECT + 8		//기능 확인

//resource
#define WM_LOAD_MODEL					WM_RESOURCE + 1
#define WM_LOAD_MAT						WM_RESOURCE + 2

#define WM_REQUEST_MODEL_LIST			WM_RESOURCE + 3		//수정 고려
#define WM_REQUEST_MAT_LIST				WM_RESOURCE + 4	
#define WM_UPDATE_OBJECT_LIST			WM_RESOURCE + 5		//수정 고료
#define WM_UPDATE_MODEL_LIST			WM_RESOURCE + 6
#define WM_UPDATE_MATERIAL_LIST			WM_RESOURCE + 7		


#define WM_LOAD_MAP						WM_UTIL + 1
#define WM_SAVE_MAP						WM_UTIL + 2
#define WM_LOAD_FBX						WM_UTIL + 3

#define WM_HIDE_WINDOW					WM_UTIL + 4
#define WM_LOG							WM_UTIL + 5
#define WM_CHANGE_GRID					WM_UTIL + 6			//Grid
#define WM_RE_DRAW						WM_UTIL + 26		//viwer draw call


//Collider
#define WM_CREATE_COLLIDER				WM_COLLIDER	+ 1		
#define WM_DELETE_COLLIDER				WM_COLLIDER + 2		
#define WM_EDIT_COLLIDER				WM_COLLIDER + 3		
#define WM_UPDATE_COLLIDER_LIST			WM_COLLIDER + 4		
#define WM_CLEAR_COLLIDER_LIST			WM_COLLIDER + 5		


#define WM_PICKING_COLLIDER				WM_COLLIDER + 6		//Collider picking
#define WM_CHANGE_SELECTED				WM_COLLIDER + 7		//Collider picking


