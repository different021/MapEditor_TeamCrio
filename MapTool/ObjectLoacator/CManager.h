#pragma once
#include "CContainer.h"

class CContainerArray;
class CSelectedArray;
class CDeleteArray;

class CManager
{
private:
	eCONTAINER_TYPE  m_ContainerType;		//�� �Ŵ����� ���� �����̳��� Ÿ��.
	CContainerArray* m_pArray;
	CSelectedArray*  m_pSelectedArray;
	CDeleteArray*    m_pDeleteArray;

private:
	void CreateArrays(int maxResourceCount);
	void SetContainerType(eCONTAINER_TYPE type);
	void Cleanup();

public:
	CManager();
	~CManager();

	void Initialize(int maxResourceCount, eCONTAINER_TYPE type);	//second parameter(eCONTAINER_TYPE) is Type of Managerment, first Parameter is Array Size
	
};

