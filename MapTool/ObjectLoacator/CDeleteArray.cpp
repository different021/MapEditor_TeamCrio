#include "pch.h"
#include "CDeleteArray.h"
#include "CContainer.h"

CDeleteArray::CDeleteArray()
{
}

CDeleteArray::~CDeleteArray()
{
}

void CDeleteArray::ReleaseArray()
{
	if(m_ppContainerArray == nullptr)
	{
		m_ArrayMaxCount = 0;
		m_ArrayMaxIndex = 0;
		m_CountOfContainer = 0;
		return;
	}

	if (!m_ppContainerArray)
	{
		//���� ������Ҵ� �������� ���ϵ���?
		delete[] m_ppContainerArray;
		m_ppContainerArray = nullptr;
	}

	//�迭 ���� �ʱ�ȭ
	m_ArrayMaxCount = 0;
	m_ArrayMaxIndex = 0;
	m_CountOfContainer = 0;
}

int CDeleteArray::AddContainer(CContainer* pNewContainer)
{
	return CContainerArray::AddContainer(pNewContainer);
}


void CDeleteArray::ClearArray()
{
	//���븸 ����.
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] != nullptr)
		{
			m_ppContainerArray[i] = nullptr;
		}
	}

	m_CountOfContainer = 0;
}

void CDeleteArray::DeleteContainerInstanceInArray()
{
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray != nullptr)
		{
			m_ppContainerArray[i]->DeleteMe();
			m_ppContainerArray[i] = nullptr;
		}
	}

	ClearArray();
}
