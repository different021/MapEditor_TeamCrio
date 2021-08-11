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

//���� �ʿ� ->�׷��� ������ ������ 
struct Light
{
	lightData					m_LightData;				//����� �ν��Ͻ�
	HLightData*					m_pGraphicData = nullptr;	//�׷��� �ν��Ͻ�
	HSimplePrimitive*			m_pPositionBox = nullptr;	//������ ǥ�ÿ� ����
	HProceduralGeometry_line*	m_pDirectionLine = nullptr;	//�𷺼� ǥ�ÿ� ����

	~Light();
	void DeleteGraphicInstance();	
	void OutData(lightData& dest);
	void CopyData(lightData& newData);

	//������ ������Ʈ�� ���⼭ �������� �ı� �� ������� �Ŵ������� ���ش�.
	void Update();			//�Ʒ� �� ���� -> �̰͸� ���ָ� ��.
	void UpdateLightData();	//�Է¹��� �����ͷ� �׷��� �ν��Ͻ��� ������Ʈ.
	void UpdateBox();		//����Ʈ ��ġ�� ǥ���ϱ� ���� ���� ������Ʈ
	void UpdateLine();		//����Ʈ ������ ǥ���ϱ� ���� �� ������Ʈ
	XMMATRIX GetTM();
};

typedef std::pair<Light*, ShaderType_Primitive> CHANGE_PRIMITIVE;
typedef std::pair<Light*, LightType> CHANGE_LIGHTTYPE;

class CLightManager
{
	static CLightManager*	m_pInstance;			//�̱��� �ν��Ͻ�
	static int				m_iInstanceCounter;		//�ν��Ͻ� ī����
	HEngine_DX12_3D*		m_pEngine;				//�׷��� ����
	std::vector<Light*>		m_LightList;			//�� ����Ʈ
	std::vector<Light*>		m_Selected;				//���õ� �� ����Ʈ.
	std::vector<CHANGE_PRIMITIVE>   m_ChangePrimitiveList;	//������Ƽ�� ���� -> �׷��� �ν��Ͻ� �����
	std::vector<CHANGE_LIGHTTYPE>   m_ChangeTypeList;		//����Ʈ Ÿ�Ժ��� -> �׷��� �ν��Ͻ� �����
	std::vector<Light*>		m_DeleteList;			//������ ����Ʈ. -> ���� ������ �������� �޸𸮸� �ǵ�� �վ ������ ������ ����Ѵ�.

	eSELECT_MODE	m_selectMode	= eSELECT_ONE;	//����/���� ���� ���
	bool			m_bSelectEnable = true;			//���� ���� ����

private:
	CLightManager();
	~CLightManager();

	//����. ���� �� ���� ��� �վ ������ ���� �ؾ��Ѵ�.
	void CreatePosBox(Light* pLight, ShaderType_Primitive type);		//Postion�ڽ� ����
	void CreateDirectionLine(Light* pLight, ShaderType_Primitive type); //Direction���� ����

public:
	static  CLightManager* GetInstance();
	void	Release();
	void	SetSelectMode(eSELECT_MODE mode);
	void    SetEngine(HEngine_DX12_3D* pEngine);
	void	SetSelectEnabled(bool bCanISelect);
	bool	IsEnableLightSelect();

	//����
	Light* CreateLight(lightData& lightData);	


	//�ֽ�ȭ.
	void Update();				//
	void UpdatePos();			//��ġ���� �ֽ�ȭ
	void UpdatePrimitive();		//���� ���� �ֽ�ȭ	//����Ʈ�� �ö�� �մ°͸� �ֽ�ȭ
	void UpdateLightType();		//�� Ÿ�� �ֽ�ȭ		//

	//���ϱ����� �ٲٴ� �۾��� ���� -> ������ �ٽ��ϴ� �۾��̴�.
	void AddChangePrimitiveList(Light* pLight, ShaderType_Primitive primitive);
	
	//�� Ÿ���� ���� -> �׷��� �ν��Ͻ� �����.
	void UpdateLightType(Light* pLight, LightType type);
	void AddChangeLightTypeList(Light* pLight, LightType type);

	LightType GetLightType(eLightType type);
			
	//Delete List
	void	AddDeleteList(Light* pLight);
	void	Delete(std::vector<Light*>* pList = NULL);					//�����ϴ�.
	void	DelteInDelList(std::vector<Light*>* pList = NULL);			//����.

	void	EditLight(HLightData* pTarget);
	Light*	FindLight(HLightData* pTarget);
	void	ClearList();						
	void	DuplicateSelected();
	
	//SelectedList ����
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

	//Picking����
	Light*  Picking(unsigned int screenX, unsigned int screenY, int inWidth, int inHeight);
	void	GetLightInRect(float ndcX1, float ndcY1, float ndcX2, float ndcY2);

	//lightData ����.
	void	MoveSelected(float x, float y, float z);
	void	MoveLight(Light* pLight, float dx, float dy, float dz);	//����Ʈ ��ġ ��ȭ.

	//Select Light
	Light*	GetLastSelected();

	std::vector<Light*>* GetList();
};

