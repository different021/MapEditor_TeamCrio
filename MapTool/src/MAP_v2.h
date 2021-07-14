#pragma once
#include "Defines.h"

/*
	�ݶ��̴��� Ÿ�� �߰��Ǳ���.
*/

struct collider_v2
{
	//eCOLLIDER_TYPE	  type;		�߰�
	//int				  index;	�߰�
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
	//eOBJECT_NPC_REINFORCE_V2,			//�߰�
	eOBJECT_MAX_v2,
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

};

struct MAP_V2
{
	STAGE_HEADER	_header;
	object* _pObjList = NULL;	//������Ʈ ����Ʈ
	collider* _pColliderList = NULL;	//�ݶ��̴� ����Ʈ
	lightData* _pLightList = NULL;	//�� ����Ʈ
};
