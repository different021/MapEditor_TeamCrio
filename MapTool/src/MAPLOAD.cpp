#include "MAPLOAD.h"
#include "MAP_v1.h"
#include "MAP_v2.h"

//OVERLAPPED g_OverLapped = { 0, };
//LARGE_INTEGER g_nLarge = { 0, };

BOOL GetError(BOOL bResult)
{
	BOOL bR = FALSE;
	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bR = TRUE;
		}
		else if (dwRet == ERROR_INSUFFICIENT_BUFFER)
		{
			OutputDebugStringW(L"Small Buffer\n");
			bR = FALSE;
			assert(0);
			exit(1);
		}
		else if (dwRet == ERROR_OPERATION_ABORTED)
		{
			OutputDebugStringW(L"ERROR ABORT\n");
			bR = FALSE;
			assert(0);
			exit(1);
		}
		else if (dwRet == ERROR_NOT_ENOUGH_MEMORY)
		{
			OutputDebugStringW(L"ERROR MEMORY\n");
			bR = FALSE;
			assert(1);
			exit(1);
		}
	}

	return bR;
}

BOOL MapLoader::Load(wchar_t* pFileName, MAP* &pDest)
{
	//HANDLE hWaitObj = CreateEvent(NULL, TRUE, FALSE, NULL);
	//DWORD dwRet = WaitForSingleObject(hWaitObj, INFINITE);
	BOOL bResult = TRUE;
	HANDLE hFile = NULL;

	//최기화.
	pDest = new MAP;
	ZeroMemory(pDest, sizeof(MAP));

	bResult = OpenMapFileToLoad(hFile, pFileName);
	if (!bResult)
	{
		OutputDebugStringW(L"[FAIL]MpaLoader::Load()::OpenMapFileToLoad()\n");
		assert(FALSE);
		//exit(1);
	}

	bResult = ReadHeader(hFile, pDest->_header);
	if (!bResult)
	{
		OutputDebugStringW(L"[FAIL]MapLoader::Load()::ReadHeader()\n");
		CloseHandle(hFile);
		assert(FALSE);
		//exit(1);
		//return FALSE;
	}

	//bResult = ReadObject(hFile, pDest->_header, pDest->_pObjList);
	bResult = ReadObjectAllVersion(hFile, pDest->_header, pDest->_pObjList);
	if (!bResult)
	{
		OutputDebugStringW(L"[FAIL]MapLoader::Load()::ReadObject()\n");
		CloseHandle(hFile);
		assert(FALSE);
		//exit(1);
		//return FALSE;
	}

	bResult = ReadColliderAllVersion(hFile, pDest->_header, pDest->_pColliderList);
	if (!bResult)
	{
		OutputDebugStringW(L"[FAIL]MapLoader::Load()::ReadCollider()\n");
		CloseHandle(hFile);
		assert(FALSE);
		//exit(1);
		//return FALSE;
	}

	bResult = ReadLight(hFile, pDest->_header, pDest->_pLightList);
	if (!bResult)
	{
		OutputDebugStringW(L"[FAIL]MapLoader::Load()::ReadLight()\n");
		CloseHandle(hFile);
		assert(FALSE);
		//exit(1);
	}

	bResult = ReadWaveCnt(hFile, pDest->_header, &pDest->_iWaveCnt);								//추가됨.
	if (pDest->_iWaveCnt > 0)
	{
		bResult = ReadWaveAllVersion(hFile, pDest->_header, pDest->_iWaveCnt, pDest->_pWaveList);
		if (!bResult)
		{
			OutputDebugStringW(L"[FAIL]MapLoader::Load()::ReadWave()\n");
			CloseHandle(hFile);
			assert(FALSE);
			//exit(1);
		}
	}

	CloseHandle(hFile);
	//SetEvent(hWaitObj);

	return bResult;
}

BOOL MapLoader::Load(const wchar_t pFileName[], MAP*& pDest)
{
	//return Load(std::move(pFileName), pDest);
	return Load(pFileName, pDest);
}

void MapLoader::DeleteMap(MAP* pMap)
{
	if (pMap != NULL)
	{
		if (pMap->_pObjList)
		{
			delete[] pMap->_pObjList;
			pMap->_pObjList = NULL;
		}
		if (pMap->_pColliderList)
		{
			delete[] pMap->_pColliderList;
			pMap->_pColliderList = NULL;
		}

		delete pMap; 
		pMap = NULL;
	}
}

