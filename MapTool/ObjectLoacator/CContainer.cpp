#include "pch.h"
#include "CContainer.h"

void CContainer::SetType(eCONTAINER_TYPE type)
{
	if (type > eCONTAINER_TYPE::eCONTAINER_MAX)
	{
		OutputDebugStringW(L"CContainer::SetType() [Out of index]\n");
		assert(false);
		return;
	}
	else if (type < 0)
	{

		OutputDebugStringW(L"CContainer::SetType() [type is under Zero]\n");
		assert(false);
		return;
	}

	m_ContainerType = type;
}

eCONTAINER_TYPE CContainer::GetType()
{
	return m_ContainerType;
}
