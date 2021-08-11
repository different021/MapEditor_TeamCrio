#pragma once
#include <vector>
//#include "EngineInterface.h"  
#include "../src/Defines.h"
#include "ToolDefine.h"

class HLightData;
class HSimplePrimitive;
class HProceduralGeometry_line;
class HEngine_DX12_3D;
class CLightManager;

//수정 필요 ->그래픽 엔진에 의존적 
struct Light
{
	lightData					m_LightData;				//저장용 인스턴스
	HLightData*					m_pGraphicData = nullptr;	//그래픽 인스턴스
	HSimplePrimitive*			m_pPositionBox = nullptr;	//포지션 표시용 라인
	HProceduralGeometry_line*	m_pDirectionLine = nullptr;	//디렉션 표시용 라인

	~Light();
	void DeleteGraphicInstance();	
	void OutData(lightData& dest);
	void CopyData(lightData& newData);

	//데이터 업데이트는 여기서 해주지만 파괴 및 재생성은 매니저에서 해준다.
	void Update();			//아래 세 과정 -> 이것만 해주면 됨.
	void UpdateLightData();	//입력받은 데이터로 그래픽 인스턴스를 업데이트.
	void UpdateBox();		//라이트 위치를 표시하기 위한 상자 업데이트
	void UpdateLine();		//라이트 방향을 표시하기 위한 선 업데이트
	XMMATRIX GetTM();
};

typedef std::pair<Light*, ShaderType_Primitive> CHANGE_PRIMITIVE;
typedef std::pair<Light*, LightType> CHANGE_LIGHTTYPE;

class CLightManager
{
	static CLightManager*	m_pInstance;			//싱글톤 인스턴스
	static int				m_iInstanceCounter;		//인스턴스 카운터
	HEngine_DX12_3D*		m_pEngine;				//그래픽 엔진
	std::vector<Light*>		m_LightList;			//빛 리스트
	std::vector<Light*>		m_Selected;				//선택된 빛 리스트.
	std::vector<CHANGE_PRIMITIVE>   m_ChangePrimitiveList;	//프리미티브 병경 -> 그래픽 인스턴스 재생성
	std::vector<CHANGE_LIGHTTYPE>   m_ChangeTypeList;		//라이트 타입변경 -> 그래픽 인스턴스 재생성
	std::vector<Light*>		m_DeleteList;			//삭제할 리스트. -> 삭제 시점에 엔진에서 메모리를 건들수 잇어서 시점을 통일해 줘야한다.

	eSELECT_MODE	m_selectMode	= eSELECT_ONE;	//단일/다중 선택 모드
	bool			m_bSelectEnable = true;			//선택 가능 여부

private:
	CLightManager();
	~CLightManager();

	//삭제. 생성 이 같이 들어 잇어서 시점에 유의 해야한다.
	void CreatePosBox(Light* pLight, ShaderType_Primitive type);		//Postion박스 생성
	void CreateDirectionLine(Light* pLight, ShaderType_Primitive type); //Direction라인 생성

public:
	static  CLightManager* GetInstance();
	void	Release();
	void	SetSelectMode(eSELECT_MODE mode);
	void    SetEngine(HEngine_DX12_3D* pEngine);
	void	SetSelectEnabled(bool bCanISelect);
	bool	IsEnableLightSelect();

	//생성
	Light* CreateLight(lightData& lightData);	


	//최신화.
	void Update();				//
	void UpdatePos();			//위치정보 최신화
	void UpdatePrimitive();		//기하 구조 최신화	//리스트에 올라와 잇는것만 최신화
	void UpdateLightType();		//빛 타입 최신화		//

	//기하구조를 바꾸는 작업은 삭제 -> 생성을 다시하는 작업이다.
	void AddChangePrimitiveList(Light* pLight, ShaderType_Primitive primitive);
	
	//빛 타입을 변경 -> 그래픽 인스턴스 재생성.
	void UpdateLightType(Light* pLight, LightType type);
	void AddChangeLightTypeList(Light* pLight, LightType type);

	LightType GetLightType(eLightType type);
			
	//Delete List
	void	AddDeleteList(Light* pLight);
	void	Delete(std::vector<Light*>* pList = NULL);					//동일하다.
	void	DelteInDelList(std::vector<Light*>* pList = NULL);			//삭제.

	void	EditLight(HLightData* pTarget);
	Light*	FindLight(HLightData* pTarget);
	void	ClearList();						
	void	DuplicateSelected();
	
	//SelectedList 관련
	void	AddSelected(Light* pLight);
	void	AddSelected_public(Light* pLight);
	void	AddSelected_onlyOne(Light* pLight);
	void	DeleteInSelectedList(Light* pLight);
	void	DeleteSelectedLight();
	void	EraseSelectedAll();
	void	ClearSelected();
	bool	IsInSelected(Light* pLight);
	DirectX::XMFLOAT3 GetSelectedCenterPos();
	size_t  GetNumberOfSelected();		

	//Picking관련
	Light*  Picking(unsigned int screenX, unsigned int screenY, int inWidth, int inHeight);
	void	GetLightInRect(float ndcX1, float ndcY1, float ndcX2, float ndcY2);

	//lightData 수정.
	void	MoveSelected(float x, float y, float z);
	void	MoveLight(Light* pLight, float dx, float dy, float dz);	//라이트 위치 변화.

	//Select Light
	Light*	GetLastSelected();

	std::vector<Light*>* GetList();
};

