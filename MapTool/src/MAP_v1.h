#pragma once
#include "Defines.h"

/*
	//������Ʈ Ÿ�� ��ȭ
	//collision �� ������ �Ǵ����� �ʴ´�.

	���Ѱ͸� �������� �ʰ� ��� ����� �����Ѵ�. -> offset�� Ʋ������ ������.
	�Ѽ�Ʈ��
*/

enum eOBJECT_TYPE_v1
{
	eOBJECTv1_FIELD = 0,
	eOBJECTv1_ENEMY_NORMAL,
	eOBJECTv1_ENEMY_RANGE,
	eOBJECTv1_ENEMY_DASH,
	eOBJECTv1_MAX,
};

enum eIsCollision		//������ enum
{
	eCollisionYes = 0,
	eCollisionNo,

	eCollisionMax,
};

struct object_v1
{
	eOBJECT_TYPE_v1 type;				//������Ʈ Ÿ��
	eShaderType		eShaderType;		//���̴� Ÿ��.
	eIsCollision	isCollision;		//�ݸ��� üũ�� �� ������ �� �������� ���Ͽ�. -> eNum ->

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
	object*			_pObjList = NULL;	//������Ʈ ����Ʈ
	collider*		_pColliderList = NULL;	//�ݶ��̴� ����Ʈ
	lightData*		_pLightList = NULL;	//�� ����Ʈ
};

