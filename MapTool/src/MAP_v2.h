#pragma once
#include "Defines.h"

/*
	콜라이더의 타입 추가되기전.
*/

struct collider_v2
{
	//eCOLLIDER_TYPE	  type;		추가
	//int				  index;	추가
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;
	DirectX::SimpleMath::Vector3 size;
	BOOL isVisible;
	WCHAR name[MAX_LENGTH];
};

enum eOBJECT_TYPE_V2
{
	eOBJECT_FIELD_COLLISION_YES_V2 = 0,
	eOBJECT_FIELD_COLLISION_NO_V2,
	eOBJECT_ENEMY_NORMAL_V2,
	eOBJECT_ENEMY_RANGE_V2,
	eOBJECT_ENEMY_DASH_V2,
	//eOBJECT_NPC_REINFORCE_V2,			//추가
	eOBJECT_MAX_v2,
};


struct object_v2
{
	eOBJECT_TYPE type;				//오브젝트 타입
	eShaderType  eShaderType;		//쉐이더 타입.
	DWORD		 voidSpace;			//빈 공간.

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;

	WCHAR objectName[MAX_LENGTH];	//툴지정 -> 사용자 입력 가능.
	WCHAR modelName[MAX_LENGTH];	//한단어 ex) robot
	WCHAR matName[MAX_LENGTH];		//한단어 ex) silverMetal

	int modelIndex;					//참조할 모델 인덱스		//필요없다?
	int matIndex;					//참조할 텍스쳐 인덱스	//필요없다?

};

struct MAP_V2
{
	STAGE_HEADER	_header;
	object* _pObjList = NULL;	//오브젝트 리스트
	collider* _pColliderList = NULL;	//콜라이더 리스트
	lightData* _pLightList = NULL;	//빛 리스트
};