//쓰기 전용 핸들 획득
BOOL MapLoader::OpenMapFileToSave(HANDLE& hFile, wchar_t* pFileName)
{
	BOOL bResult = TRUE;
	LARGE_INTEGER nLargeInteger = { 0 };
	hFile = CreateFileW(
		pFileName,		//파일 이름
		GENERIC_WRITE,				//open for write -> GENERIC_ALL
		0,							//do not share
		NULL,						//defaultsecurity
		CREATE_ALWAYS,				//create New file or overwrite if it exist, and last-error code is set to ERROR_ALREADY_EXISTS (183). or Not Exist the file return ERROR code 0
		FILE_ATTRIBUTE_NORMAL,		//normal file
		NULL						//no attr. template
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwRet = GetLastError();
		WCHAR buf[256];
		wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
		OutputDebugStringW(buf);
		assert(FALSE);
		exit(1);

		bResult = FALSE;
	}
	else
	{
		GetFileSizeEx(hFile, &nLargeInteger);
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		long readAmount = si.dwPageSize;
	}

	return bResult;
}

//해당 위치에 데이터 복사
DWORD MapLoader::WriteHeaderFromFile(HANDLE hFile, STAGE_HEADER& header)
{
	DWORD dwWritten;
	DWORD size = sizeof(STAGE_HEADER);

	BOOL bResult = ::WriteFile(hFile,
								(LPCVOID)&header,
								sizeof(STAGE_HEADER),
								&dwWritten,
								NULL);

	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
		}
	}

	if (dwWritten != size)
	{
		OutputDebugStringW(L"[ERROR]WriteHeader\n");
	}

	return dwWritten;
}

//해당 데이터 청크를 파일 핸들에 복사
DWORD MapLoader::WriteObjectFromFile(HANDLE hFile, object* pDest, DWORD sizeOfBuf)
{
	BOOL bResult = FALSE;
	DWORD dwRet = 0;
	
	bResult = WriteFile(hFile,
		(LPCVOID)pDest,
		sizeOfBuf,
		&dwRet,
		NULL);
	
	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
			assert(0);
		}
	}

	return dwRet;
}

DWORD MapLoader::WriteColliderFromFile(HANDLE hFile, collider* pDest, DWORD sizeOfBuf)
{
	BOOL bResult = FALSE;
	DWORD dwRet = 0;
	bResult = WriteFile(hFile,
		(LPCVOID)pDest,
		sizeOfBuf,
		&dwRet,
		NULL);

	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
			assert(0);
		}
	}

	return dwRet;
}

DWORD MapLoader::WriteLightFromFile(HANDLE hFile, lightData* pDest, DWORD sizeOfBuf)
{
	BOOL bResult = FALSE;
	DWORD dwRet = 0;

	bResult = WriteFile(hFile,
		(LPCVOID)pDest,
		sizeOfBuf,
		&dwRet,
		NULL);

	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
			assert(0);
		}
	}

	return dwRet;
}

DWORD MapLoader::WriteWaveCountFromFile(HANDLE hFile, int* pDest, DWORD sizeOfBuf)
{
	BOOL bResult = FALSE;
	DWORD dwRet = 0;

	bResult = WriteFile(hFile,
		(LPCVOID)pDest,
		sizeOfBuf,
		&dwRet,
		NULL);

	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
			assert(0);
		}
	}

	return dwRet;

}

DWORD MapLoader::WriteWaveFromFile(HANDLE hFile, waveData* pDest, DWORD sizeOfBuf)
{
	BOOL bResult = FALSE;
	DWORD dwRet = 0;

	bResult = WriteFile(hFile,
		(LPCVOID)pDest,
		sizeOfBuf,
		&dwRet,
		NULL);

	if (bResult == 0)
	{
		DWORD dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			bResult = TRUE;
		}
		else
		{
			WCHAR buf[256];
			wprintf(buf, L"[ERROR CODE] : %d\n", dwRet);
			OutputDebugStringW(buf);
			assert(0);
		}
	}

	return dwRet;
}

