#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <atlstr.h>
#include <SimpleMath.h>


#define MAX_LENGTH 128

//�� �����ͺ��� ������ �ɰ��� ���� ���� ��.

enum eMAP_VERSION
{
	eMAP_VERSION0 = 0,
	eMAP_VERSION1,
	eMAP_VERSION_MAX,
};

enum eOBJ_VERSION
{
	eOBJ_VERSION0 = 0,
	eOBJ_VERSION1,
	eOBJ_VERSION2,	
	eOBJ_VERSION3,
	eOBJ_VERSION_MAX,	//�ݶ��̴� Ÿ�� �߰�.
};

enum eOBJECT_TYPE
{
	eOBJECT_FIELD_COLLISION_YES = 0,
	eOBJECT_FIELD_COLLISION_NO,
	eOBJECT_ENEMY_NORMAL,
	eOBJECT_ENEMY_RANGE,
	eOBJECT_ENEMY_DASH,
	eOBJECT_NPC_REINFORCE,	//ver3�߰�.
	eOBJECT_ENEMY_BOSS,		//�߰�.

	eOBJECT_MAX,
};

enum eShaderType
{
	eSHADER_DEFAULT = 0,
	eSHADER_CORLORCHIP,
	eSHADER_WIRE,
	eSHADER_MAX,
};

#pragma pack(push, 1)
struct object
{
	eOBJECT_TYPE type;				//������Ʈ Ÿ��
	eShaderType  eShaderType;		//���̴� Ÿ��.
	int			 regenIndex;		//ver3 ����. ���� �ݶ��̴� �ε��� ����(���� �ݶ��̴��� �����ϸ� �ش� �ε����� ������Ʈ�� ����)
	
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
	object();
	~object();
	void SetPos(DirectX::XMFLOAT3& pos);
	void SetRot(DirectX::XMFLOAT4& rot);
	void SetOiler(DirectX::XMFLOAT3& oiler);
	void SetScale(DirectX::XMFLOAT3& scale);
	void SetPrvRot(DirectX::XMFLOAT4& rot);
	void SetMatIndex(int index);
	void SetRegenIndex(int index);
	void CopyTo(object* pDest);

	void GetTm(DirectX::XMFLOAT4X4& out) const; 
	DirectX::XMFLOAT4X4 GetTm();
	DirectX::XMFLOAT4X4 GetTmByOiler();
	DirectX::XMFLOAT4X4	GetTmByQuaternion();
	//void GetTm(DirectX::XMFLOAT4X4* pOut);
};
#pragma pack(pop)

enum eCOLLIDER_TYPE						//ver3 �߰�
{
	eCOLLIDER_STARTPOINT = 0,			//���� ����(�ټ�?)
	eCOLLIDER_ENDPOINT,					//���� Ŭ���� ����(�ټ�)
	eCOLLIDER_REGENERATIONPOINT,		//�� ���ڿ� �浹�� ��� ���� ����
	eCOLLIDER_NORMAL,					//������ϴ� �Ϲ��� �ݶ��̴�.
	eCOLLIDER_MAX,
};

#pragma pack(push, 1)
struct collider
{
	eCOLLIDER_TYPE	  type;				//ver3 �߰�.
	int				  index;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;
	DirectX::SimpleMath::Vector3 size;
	BOOL  isVisible;
	WCHAR name[MAX_LENGTH];

	friend class CColliderManager;			//�ν��Ͻ� ����/����
	friend class MapLoader;					//�� ������ �δ�
	friend class CSaver;					//�� ������ ���̺�
private:
	collider();								//��ü ���� ���� -> friend

public:
	void CopyTo(collider* pDest);
	void CopyExceptIndex(collider* pDest);
	void SaveCollider(collider* pDest);
	void MakeName();
	void GetTM(DirectX::XMFLOAT4X4& out);
	void GetTM(DirectX::XMFLOAT4X4* pOut);
	DirectX::XMMATRIX GetTM();
};
#pragma pack(pop)

enum eTEX
{
	eAlbedo = 0,
	eRoughness,
	eMetallic,
	eAo,
	eNormal,
	eHeight,
	eTEX_MAX,
};

struct strTEXTURE
{
	wchar_t* tex[eTEX::eTEX_MAX];

	//~strTEXTURE();
	void Clear();
};

enum eLightType
{
	eDirect = 0,
	ePoint,
	eSpot,
	eLightTypeMax,
};

#pragma pack(push, 1)
struct lightData
{
	eLightType type;
	float strength[3];
	float falloffStart;
	float direction[3];
	float falloffEnd;
	float position[3];
	float spotPower;
	wchar_t name[MAX_LENGTH];
};
#pragma pack(pop)

enum STAGE_HEADER_VERSION
{
	eSTAGE_HEADER_V0 = 0,
	eSTAGE_HEADER_V1,
	eSTAGE_HEADER_V2,		//�ĵ� �߰�.

	eSTAGE_HEADER_MAX,
};

#pragma pack(push, 1)
struct waveData
{
	//���� �Ұ� -> ����� �ν��Ͻ� �����
	int		row;		//���� 
	int		col;		//����
	float	dx;			//�׸��� �Ÿ�
	float	speed;		//�ӵ�?
	float	damping;	//??

	//���氡��
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;

	waveData();
	void SetOiler(DirectX::XMFLOAT3& oil);
	DirectX::XMFLOAT4X4 GetTm();
};
#pragma pack(pop)

//120byte
struct STAGE_HEADER
{
	WCHAR	mapName[MAX_LENGTH];	//�� �̸�
	int		MapVersion;				//�� ����
	int		ObjectVersion;			//������Ʈ ����	-> ������ ����ü ��ȣ
	int		iCamCnt;				//ī�޶� ����	-> �̱���
	int		iObjCnt;				//������Ʈ ����
	int		iLightCnt;				//����Ʈ ����	-> �߰� �� ����.	
	int		iColliderCnt;			//�浹�ڽ� ����
	int		iWidth;					//�׸��� ���� _ ����
	int		iHeight;				//�׸��� ���� _ ����
	float	offset;					//�׸��� ��ĭ ũ�� _ ���簢��
};


struct MAP
{
	STAGE_HEADER	_header;
	object*			_pObjList = NULL;		//������Ʈ ����Ʈ
	collider*		_pColliderList = NULL;	//�ݶ��̴� ����Ʈ
	lightData*		_pLightList = NULL;		//�� ����Ʈ

	int				_iWaveCnt = 0;			//ver2�߰�. -> ����� ���Խ�, ��� byte���� ����Ǿ� �ϴ��� ���⿡ �߰�.
	waveData*		_pWaveList = NULL;		//ver2�߰�.
};



