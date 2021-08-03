#pragma once
#include "CContainer.h"

class CContainerArray;
class CSelectedArray;
class CDeleteArray;


/*
  Remove: Array에서만 제거
  Delete: 인스턴스 삭제
*/


class CManager
{
private:
	eCONTAINER_TYPE  m_ContainerType;		//이 매니저에 담을 콘테이너의 타입.
	CContainerArray* m_pArray;
	CSelectedArray*  m_pSelectedArray;
	CDeleteArray*    m_pDeleteArray;

private:
	void CreateArrays(int maxResourceCount);
	void SetContainerType(eCONTAINER_TYPE type);
	void Cleanup();

	void RemoveContainer(CContainer* pContainer);					//콘테이너를 리스트에서만 제거
public:
	CManager();
	~CManager();

	void Initialize(int maxResourceCount, eCONTAINER_TYPE type);	//second parameter(eCONTAINER_TYPE) is Type of Managerment, first Parameter is Array Size
	
	//Array -> 모든 콘테이너
	void AddContainer(CContainer* pContainer);						//콘테이너를 새로이 추가
	
	//Selected -> 선택된 컨테이너
	void AddSelected(CContainer* pContainer);						//선택된 리스트에 추가(선택 됨)
	void RemoveInSelected(CContainer* pContainer);					//선택된 리스트에서 제거(더 이상 선택 취소)
	void RemoveInSelectedAll();										//선택된 리스트에의 모든 것 제거.

	//DeleteArray -> 삭제할 컨테이너
	//그래픽 인스턴스 null참조 하지 않도록 -> 그리기 전 타이밍에 일괄 삭제
	void AddToDeleteArray(CContainer* pContainer);					//삭제할 리스트에 추가. 
	void AddToDeleteListInSelectedArray();							//선택한 인스턴스들을 삭제 리스트에 추가.
	void DeleteInDeleteArray();										//삭제 리스트에 있는 인스턴스 삭제
	void RemoveInDeleteArray();										//삭제 리스트에서 제거(리스트만 클리어)
};