//열기전용으로 파일 핸들 획득
BOOL MapLoader::OpenMapFileToLoad(HANDLE& hFile, wchar_t* pFileName)
{
	BOOL bResult = TRUE;
	hFile = ::CreateFileW(
		pFileName,
		GENERIC_READ,									//Open for reading
		0,												//FILE_SHARE_READ : share for reading
		NULL,											//default security
		OPEN_EXISTING,									//existing file only
		FILE_FLAG_OVERLAPPED,							//FILE_ATTRIBUTE_NORMAL, // | FILE_FLAG_OVERLAPPED,	//normal file
		NULL);											//no attr. template Filter

	if (hFile == INVALID_HANDLE_VALUE)
	{
		bResult = FALSE;
		OutputDebugStringW(L"[Fail]MapLoader:OpenMapFileToLoad()\n");
		assert(FALSE);
		exit(1);
	}
	else
	{
		//Sucess FileOpen
		LARGE_INTEGER nLargeInteger = {0};
		GetFileSizeEx(hFile, &nLargeInteger);
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		long readAmount = si.dwPageSize;
	}


	return bResult;
}

//데이터 카피
BOOL MapLoader::ReadHeader(HANDLE hFile, STAGE_HEADER& out)
{
	DWORD readByte = 0;
	OVERLAPPED ol{ };
	ol.Offset = 0;

	BOOL bResult = ReadFile(
		hFile,
		&out,
		sizeof(STAGE_HEADER),
		&readByte,
		&ol);

	bResult = GetError(bResult);		//만든 함수 임돠.

	return bResult;
}


BOOL MapLoader::ReadColliderAllVersion(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest)
{
	BOOL bResult = FALSE;
	int objVer = header.ObjectVersion;

	//if (objVer == eOBJ_VERSION::eOBJ_VERSION1);	//초기버젼과 동일.
	if (objVer == eOBJ_VERSION::eOBJ_VERSION2)
	{
		bResult = ReadCollider_v2(hFile, header, pDest);	
	}
	else if (objVer == (eOBJ_VERSION::eOBJ_VERSION_MAX - 1))
	{
		bResult = ReadCollider(hFile, header, pDest);
	}
	return bResult;
}

BOOL MapLoader::ReadCollider(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest)
{
	BOOL bResult = FALSE;
	DWORD dwLengthOfRead = 0;
	size_t countOfCollider = header.iColliderCnt;
	pDest = new collider[countOfCollider];
	
	if (pDest == nullptr)
	{
		//메모리 할당 실패
		OutputDebugStringW(L"[Fail]MapLoader::ReadCollider() Fail to Memory allocate.\n");
		assert(false);
	}

	if (countOfCollider < 0)
	{
		//실패 아니라 콜라이더가 0일 뿐 -> 하지만 제대로된 저장이 아닐 경우일 수 있으니 로그는 남길 것
		OutputDebugStringW(L"MapLoader::ReadCollider() Number Of Collider is Under 0\n ");
		return TRUE;
	}

	long objOffset = sizeof(object) * header.iObjCnt;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER) + objOffset;
	DWORD dwReadSize = static_cast<DWORD>(sizeof(collider) * countOfCollider);
	
	bResult = ReadFile(
		hFile, 
		pDest, 
		dwReadSize, 
		NULL,				//&dwLengthOfRead,
		&ol);

	bResult = GetError(bResult);

	return bResult;
}

BOOL MapLoader::ReadCollider_v2(HANDLE& hFile, STAGE_HEADER& header, collider*& pDest)
{
	BOOL bResult = FALSE;
	DWORD dwLengthOfRead = 0;
	size_t countOfCollider = header.iColliderCnt;

	pDest				= new collider[countOfCollider];
	collider_v2* pTemp  = new collider_v2[countOfCollider];
	
	if (pDest == nullptr)
	{
		//메모리 할당 실패 확인
		OutputDebugStringW(L"[Fail]MapLoader::ReadCollider_v2() Fail to Memory allocate\n");
		assert(false);
	}

	if (pTemp == nullptr)
	{
		//메모리 할당 실패 확인
		OutputDebugStringW(L"[Fail]MapLoader::ReadCollider_v2() Fail to Memory allocate\n");
		assert(false);
	}


	if (countOfCollider < 0)
	{
		OutputDebugStringW(L"MapLoader::ReadCollider_v2() Number Of Collider is Under 0\n ");
		return TRUE;
	}

	long objOffset = sizeof(object_v2) * header.iObjCnt;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER) + objOffset;
	DWORD dwReadSize = static_cast<DWORD>(sizeof(collider_v2) * countOfCollider);

	bResult = ReadFile(
		hFile,
		pTemp,
		dwReadSize,
		NULL,				//&dwLengthOfRead,
		&ol);

	bResult = GetError(bResult);

	if (bResult != TRUE) return bResult;

	for (int i = 0; i < countOfCollider; i++)
	{
		pDest[i].type		= eCOLLIDER_TYPE::eCOLLIDER_NORMAL;
		pDest[i].index		= i;
		pDest[i].pos		= pTemp[i].pos;
		pDest[i].oiler		= pTemp[i].oiler;
		pDest[i].quaternion = pTemp[i].quaternion;
		pDest[i].prevRot	= pTemp[i].prevRot;
		pDest[i].scale		= pTemp[i].scale;
		pDest[i].size		= pTemp[i].size;
		pDest[i].isVisible	= pTemp[i].isVisible;

		pDest->MakeName();		//과거버젼과 이름이 바뀔 수 있다.
	}

	delete[] pTemp;
	pTemp = NULL;

	return bResult;
}

