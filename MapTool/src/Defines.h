#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <atlstr.h>
#include <SimpleMath.h>


#define MAX_LENGTH 128

//각 데이터별로 파일을 쪼개는 편이 좋을 듯.

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
	eOBJ_VERSION_MAX,	//콜라이더 타입 추가.
};

enum eOBJECT_TYPE
{
	eOBJECT_FIELD_COLLISION_YES = 0,
	eOBJECT_FIELD_COLLISION_NO,
	eOBJECT_ENEMY_NORMAL,
	eOBJECT_ENEMY_RANGE,
	eOBJECT_ENEMY_DASH,
	eOBJECT_NPC_REINFORCE,	//ver3추가.
	eOBJECT_ENEMY_BOSS,		//추가.

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
	eOBJECT_TYPE type;				//오브젝트 타입
	eShaderType  eShaderType;		//쉐이더 타입.
	int			 regenIndex;		//ver3 변경. 리젠 콜라이더 인덱스 저장(리젠 콜라이더에 진입하면 해당 인덱스의 오브젝트들 리젠)
	
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

enum eCOLLIDER_TYPE						//ver3 추가
{
	eCOLLIDER_STARTPOINT = 0,			//시작 지점(다수?)
	eCOLLIDER_ENDPOINT,					//게임 클리어 지점(다수)
	eCOLLIDER_REGENERATIONPOINT,		//이 상자에 충돌할 경우 몬스터 생성
	eCOLLIDER_NORMAL,					//통과못하는 일반적 콜라이더.
	eCOLLIDER_MAX,
};

#pragma pack(push, 1)
struct collider
{
	eCOLLIDER_TYPE	  type;				//ver3 추가.
	int				  index;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 oiler;
	DirectX::XMFLOAT4 quaternion;
	DirectX::XMFLOAT4 prevRot;
	DirectX::XMFLOAT3 scale;
	DirectX::SimpleMath::Vector3 size;
	BOOL  isVisible;
	WCHAR name[MAX_LENGTH];

	friend class CColliderManager;			//인스턴스 생성/관리
	friend class MapLoader;					//맵 데이터 로더
	friend class CSaver;					//맵 데이터 세이브
private:
	collider();								//객체 생성 제한 -> friend

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
	eSTAGE_HEADER_V2,		//파도 추가.

	eSTAGE_HEADER_MAX,
};

#pragma pack(push, 1)
struct waveData
{
	//변경 불가 -> 변경시 인스턴스 재생성
	int		row;		//가로 
	int		col;		//세로
	float	dx;			//그리드 거리
	float	speed;		//속도?
	float	damping;	//??

	//변경가능
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
	WCHAR	mapName[MAX_LENGTH];	//맵 이름
	int		MapVersion;				//맵 버젼
	int		ObjectVersion;			//오브젝트 버젼	-> 참고할 구조체 번호
	int		iCamCnt;				//카메라 갯수	-> 미구현
	int		iObjCnt;				//오븢게트 갯수
	int		iLightCnt;				//라이트 갯수	-> 추가 할 내용.	
	int		iColliderCnt;			//충돌박스 갯수
	int		iWidth;					//그리드 갯수 _ 가로
	int		iHeight;				//그리드 갯수 _ 세로
	float	offset;					//그리드 한칸 크기 _ 정사각형
};


struct MAP
{
	STAGE_HEADER	_header;
	object*			_pObjList = NULL;		//오브젝트 리스트
	collider*		_pColliderList = NULL;	//콜라이더 리스트
	lightData*		_pLightList = NULL;		//빛 리스트

	int				_iWaveCnt = 0;			//ver2추가. -> 헤더에 포함시, 헤더 byte수가 변경되어 일단은 여기에 추가.
	waveData*		_pWaveList = NULL;		//ver2추가.
};



