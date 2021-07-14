#pragma once
#include "Defines.h"

/*
	콜라이더의 타입 추가되기전.
*/



struct collider_v2
{
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
	//eOBJECT_NPC_REINFORCE_V2,			//현버젼에서 추가됨.
	eOBJECT_MAX,
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

//public:
//	object();
//	void SetPos(DirectX::XMFLOAT3& pos);
//	void SetRot(DirectX::XMFLOAT4& rot);
//	void SetOiler(DirectX::XMFLOAT3& oiler);
//	void SetScale(DirectX::XMFLOAT3& scale);
//	void SetPrvRot(DirectX::XMFLOAT4& rot);
//	void SetMatIndex(int index);
//
//	void CopyObject(object* pDest);
//
//	void GetTm(DirectX::XMFLOAT4X4& out) const;
//	DirectX::XMFLOAT4X4 GetTm();
//	DirectX::XMFLOAT4X4 GetTmByOiler();
//	DirectX::XMFLOAT4X4	GetTmByQuaternion();
//	//void GetTm(DirectX::XMFLOAT4X4* pOut);
};