BOOL MapLoader::ReadLight(HANDLE& hFile, STAGE_HEADER& header, lightData*& pDest)
{
	BOOL bResult = FALSE;
	DWORD dwLengthOfRead = 0;
	size_t countOfLight = header.iLightCnt;
	pDest = new lightData[countOfLight];

	if (countOfLight < 0)
	{
		OutputDebugStringW(L"[LoadCollider] Number Of Light is Under 0\n ");
		return TRUE;
	}

	long objOffset = sizeof(object) * header.iObjCnt;
	long colOffset = sizeof(collider) * header.iColliderCnt;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER) + objOffset + colOffset;
	DWORD dwReadSize = static_cast<DWORD>(sizeof(lightData) * countOfLight);

	bResult = ReadFile(
		hFile,
		pDest,
		dwReadSize,
		NULL,				//&dwLengthOfRead,
		&ol);

	bResult = GetError(bResult);


	return bResult;
}

//실패시 FALSE 리턴
//파일 입출력에 windows API CreateFile(), ReadFile()을 사용하는 이유로 리턴값은 대문자 BOOL 
BOOL MapLoader::ReadObjectAllVersion(HANDLE& hFile, STAGE_HEADER& header, object*& pDest)
{
	BOOL bResult = FALSE;
	int objVer = header.ObjectVersion;
	if (objVer == eOBJ_VERSION::eOBJ_VERSION1)
	{
		bResult = ReadObject_v1(hFile, header, pDest);
	}
	else if (objVer == eOBJ_VERSION::eOBJ_VERSION2)
	{
		bResult = ReadObject_v2(hFile, header, pDest);	//두번째, 
	}
	else if (objVer == (eOBJ_VERSION::eOBJ_VERSION_MAX - 1) )
	{
		bResult = ReadObject(hFile, header, pDest);
	}

	return bResult;
}

//해당 포인터에 데이터 청크 생성.(new)
BOOL MapLoader::ReadObject(HANDLE& hFile, STAGE_HEADER& header, object*& pDest)
{
	DWORD dwLengthOfRead = 0;

	int cntObj = header.iObjCnt;
	if (pDest != NULL)
	{
		delete[] pDest;
		pDest = NULL;
	}

	pDest = new object[cntObj];

	BOOL bResult = FALSE;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER);
	DWORD dwReadSize = sizeof(object) * cntObj;

	bResult = ReadFile(
		hFile,
		pDest,
		dwReadSize,
		NULL,
		&ol);

	bResult = GetError(bResult);

	return bResult;
}

