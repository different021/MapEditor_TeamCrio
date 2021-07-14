#pragma once
#include "Defines.h"

/*
	//오브젝트 타입 변화
	//collision 을 별도로 판단하지 않는다.

	변한것만 보관하지 않고 모든 헤더를 보관한다. -> offset이 틀려지기 때문에.
	한셋트씩
*/

enum eOBJECT_TYPE_v1
{
	eOBJECTv1_FIELD = 0,
	eOBJECTv1_ENEMY_NORMAL,
	eOBJECTv1_ENEMY_RANGE,
	eOBJECTv1_ENEMY_DASH,
	eOBJECTv1_MAX,
};

enum eIsCollision		//삭제된 enum
{
	eCollisionYes = 0,
	eCollisionNo,

	eCollisionMax,
};

struct object_v1
{
	eOBJECT_TYPE_v1 type;				//오브젝트 타입
	eShaderType		eShaderType;		//쉐이더 타입.
	eIsCollision	isCollision;		//콜리전 체크를 할 것인지 말 것인지에 대하여. -> eNum ->

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

public:
	object_v1();
	void SetPos(DirectX::XMFLOAT3& pos);
	void SetRot(DirectX::XMFLOAT4& rot);
	void SetOiler(DirectX::XMFLOAT3& oiler);
	void SetScale(DirectX::XMFLOAT3& scale);
	void SetPrvRot(DirectX::XMFLOAT4& rot);
	void SetMatIndex(int index);

	void CopyObject(object_v1* pDest);

	void GetTm(DirectX::XMFLOAT4X4& out) const;
	DirectX::XMFLOAT4X4 GetTm();
	DirectX::XMFLOAT4X4 GetTmByOiler();
	DirectX::XMFLOAT4X4	GetTmByQuaternion();
};


struct MAP_V1
{
	STAGE_HEADER	_header;
	object*			_pObjList = NULL;	//오브젝트 리스트
	collider*		_pColliderList = NULL;	//콜라이더 리스트
	lightData*		_pLightList = NULL;	//빛 리스트
};

