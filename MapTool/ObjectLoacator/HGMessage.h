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
	unsigned int  msg;				//사용자 정의 메세지 (확인용) SendMessage의 내용과 동일해야한다.
	eObjectType   objType;			//오브젝트의 타입.
	unsigned int  numOfInstance;	//몇개의 인스턴스를 보내왔는가
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