//object Version1에 대응하는 함수
//파일 입출력 : windows API
//이 함수는 해당 포인터(pDest)에 메모리 할당이 발생한다.
BOOL MapLoader::ReadObject_v1(HANDLE& hFile, STAGE_HEADER& header, object*& pDest)
{
	DWORD dwLengthOfRead = 0;
	BOOL bResult = FALSE;

	int cntObj = header.iObjCnt;
	if (pDest != NULL)
	{
		//메모리할당 예정
		//이미 할당된 메모리가 있다면 해제하고 재할당
		delete[] pDest;
		pDest = NULL;
	}

	pDest			= new object[cntObj];		//현재 버젼 오브젝트
	object_v1* temp = new object_v1[cntObj];	//과거 버젼 오브젝트
	if (pDest == nullptr || temp == nullptr)
	{
		//메모리 할당 실패
		OutputDebugStringW(L"[FAIL]MapLoader::ReadObject_v1(), Fail to Memory Allocate\n");
		CloseHandle(hFile);
		assert(false);
		exit(1);
	}
	
	//오프셋 계산을 위한 overlapped
	OVERLAPPED ol{};							
	ol.Offset = sizeof(STAGE_HEADER);
	DWORD dwReadSize = sizeof(object_v1) * cntObj;

	bResult = ReadFile(
		hFile,
		temp,
		dwReadSize,
		NULL,
		&ol);

	if (bResult == 0)
	{
		//ReadFile 실패
		DWORD err = GetLastError();
		wchar_t buffer[256] = {};
		wsprintfW(buffer, L"[FAIL]MapLoader::Load()::ReadObject_v1() ErrorCode : %d\n", err);
		OutputDebugStringW(buffer);
		CloseHandle(hFile);
		assert(false);
		exit(1);
	}

	//Convert data
	if ( sizeof(object) == sizeof(object_v1) )
	{
		//쓸모없는 코드
		memcpy(pDest, temp, sizeof(object) * cntObj);
	}
	else
	{
		for (int i = 0; i < cntObj; i++)
		{
			pDest[i].eShaderType = temp[i].eShaderType;
			pDest[i].pos		 = temp[i].pos;
			pDest[i].oiler		 = temp[i].oiler;
			pDest[i].quaternion  = temp[i].quaternion;
			pDest[i].prevRot	 = temp[i].prevRot;
			pDest[i].scale		 = temp[i].scale;

			memcpy(pDest[i].objectName, temp[i].objectName, sizeof(WCHAR) * MAX_LENGTH);	//Object	Name
			memcpy(pDest[i].modelName,	temp[i].modelName,	sizeof(WCHAR) * MAX_LENGTH);	//Model		Name
			memcpy(pDest[i].matName,	temp[i].matName,	sizeof(WCHAR) * MAX_LENGTH);	//Material	Name

			pDest[i].modelIndex = temp[i].modelIndex;
			pDest[i].matIndex	= temp[i].matIndex;
		}
	}
	
	//Convert data (이번 버전에서 변경된 내용 적용) -> 콜라이더 타입, 오브젝트 타입
	for (int i = 0; i < cntObj; i++)
	{
		//변경하는 부분 (과거 버젼과의 차이점) -> 버젼 컨버팅하는 별도의 함수를 만들 것 고려 
		eOBJECT_TYPE_v1 typeVer1	= temp[i].type;
		eIsCollision	eCollision	= temp[i].isCollision;

		if (typeVer1 == eOBJECT_TYPE_v1::eOBJECTv1_FIELD)
		{
			if (eCollision == eIsCollision::eCollisionYes)
			{
				pDest[i].type = eOBJECT_TYPE::eOBJECT_FIELD_COLLISION_YES;				//충돌 바닥
			}
			else if (eCollision == eIsCollision::eCollisionNo)
			{
				pDest[i].type = eOBJECT_TYPE::eOBJECT_FIELD_COLLISION_NO;				//안충돌 바닥
			}
		}
		else if (typeVer1 == eOBJECT_TYPE_v1::eOBJECTv1_ENEMY_NORMAL)
		{
			pDest[i].type = eOBJECT_TYPE::eOBJECT_ENEMY_NORMAL;
		}
		else if (typeVer1 == eOBJECT_TYPE_v1::eOBJECTv1_ENEMY_RANGE)
		{
			pDest[i].type = eOBJECT_TYPE::eOBJECT_ENEMY_RANGE;
		}
		else if (typeVer1 == eOBJECT_TYPE_v1::eOBJECTv1_ENEMY_DASH)
		{
			pDest[i].type = eOBJECT_TYPE::eOBJECT_ENEMY_DASH;
		}

		pDest[i].regenIndex = -1;		//-1 is not Setting this value;
	}

	//메모리 해제(과거 버전 저장용)
	delete[] temp;		
	temp = NULL;

	return bResult;
}

