#pragma once

class CContainer;

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
	bool  FilltheBlankInArray(int blankIndex);	

public:
	CContainerArray();
	CContainerArray(unsigned int MaxArrayCount);
	virtual ~CContainerArray();
	virtual void ReleaseArray();												//Relese Memory Allocated -> private?
	virtual int  AddContainer(CContainer* pNewObject);					//return object Count
	virtual int  DeleteContainerInArray(CContainer* pObjectDeleting);	//return object Count
	virtual int  FindContainerInArray(CContainer* pObjectFinding);		//return index of object
	virtual void ClearArray();											//Delete Object List

	CContainer* GetContainer(int index);						//It return nullptr When can not Find
};

