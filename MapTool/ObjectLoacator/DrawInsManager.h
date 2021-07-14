#pragma once

#include <vector>
#include <utility>
#include "EngineInterface.h"
#include "../src/Defines.h"
#include "ToolDefine.h"

typedef std::pair<object*, HInstanceData*> DRAW_INSTANCE;
typedef std::vector<DRAW_INSTANCE*> DrawInsList;


class DrawInsManager
{
	static DrawInsManager* m_pInstance;
	std::vector<DRAW_INSTANCE*> m_List;
	std::vector<DRAW_INSTANCE*> m_SelectedList;
	std::vector<DRAW_INSTANCE*> m_DeleteList;

	bool m_bEnableSelect = true;
	eSELECT_MODE m_eSelectedMode = eSELECT_ONE;
	static int m_InsCount;

	DrawInsManager();
	~DrawInsManager();
	void cleanup();
public:
	void Update();
	//bool UpdateTM(object* pObj);
	
	static DrawInsManager* GetInstance();
	void Release();
	
	void SetSelectedMode(eSELECT_MODE mode);
	void EnableSelect(bool bEnable);
	bool IsEnableSelectObject();

	//����Ʈ���� ���Ÿ�. -> �ν��Ͻ� �������� �ƴ���.
	//void EraseInObjList(object* pFind);

	//��������Ʈ
	void AddToDeleteListAll();				//��ü�� 
	void AddDeleteListInSelectedList();						///���⼭���� Ȯ���� ��.
	void AddDeleteList(DRAW_INSTANCE* pIns);
	void AddDeleteList(object* pObj);		//�ش� �ϳ���

	//����
	void DeleteInDeleteList(std::vector<object*>* objList = NULL);
	//void DeleteObject(DRAW_INSTANCE* pIns);		//���� �ٿ�.?
	//bool DeleteObject(object* pObj);	
	
	//����
	void CreateObj(object* pObj, HInstanceData* pGraphicsData);

	//Selected List 
	object* AddSelected_public(HInstanceData* pHIns);//���� ����Ʈ ��忡 ���� �߰�
	object* AddSelected(HInstanceData* pHIns);							//�ƹ� ��� ���� �߰�
	object* AddSelected_DeleteAlreadySelected(HInstanceData* pHIns);	//�̹� ���õ� ������Ʈ�� ����, ���� ��� �߰�
	object* AddSelected_OnlyOne(HInstanceData* pHIns);					//������ �ϳ��� �߰��Ѵ�.
	DRAW_INSTANCE* AddSelected(object* pObj);
	object* GetLastSelected();
	void	AddObjectInSelectedList(object* pObj);
	size_t  GetNumberOfSelectedObject();

	bool IsSelected(object* pObj);	//���õ� ����Ʈ�ȿ� �ִ°�?
	DRAW_INSTANCE* FindDrawIns(object* pObj);		//Obj�� �̿��Ͽ� DrawIns ã��
	
	//SelectedList Clear
	void ClearSelectedListAll();				//��ü
	void ClearSelectedListByDeleteList();		//private ���� ����Ʈ���� ������ �̷������ ���� ��������.
	bool ClerarSelectedListByObject(object* pTarget);

	size_t GetSizeOfSelected();
	void   SetSelectedPrvRot();
	
	
	void ChangeMaterialSelected(MATERIAL* pMat);

	DRAW_INSTANCE* GetDrawIns(object* pObj);
	bool GetCenterPos(DirectX::XMFLOAT3* pOut);

	object* GetObjectByIns(__in HInstanceData* hIns, __out DRAW_INSTANCE* pOut);
	HInstanceData* GetInsByObject(__in object* pObj, __out DRAW_INSTANCE* pOut = NULL);

	void MoveSelected(__in float dx, __in float dy, __in float dz);
	void ReScaleSelected(__in float dx, __in float dy, __in float dz);
	void ReScaleSelectedByRatio(float ratio);
	void ReScaleSelectedByRatioX(float ratio);
	void ReScaleSelectedByRatioY(float ratio);
	void ReScaleSelectedByRatioZ(float ratio);

	void RotateSelected(DirectX::XMFLOAT4& quaternion);

	void EditRegenIndexDefault(int regenColliderIndex);

	//List
	std::vector<DRAW_INSTANCE*>* GetDrawInsList();
	std::vector<DRAW_INSTANCE*>* GetSelectedList();
};

