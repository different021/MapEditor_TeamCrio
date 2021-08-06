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
	int			  space;
	int			  space2;
	int			  space3;
	int			  space4;
};

struct MsgObjectArray
{
	unsigned int count;
	eObjectType  objectType;
	void*		 pArray;
};