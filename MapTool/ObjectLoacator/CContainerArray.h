#pragma once
#include "CContainer.h"
//class CContainer;

class CContainerArray
{
protected:
	int m_ArrayMaxCount			= 0;			//�� �迭�� ������ �� �մ� �ִ� ������Ʈ ��
	int m_ArrayMaxIndex			= 0;			//�� �迭�� �ִ� �ε���
	int m_CountOfContainer		= 0;			//�� �迭�� ����ִ� ������Ʈ ��
	CContainer** m_ppContainerArray = nullptr;	//�����Ҵ� ������ �迭

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

