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
		//내부 구성요소는 삭제하지 못하도록?
		delete[] m_ppContainerArray;
		m_ppContainerArray = nullptr;
	}
	
	//배열 정보 초기화
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

	//찾을때까지 순회
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] == pTarget)	//찾음
		{
			//삭제하지 않고 목록에서만 지운다.
			m_ppContainerArray[i] = nullptr;

			FilltheBlankInArray(i);			//삭제로 인해 배열 한칸씩 밀기.

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
	//내용만 비운다.
	for (int i = 0; i < m_CountOfContainer; i++)
	{
		if (m_ppContainerArray[i] != nullptr)
		{
			m_ppContainerArray[i] = nullptr;
		}
	}

	m_CountOfContainer = 0;
}

