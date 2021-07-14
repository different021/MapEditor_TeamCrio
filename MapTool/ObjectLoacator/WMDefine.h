#pragma once
//******************************************************//
//사용자 지정 윈도우 메세지를 저장하고 잇는 헤더
//******************************************************//

#define WM_CREATE_OBJ                   10001					//사용안함.
#define WM_DELETE_OBJ                   WM_CREATE_OBJ + 1
#define WM_EDIT_OBJ						WM_CREATE_OBJ + 2

#define WM_CREATE_PREVIEW_OBJ			WM_CREATE_OBJ + 4	//삭제 대상
#define WM_UPDATE_PREVIEW_OBJ			WM_CREATE_OBJ + 5	//삭제 대상
#define WM_LOAD_MODEL					WM_CREATE_OBJ + 6
#define WM_LOAD_MAT						WM_CREATE_OBJ + 7
//#define WM_CHANGE_GRID					WM_CREATE_OBJ + 8
#define WM_HIDE_WINDOW					WM_CREATE_OBJ + 9
#define WM_LOG							WM_CREATE_OBJ + 10
#define WM_LOAD_MAP						WM_CREATE_OBJ + 11
#define WM_SAVE_MAP						WM_CREATE_OBJ + 12
#define WM_LOAD_FBX						WM_CREATE_OBJ + 13
#define WM_CREATE_CHARACTER				WM_CREATE_OBJ + 14

//#define WM_UPDATE_SELECTED				WM_CREATE_OBJ + 15

#define WM_REQUEST_MODEL_LIST			WM_CREATE_OBJ + 16	
#define WM_REQUEST_MAT_LIST				WM_CREATE_OBJ + 17	

#define WM_MOVE_SELECTED				WM_CREATE_OBJ + 20		//viewer
#define WM_SELECT_OBJ					WM_CREATE_OBJ + 21		
//#define WM_UPDATE_LAST_SELECTED			WM_CREATE_OBJ + 22	

#define WM_UPDATE_OBJECT_LIST			WM_CREATE_OBJ + 23
#define WM_UPDATE_MODEL_LIST			WM_CREATE_OBJ + 24
#define WM_UPDATE_MATERIAL_LIST			WM_CREATE_OBJ + 25		//
#define WM_RE_DRAW						WM_CREATE_OBJ + 26		//ObjLocator
//#define WM_CLEAR_DRAW_INS_LIST			WM_CREATE_OBJ + 27		//objLocator
#define WM_DELETE_GRAPHIC_INS			WM_CREATE_OBJ + 28		//objLocator
#define WM_LIST_BOX_NONE				WM_CREATE_OBJ + 29		//objLocator

#define WM_CREATE_COLLIDER				WM_CREATE_OBJ + 100		//Collider
//#define WM_DELETE_COLLIDER				WM_CREATE_OBJ + 101		//Collider
#define WM_EDIT_COLLIDER				WM_CREATE_OBJ + 102		//Collider
#define WM_UPDATE_COLLIDER_LIST			WM_CREATE_OBJ + 103		//Collider
#define WM_CLEAR_COLLIDER_LIST			WM_CREATE_OBJ + 104		//Collider


#define WM_PICKING_COLLIDER				WM_CREATE_OBJ + 105		//Collider picking
#define WM_CHANGE_SELECTED				WM_CREATE_OBJ + 106		//Collider picking


