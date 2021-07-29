#include "pch.h"
#include "CManager.h"
#include "CContainer.h"

CManager::CManager()
{
	m_ppList		 = new CContainer*[MAX_OBJECT_COUNT];
	m_ppSelectedList = new CContainer*[MAX_OBJECT_COUNT];
	m_ppDeleteList   = new CContainer*[MAX_OBJECT_COUNT];
}

CManager::~CManager()
{
	if (!m_ppDeleteList)
	{
		for (int i = 0; i < MAX_OBJECT_COUNT; i++)
		{
			m_ppDeleteList[i] = nullptr;
		}
		
		delete[] m_ppDeleteList;
		m_ppDeleteList = nullptr;
	}

	if (!m_ppSelectedList)
	{
		for (int i = 0; i < MAX_OBJECT_COUNT; i++)
		{
			m_ppSelectedList[i] = nullptr;
		}

		delete[] m_ppSelectedList;
		m_ppSelectedList = nullptr;
	}

	if (!m_ppList)
	{
		for (int i = 0; i < MAX_OBJECT_COUNT; i++)
		{
			m_ppList[i]->DeleteMe();
			m_ppList[i] = nullptr;
		}

		delete[] m_ppList;
		m_ppList = nullptr;
	}
}