BOOL MapLoader::ReadObject_v2(HANDLE& hFile, STAGE_HEADER& header, object*& pDest)
{
	DWORD dwLengthOfRead = 0;

	int cntObj = header.iObjCnt;
	if (pDest != NULL)
	{
		delete[] pDest;
		pDest = NULL;
	}

	pDest			= new object[cntObj];		//현재 버젼 오브젝트
	object_v2* temp = new object_v2[cntObj];	//과거 버젼 오브젝트

	BOOL bResult = FALSE;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER);
	DWORD dwReadSize = sizeof(object_v2) * cntObj;

	bResult = ReadFile(
		hFile,
		temp,
		dwReadSize,
		NULL,
		&ol);

	bResult = GetError(bResult);

	//Convert data
	if (sizeof(object) == sizeof(object_v2))
	{
		//두 데이터 구조가 같다.
		memcpy(pDest, temp, sizeof(object) * cntObj);
	}
	else
	{
		for (int i = 0; i < cntObj; i++)
		{
			pDest[i].regenIndex		= -1;		//voidSpace가 regenIndex로 변해서 오브젝트 사이즈 차이는 없다.(DWORD -> int)

			pDest[i].eShaderType	= temp[i].eShaderType;
			pDest[i].pos			= temp[i].pos;
			pDest[i].oiler			= temp[i].oiler;
			pDest[i].quaternion		= temp[i].quaternion;
			pDest[i].prevRot		= temp[i].prevRot;
			pDest[i].scale			= temp[i].scale;

			memcpy(pDest[i].objectName, temp[i].objectName, sizeof(WCHAR) * MAX_LENGTH);	//Object	Name
			memcpy(pDest[i].modelName, temp[i].modelName, sizeof(WCHAR) * MAX_LENGTH);	//Model		Name
			memcpy(pDest[i].matName, temp[i].matName, sizeof(WCHAR) * MAX_LENGTH);	//Material	Name

			pDest[i].modelIndex		= temp[i].modelIndex;
			pDest[i].matIndex		= temp[i].matIndex;
		}
	}

	delete[] temp;
	temp = NULL;

	return bResult;
}

BOOL MapLoader::ReadWaveAllVersion(HANDLE& hFile, STAGE_HEADER& header, int cntOfWave, waveData*& pDest)
{
	BOOL bResult = FALSE;
	int version = header.MapVersion;
	if (version == static_cast<int>(eMAP_VERSION1))
	{
		bResult = ReadWaveV1(hFile, header, cntOfWave, pDest);
		bResult = GetError(bResult);
	}

	return bResult;
}

BOOL MapLoader::ReadWaveCnt(HANDLE& hFile, STAGE_HEADER& header, int* pDest)
{
	DWORD dwLengthOfRead = 0;
	int cntWave = 0;		//받아야할 정보

	long objOffset = sizeof(object) * header.iObjCnt;
	long colOffset = sizeof(collider) * header.iColliderCnt;
	long lightOffset = sizeof(lightData) * header.iLightCnt;

	BOOL bResult = FALSE;
	OVERLAPPED ol{};
	ol.Offset = sizeof(STAGE_HEADER) + objOffset + colOffset + lightOffset;
	DWORD dwReadSize = sizeof(int);

	bResult = ReadFile(
		hFile,
		&cntWave,
		dwReadSize,
		NULL,
		&ol);

	bResult = GetError(bResult);
	*pDest = cntWave;

	return bResult;
}

BOOL MapLoader::ReadWaveV1(HANDLE& hFile, STAGE_HEADER& header, int cntOfWave, waveData*& pDest)
{
	DWORD dwLengthOfRead = 0;
	int cntWave = cntOfWave;		//받아야할 정보

	if (pDest != NULL)
	{
		delete[] pDest;
	}
	pDest = new waveData[cntWave];

	header.MapVersion;

	long headerOffset	= sizeof(STAGE_HEADER);
	long objOffset		= sizeof(object) * header.iObjCnt;
	long colOffset		= sizeof(collider) * header.iColliderCnt;
	long lightOffset	= sizeof(lightData) * header.iLightCnt;
	long waveCntOffset	= sizeof(int);

	BOOL bResult = FALSE;
	OVERLAPPED ol{};
	ol.Offset = headerOffset + objOffset + colOffset + lightOffset + waveCntOffset;
	DWORD dwReadSize = sizeof(waveData) * cntWave;

	bResult = ReadFile(
		hFile,
		pDest,
		dwReadSize,
		NULL,
		&ol);

	bResult = GetError(bResult);
	return bResult;
}
