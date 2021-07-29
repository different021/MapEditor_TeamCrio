#include "pch.h"
#include "CContainer.h"
#include "CSelectedArray.h"

CSelectedArray::CSelectedArray()
{
}

CSelectedArray::~CSelectedArray()
{
}

void CSelectedArray::ReleaseArray()
{
	if (m_ppContainerArray == nullptr)
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

int CSelectedArray::AddContainer(CContainer* pNewContainer)
{
	return CContainerArray::AddContainer(pNewContainer);
}

int CSelectedArray::DeleteContainerInArray(CContainer* pContainerDeleting)
{
	CContainer* pTarget = pContainerDeleting;
	if (pTarget == nullptr) return -1;

	//ã�������� ��ȸ
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] == pTarget)	//ã��
		{
			//�������� �ʰ� ��Ͽ����� �����.
			m_ppContainerArray[i] = nullptr;

			FilltheBlankInArray(i);			//������ ���� �迭 ��ĭ�� �б�.

			m_CountOfContainer--;
			break;
		}
	}

	return m_CountOfContainer;
}

int CSelectedArray::FindContainerInArray(CContainer* pContainerFinding)
{
	return CContainerArray::FindContainerInArray(pContainerFinding);
}

void CSelectedArray::ClearArray()
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

