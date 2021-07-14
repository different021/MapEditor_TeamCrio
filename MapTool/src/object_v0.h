#pragma once
#include "Defines.h"

//�� OBJECT ������ �δ� �ʿ�.

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

	WCHAR objectName[MAX_LENGTH];		//������ -> ����� �Է� ����.
	WCHAR modelName[MAX_LENGTH];		//�Ѵܾ� ex) robot
	WCHAR matName[MAX_LENGTH];			//�Ѵܾ� ex) silverMetal

	int modelIndex;						//������ �� �ε���		//�ʿ����?
	int matIndex;						//������ �ؽ��� �ε���	//�ʿ����?

	
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