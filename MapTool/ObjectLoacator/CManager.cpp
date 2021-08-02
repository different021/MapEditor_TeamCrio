#include "pch.h"
#include "CManager.h"

#include "CContainerArray.h"
#include "CSelectedArray.h"
#include "CDeleteArray.h"

void CManager::CreateArrays(int maxResourceCount)
{
	int count = maxResourceCount;
	//Create Array Instance
	if (m_pArray == nullptr)
	{
		m_pArray = new CContainerArray();
	}

	if (m_pSelectedArray == nullptr)
	{
		m_pSelectedArray = new CSelectedArray();
	}

	if (m_pDeleteArray == nullptr)
	{
		m_pDeleteArray = new CDeleteArray();
	}

	if (count < 1)
	{
		m_pArray->CreateArray();
		m_pSelectedArray->CreateArray();
		m_pDeleteArray->CreateArray();
	}
	else
	{
		m_pArray->CreateArray(count);
		m_pSelectedArray->CreateArray(count);
		m_pDeleteArray->CreateArray(count);
	}
}

void CManager::SetContainerType(eCONTAINER_TYPE type)
{
	if (type > eCONTAINER_TYPE::eCONTAINER_MAX)
	{
		OutputDebugStringW(L"Cmanager::SetContainerType() type is Out of Index");
		assert(false);
		return;
	}
	else if (type < 0)
	{
		OutputDebugStringW(L"Cmanager::SetContainerType() type is under zero");
		assert(false);
		return;
	}

	m_ContainerType = type;
}

void CManager::Cleanup()
{
	if (m_pArray)
	{
		delete m_pArray;
		m_pArray = nullptr;	
	}

	if (m_pSelectedArray)
	{
		delete m_pSelectedArray;
		m_pSelectedArray = nullptr;
	}
	
	if (m_pDeleteArray)
	{
		delete m_pDeleteArray;
		m_pDeleteArray = nullptr;
	}
}

CManager::CManager()
{
}

CManager::~CManager()
{
	Cleanup();
}

void CManager::Initialize(int maxResourceCount, eCONTAINER_TYPE type)
{
	CreateArrays(maxResourceCount);
	SetContainerType(type);

}


