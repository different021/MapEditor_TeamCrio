#pragma once
#include "CContainerArray.h"
/*
* CObjectContainer의 포인터 배열을 갖고있는 클래스
* 클릭/드래그 등으로 선택된 오브젝트의 정보를 담기 위한 클래스
* 이 클래스에서는 해당 컨테이너들의 데이터를 직접적으로 삭제하지 않는다

*/
class CContainer;

class CSelectedArray : public CContainerArray
{

public:
	CSelectedArray();
	~CSelectedArray() override;

	virtual void ReleaseArray() override;											//Relese Memory Allocated -> private?
	virtual int  AddContainer(CContainer* pNewContainer) override;					//return object Count
	virtual int  DeleteContainerInArray(CContainer* pContainerDeleting) override;	//return object Count
	virtual int  FindContainerInArray(CContainer* pContainerFinding) override;		//return index of object
	virtual void ClearArray() override;												//Delete Object List

};

