#pragma once
#include "CContainerArray.h"

class CContainer;

class CDeleteArray : public CContainerArray
{
public:
	CDeleteArray();
	~CDeleteArray() override;

	virtual void ReleaseArray() override;											//Relese Memory Allocated -> private?
	virtual int  AddContainer(CContainer* pNewContainer) override;					//return object Count
	virtual void ClearArray() override;												//Delete Object List

	void DeleteContainerInstanceInArray();											//이 배열에 들어있는 인스턴스 삭제.

};

