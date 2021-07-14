#pragma once
#include "Defines.h"

/*
	�ݶ��̴��� Ÿ�� �߰��Ǳ���.
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
	//eOBJECT_NPC_REINFORCE_V2,			//���������� �߰���.
	eOBJECT_MAX,
};


struct object_v2
{
	eOBJECT_TYPE type;				//������Ʈ Ÿ��
	eShaderType  eShaderType;		//���̴� Ÿ��.
	DWORD		 voidSpace;			//�� ����.


	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;

	WCHAR objectName[MAX_LENGTH];	//������ -> ����� �Է� ����.
	WCHAR modelName[MAX_LENGTH];	//�Ѵܾ� ex) robot
	WCHAR matName[MAX_LENGTH];		//�Ѵܾ� ex) silverMetal

	int modelIndex;					//������ �� �ε���		//�ʿ����?
	int matIndex;					//������ �ؽ��� �ε���	//�ʿ����?

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