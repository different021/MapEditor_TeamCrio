#pragma once

class CContainer;

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

