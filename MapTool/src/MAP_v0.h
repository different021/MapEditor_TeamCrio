#pragma once
#include "Defines.h"

//각 OBJECT 버젼별 로더 필요.

enum eOBJECT_TYPE_v0
{
	eOBJECTv0_FIELD = 0,
	eOBJECTv0_ENEMY_NORMAL,
	eOBJECTv0_ENEMY_RANGE,
	eOBJECTv0_ENEMY_DASH,

	eOBJECTv0_MAX,
};

struct object_v0
{
	eOBJECT_TYPE type;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;

	WCHAR objectName[MAX_LENGTH];		//툴지정 -> 사용자 입력 가능.
	WCHAR modelName[MAX_LENGTH];		//한단어 ex) robot
	WCHAR matName[MAX_LENGTH];			//한단어 ex) silverMetal

	int modelIndex;						//참조할 모델 인덱스		//필요없다?
	int matIndex;						//참조할 텍스쳐 인덱스	//필요없다?

	
public:
	void SetPos(DirectX::XMFLOAT3& pos);
	void SetRot(DirectX::XMFLOAT4& rot);
	void SetOiler(DirectX::XMFLOAT3& oiler);
	void SetScale(DirectX::XMFLOAT3& scale);
	void SetPrvRot(DirectX::XMFLOAT4& rot);

	void CopyObject(object* pDest);

	void GetTm(DirectX::XMFLOAT4X4& out) const;
	DirectX::XMFLOAT4X4 GetTm();
	DirectX::XMFLOAT4X4 GetTmByOiler();
	DirectX::XMFLOAT4X4	GetTmByQuaternion();
};