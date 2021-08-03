#pragma once

static const unsigned int MAX_OBJECT_COUNT = 128;	//���� ������ ����� �̵�.


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
	virtual void DeleteMe() abstract;								//�ν��Ͻ� ����
	virtual void Update() abstract;									//�׷��� �ν��Ͻ� ������Ʈ
	virtual CContainer* MakeDuplication(CContainer& src) abstract;	//���纻 ����
	virtual bool operator == (CContainer* pTarget) abstract;

public:
	void SetType(eCONTAINER_TYPE type);
	eCONTAINER_TYPE GetType();
};

