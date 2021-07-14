#pragma once
#include <vector>
#include <utility>
#include <DirectXMath.h>
#include "ToolDefine.h"

struct HSimplePrimitive;
struct collider;
typedef std::pair<collider*, HSimplePrimitive*> COLLIDER;	//스마트 포인터가 더 간편?


class CColliderManager
{
private:
	std::vector< COLLIDER > m_ColliderList;
	std::vector< COLLIDER > m_SelectedList;
	static CColliderManager* m_pInstance;
	static int m_iInstanceCounter;
	int m_HighestIndex = 0;
	eSELECT_MODE m_selectMode = eSELECT_ONE;
	bool m_bSelectEnable = true;
	CColliderManager();
	~CColliderManager();

	
public:
	static CColliderManager* GetInstance();
	void DeleteInstance();
	void SetSelectMode(eSELECT_MODE mode);

	void Update();

	collider* CreateCollider();
	static collider* CreateColliderArray(int numberOfCollider);
	int GetHighestColliderIndex();
	void SetIndexWhenYouLoad();

	//List
	const std::vector<COLLIDER>* GetList();		
	const std::vector<COLLIDER>* GetSelected();
	void DeleteColliderList();
	void EnableSelect(bool bSelect);
	bool IsEnableColliderSelect();
	void Sort();

	collider* Picking(unsigned int screenX, unsigned int screenY, int width, int height);

	COLLIDER* FindCollider(collider* pTarget);
	std::vector<COLLIDER>::iterator FindColliderFromSelected(collider* pTarget);

	//Selected List 
	size_t AddSelected(collider* pObj);
	size_t AddSelected_public(collider* pObj);		//이것을 사용할 것.
	size_t AddSelected_OnlyOne(collider* pObj);
	size_t AddSelected_DeleteAlreadySelected(collider* pObj);
	void   DeleteSelectedAll();
	size_t DeleteSelected(collider* pObj);
	size_t GetSizeOfSelected();
	void   SetSelectedPrvRot();
	void   MoveSelected(__in float dx, __in float dy, __in float dz);
	void   RotateSelected(DirectX::XMFLOAT4& quaternion);
	void   RescaleSelected(float dx, float dy, float dz);
	void   ReScaleSelectedByRatio(float ratio);
	void   RescaleSelectedByRatioX(float ratio);
	void   RescaleSelectedByRatioY(float ratio);
	void   RescaleSelectedByRatioZ(float ratio);

	collider* GetLastSelected();
	DirectX::XMFLOAT3 GetSelectedPos();




	void GetColliderInRect(float ndcX1, float ndcY1, float ndcX2, float ndcY2);

	HRESULT InsertColliderList(collider* pCollider);
	HRESULT DeleteCollider(collider* pCollider);
	HRESULT EditCollider(collider* pCollider);
	//HRESULT CopyCollider(collider* pDest, collider* pSrc);
	HRESULT DeleteColliderInSelectedList();
	HRESULT DuplicateColliderInSelectedList();
};

