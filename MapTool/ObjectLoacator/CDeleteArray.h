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
	virtual int  DeleteContainerInArray(CContainer* pContainerDeleting) override;	//return object Count
	virtual void ClearArray() override;												//Delete Object List

	void DeleteContainerInstanceInArray();

};

