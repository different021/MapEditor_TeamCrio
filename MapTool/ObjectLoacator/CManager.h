#pragma once


class CContainer;

class CManager
{
	CContainer** m_ppList = {};
	CContainer** m_ppSelectedList = {};
	CContainer** m_ppDeleteList   = {};

	int m_CountOfObjectList		= 0;
	int m_CountOfSelectedList	= 0;
	int m_CountOfDeleteLsit		= 0;

public:
	CManager();
	~CManager();

	int AddObject(CContainer* pObj);					//새로운 오브젝트에 추가
	int DeleteObjectByDeleteList(CContainer* pObj);	//디리트 리스트
	
};

