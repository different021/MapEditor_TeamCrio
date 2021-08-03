#pragma once
#include "CContainer.h"

class CContainerArray;
class CSelectedArray;
class CDeleteArray;


/*
  Remove: Array������ ����
  Delete: �ν��Ͻ� ����
*/


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

	void RemoveContainer(CContainer* pContainer);					//�����̳ʸ� ����Ʈ������ ����
public:
	CManager();
	~CManager();

	void Initialize(int maxResourceCount, eCONTAINER_TYPE type);	//second parameter(eCONTAINER_TYPE) is Type of Managerment, first Parameter is Array Size
	
	//Array -> ��� �����̳�
	void AddContainer(CContainer* pContainer);						//�����̳ʸ� ������ �߰�
	
	//Selected -> ���õ� �����̳�
	void AddSelected(CContainer* pContainer);						//���õ� ����Ʈ�� �߰�(���� ��)
	void RemoveInSelected(CContainer* pContainer);					//���õ� ����Ʈ���� ����(�� �̻� ���� ���)
	void RemoveInSelectedAll();										//���õ� ����Ʈ���� ��� �� ����.

	//DeleteArray -> ������ �����̳�
	//�׷��� �ν��Ͻ� null���� ���� �ʵ��� -> �׸��� �� Ÿ�ֿ̹� �ϰ� ����
	void AddToDeleteArray(CContainer* pContainer);					//������ ����Ʈ�� �߰�. 
	void AddToDeleteListInSelectedArray();							//������ �ν��Ͻ����� ���� ����Ʈ�� �߰�.
	void DeleteInDeleteArray();										//���� ����Ʈ�� �ִ� �ν��Ͻ� ����
	void RemoveInDeleteArray();										//���� ����Ʈ���� ����(����Ʈ�� Ŭ����)
};

