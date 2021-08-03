#pragma once
#include "CContainer.h"
//class CContainer;

class CContainerArray
{
protected:
	int m_ArrayMaxCount			= 0;			//이 배열이 저장할 수 잇는 최대 오브젝트 수
	int m_ArrayMaxIndex			= 0;			//이 배열의 최대 인덱스
	int m_CountOfContainer		= 0;			//이 배열에 들어있는 오브젝트 값
	CContainer** m_ppContainerArray = nullptr;	//동적할당 포인터 배열

protected:
	/*
	* return value
	*	true  : Excute memcpy
	*	false : Not Excute memcpy
	*/
	bool FilltheBlankInArray(int blankIndex);	
	bool IsInArray(CContainer* pContainer);				//Checking does it exist in array

public:
	CContainerArray();
	virtual ~CContainerArray();
	virtual void ReleaseArray();										//Relese Memory Allocated -> private?
	virtual int  AddContainer(CContainer* pNewObject);					//return object Count
	virtual void ClearArray();											//Delete Object List
	
public:
	bool		CreateArray(int MaxArrayCount = MAX_OBJECT_COUNT);		//You must do this func when you initialate this class
	int		    GetContainerIndex(CContainer* pContainerFinding);		//return -1 when It can not find, return -2 when parameter is nullptr
	CContainer* GetContainerPointer(int index);							//It return nullptr When can not Find
	int			RemoveContainer(CContainer* pRemove);					//remove in array (not Delete instance)
	int			RemoveAll();											//remove all in array(not Delete instance)
};

