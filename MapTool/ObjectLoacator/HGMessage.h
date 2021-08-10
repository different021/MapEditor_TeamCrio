#pragma once

enum eInstanceType
{
	eCpuInstance = 0,
	eGpuInstance,
	eInstanceTypeMax,
};

enum eObjectType
{
	eObject = 0,
	eCollider,
	eLight,
	eWave,
	eObjectTypeMax,
};

struct HGMessage
{
	unsigned int  msg;				//����� ���� �޼��� (Ȯ�ο�) SendMessage�� ����� �����ؾ��Ѵ�.
	eObjectType   objType;			//������Ʈ�� Ÿ��.
	unsigned int  numOfInstance;	//��� �ν��Ͻ��� �����Դ°�
	eInstanceType instanceType;		//CPU or GPU instance type (GPU means Engine Type)
	void*		  pObj0;			//�����̳ʷ� ����?
	void*		  pObj1;			//
	void*		  pObj2;			//
	void*		  pObj3;			//
};

struct MsgObjectArray
{
	unsigned int count;
	eObjectType  objectType;
	void*		 pArray;
};
