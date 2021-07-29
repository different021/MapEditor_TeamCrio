#pragma once
#include "CContainerArray.h"
/*
* CObjectContainer�� ������ �迭�� �����ִ� Ŭ����
* Ŭ��/�巡�� ������ ���õ� ������Ʈ�� ������ ��� ���� Ŭ����
* �� Ŭ���������� �ش� �����̳ʵ��� �����͸� ���������� �������� �ʴ´�

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

