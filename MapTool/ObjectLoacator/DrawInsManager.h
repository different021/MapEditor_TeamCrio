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

	//리스트에서 제거만. -> 인스턴스 삭제하지 아니함.
	//void EraseInObjList(object* pFind);

	//삭제리스트
	void AddToDeleteListAll();				//전체를 
	void AddDeleteListInSelectedList();						///여기서부터 확인할 것.
	void AddDeleteList(DRAW_INSTANCE* pIns);
	void AddDeleteList(object* pObj);		//해당 하나만

	//삭제
	void DeleteInDeleteList(std::vector<object*>* objList = NULL);
	//void DeleteObject(DRAW_INSTANCE* pIns);		//악의 근원.?
	//bool DeleteObject(object* pObj);	
	
	//생성
	void CreateObj(object* pObj, HInstanceData* pGraphicsData);

	//Selected List 
	object* AddSelected_public(HInstanceData* pHIns);//현재 셀렉트 모드에 따라 추가
	object* AddSelected(HInstanceData* pHIns);							//아무 고려 없이 추가
	object* AddSelected_DeleteAlreadySelected(HInstanceData* pHIns);	//이미 선택된 오브젝트는 제거, 없을 경우 추가
	object* AddSelected_OnlyOne(HInstanceData* pHIns);					//오로지 하나만 추가한다.
	DRAW_INSTANCE* AddSelected(object* pObj);
	object* GetLastSelected();
	void	AddObjectInSelectedList(object* pObj);
	size_t  GetNumberOfSelectedObject();

	bool IsSelected(object* pObj);	//석택된 리스트안에 있는가?
	DRAW_INSTANCE* FindDrawIns(object* pObj);		//Obj를 이용하여 DrawIns 찾기
	
	//SelectedList Clear
	void ClearSelectedListAll();				//전체
	void ClearSelectedListByDeleteList();		//private 삭제 리스트에서 삭제가 이루어지기 전에 행해질것.
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

