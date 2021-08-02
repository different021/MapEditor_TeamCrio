#include "pch.h"
#include "CContainerArray.h"
#include "CContainer.h"

/*
	* return value
	*	true : does memcpy
	*	false : does not memcpy
*/
bool CContainerArray::FilltheBlankInArray(int blankIndex)
{
	bool bResult = false;
	int targetIndex = blankIndex;
	if (targetIndex < 0) return bResult;
	if (targetIndex > m_ArrayMaxIndex) return bResult;

	int countOfCopyObj = m_CountOfContainer - (targetIndex + 1);
	if (countOfCopyObj > 0)
	{
		//�ش� ��ĭ�� �����.
		rsize_t copySize = sizeof(CContainer*) * countOfCopyObj;
		memcpy_s(m_ppContainerArray[targetIndex], copySize, m_ppContainerArray[targetIndex + 1], copySize);		//��ĭ �ڿ� �ִ� �͵� ���� ����
		m_ppContainerArray[targetIndex + countOfCopyObj] = nullptr;										//������ �����Ͱ� �ִ� ���� ���� �����.
		bResult = true;
	}

	return bResult;
}

CContainerArray::CContainerArray()
{
}

CContainerArray::~CContainerArray()
{
	ReleaseArray();
}

void CContainerArray::ReleaseArray()
{
	if (m_ppContainerArray == nullptr)
	{
		m_ArrayMaxCount		= 0;
		m_ArrayMaxIndex		= 0;
		m_CountOfContainer		= 0;
		return;
	}

	int numOfObjDelete = 0;
	for (int i = 0; numOfObjDelete < m_CountOfContainer; i++)
	{
		if (i > m_ArrayMaxIndex)
		{
			//��� ������Ʈ�� ����������� �迭 �ε����� ������ ����
			OutputDebugStringW(L"[CContainerArray]Cleanup(): OutOfIndex");
			break;
		}

		if (m_ppContainerArray[i] != nullptr)		//�����Ͱ� �ִ� �ε����̸�.
		{
			//�迭 ������ ������Ʈ ����
			m_ppContainerArray[i]->DeleteMe();
			m_ppContainerArray[i] = nullptr;
			numOfObjDelete++;
		}
	}

	delete[] m_ppContainerArray;
	m_ppContainerArray = nullptr;

	//�迭 ���� �ʱ�ȭ
	m_ArrayMaxCount = 0;
	m_ArrayMaxIndex = 0;
	m_CountOfContainer = 0;
}

/*
	return value:
		-1 is [Object is nullptr]
		-2 is [Full Of Array]
*/
int CContainerArray::AddContainer(CContainer* pNewObject)
{
	if (pNewObject == nullptr)
	{
		//Invaildate parameter 
		return -1;
	}

	if(m_CountOfContainer >= m_ArrayMaxCount)
	{
		//Full Of Array
		unsigned int newArraySize = m_ArrayMaxCount * 2;
		CContainer** ppTemp = new CContainer*[newArraySize];
		if (ppTemp != nullptr)
		{
			rsize_t sourceSize = sizeof(CContainer*) * m_CountOfContainer;
			memcpy_s(ppTemp, sourceSize, m_ppContainerArray, sourceSize);
			
			delete[] m_ppContainerArray;
			m_ppContainerArray = nullptr;

			m_ppContainerArray = ppTemp;

			m_ArrayMaxCount = newArraySize;
			m_ArrayMaxIndex = newArraySize - 1;
		}
		
	}

	m_ppContainerArray[m_CountOfContainer] = pNewObject;
	m_CountOfContainer++;

	return m_CountOfContainer;
}

/*
* return value
*	-1 : Invaild Parameter
*/
int CContainerArray::DeleteContainerInArray(CContainer* pObjectDeleting)
{
	if (pObjectDeleting == nullptr) return -1;

	//ã�������� ��ȸ
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] == pObjectDeleting)	//ã��
		{
			m_ppContainerArray[i]->DeleteMe();
			m_ppContainerArray[i] = nullptr;

			FilltheBlankInArray(i);			//������ ���� �迭 ��ĭ�� �б�.
			
			m_CountOfContainer--;
			break;
		}
	}

	return m_CountOfContainer;
}


void CContainerArray::ClearArray()
{
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] != nullptr)
		{
			m_ppContainerArray[i]->DeleteMe();
			m_ppContainerArray[i] = nullptr;
		}
	}

	m_CountOfContainer = 0;
}

bool CContainerArray::CreateArray(int MaxArrayCount)
{
	m_ppContainerArray = new CContainer * [MaxArrayCount];
	if (m_ppContainerArray == nullptr)
	{
		//�޸� �Ҵ� ���� 
		OutputDebugStringW(L"[CContainerArray]CContainerArray(): Fail Memory Allocate");
		assert(false);
	}
	m_ArrayMaxCount = MaxArrayCount;
	m_ArrayMaxIndex = MaxArrayCount - 1;
	m_CountOfContainer = 0;
	return false;
}

int CContainerArray::GetContainerIndex(CContainer* pContainerFinding)
{

	CContainer* pTarget = pContainerFinding;
	int iResult = -1;
	if (pTarget == nullptr) return -2;

	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (pTarget == m_ppContainerArray[i])
		{
			iResult = i;
			break;
		}
	}

	return iResult;
}

CContainer* CContainerArray::GetContainerPointer(int index)
{
	CContainer* pResult = nullptr;
	int objectIndex = index;

	if (objectIndex < m_CountOfContainer)
	{
		pResult = m_ppContainerArray[objectIndex];
	}

	return pResult;
}
