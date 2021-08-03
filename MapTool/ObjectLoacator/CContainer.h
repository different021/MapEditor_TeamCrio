#pragma once

static const unsigned int MAX_OBJECT_COUNT = 128;	//별도 디파인 헤더로 이동.


enum eCONTAINER_TYPE
{
	eCONTAINER_OBJECT = 0,
	eCONTAINER_COLLIDER,
	eCONTAINER_LIGHT,
	eCONTAINER_MAX,
};

class CContainer
{
	eCONTAINER_TYPE m_ContainerType;
private:
	virtual ~CContainer() = 0;

public:
	virtual void DeleteMe() abstract;								//인스턴스 삭제
	virtual void Update() abstract;									//그래픽 인스턴스 업데이트
	virtual CContainer* MakeDuplication(CContainer& src) abstract;	//복사본 생성
	virtual bool operator == (CContainer* pTarget) abstract;

public:
	void SetType(eCONTAINER_TYPE type);
	eCONTAINER_TYPE GetType();
};

