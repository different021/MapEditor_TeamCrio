#pragma once

static const unsigned int MAX_OBJECT_COUNT = 128;	//별도 디파인 헤더로 이동.

enum eCONTAINER_TYPE
{
	eCONTAINER_OBJECT = 0,
	eCONTAINER_COLLIDER,
	eCONTAINER_LIGHT,
	eCONTAINER_MAX,
};

class CContainer abstract
{
	eCONTAINER_TYPE m_ContainerType;
private:
	virtual ~CContainer() = 0;

public:
	virtual void DeleteMe() abstract;		//
};

