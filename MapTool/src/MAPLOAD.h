#pragma once
#include <Windows.h>
#include "Defines.h"


class MapLoader
{
public:
	static BOOL Load(wchar_t* pFileName, MAP* &pDest);									//USE this fuc
	static BOOL Load(const wchar_t pFileName[], MAP* &pDest);
	static void DeleteMap(MAP* pMap);

	static BOOL OpenMapFileToSave(HANDLE& hFile, wchar_t* pFileName);					//Save
	static DWORD WriteHeaderFromFile(HANDLE hFile, STAGE_HEADER & header);
	static DWORD WriteObjectFromFile(HANDLE hFile, object * pDest, DWORD sizeOfBuf);
	static DWORD WriteColliderFromFile(HANDLE hFile, collider* pDest, DWORD sizeOfBuf);
	static DWORD WriteLightFromFile(HANDLE hFile, lightData* pDest, DWORD sizeOfBuf);
	static DWORD WriteWaveCountFromFile(HANDLE hFile, int* pDest, DWORD sizeOfBuf);
	static DWORD WriteWaveFromFile(HANDLE hFile, waveData* pDest, DWORD sizeOfBuf);

	static BOOL OpenMapFileToLoad(HANDLE& hFile, wchar_t* pFileName);					//Load
	static BOOL ReadHeader(HANDLE hFile, STAGE_HEADER& out);							
	
	//Collider
	static BOOL ReadColliderAllVersion(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest);
	static BOOL ReadCollider(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest);
	//static BOOL ReadCollider_v1(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest);	//v1은 v0 버젼과 동일.
	static BOOL ReadCollider_v2(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest);

	static BOOL ReadLight(HANDLE& hFile, STAGE_HEADER& header, lightData*& pDest);

	// Object 
	static BOOL ReadObjectAllVersion(HANDLE& hFile, STAGE_HEADER& header, object*& pDest);
	static BOOL ReadObject(HANDLE& hFile, STAGE_HEADER& header, object*& pDest);
	static BOOL ReadObject_v1(HANDLE& hFile, STAGE_HEADER& header, object*& pDest);
	static BOOL ReadObject_v2(HANDLE& hFile, STAGE_HEADER& header, object*& pDest);		//필요.
	
	//Wave
	static BOOL ReadWaveAllVersion(HANDLE& hFile, STAGE_HEADER& header, int cntOfWave, waveData*& pMapDest);	//웨이브 갯수랑 리스트들을 읽어온다.
	static BOOL ReadWaveCnt(HANDLE& hFile, STAGE_HEADER& header, int* pDest);
	static BOOL ReadWaveV1(HANDLE& hFile, STAGE_HEADER& header, int cntOfWave, waveData*& pMapDest);	
};

