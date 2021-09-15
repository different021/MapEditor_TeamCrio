#include "pch.h"
#include "CSaver.h"
#include "../src/MAPLOAD.h"
#include "WMDefine.h"
#include "CColliderManager.h"
#include "CLightManager.h"
#include "CWaveManager.h"

CSaver* CSaver::m_pInstance = NULL;
//int CSaver::m_cntWave = 0;

OVERLAPPED g_OverLapped;

CSaver::CSaver()
{
	m_pDrawInsVector  = NULL;			
	m_pColliderVector = NULL;
	
	memset(&m_StageHeader, 0, sizeof(STAGE_HEADER));
	memset( &g_OverLapped, 0, sizeof(OVERLAPPED) );
}

CSaver::~CSaver()
{

}

CSaver* CSaver::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CSaver;
	}
	return m_pInstance;
}

void CSaver::CleanUp()
{
	if (m_pObjList != nullptr)
	{
		delete[] m_pObjList;
		m_pObjList = nullptr;
	}

	if (m_pColliderList != nullptr)
	{
		delete[] m_pColliderList;
		m_pColliderList = nullptr;
	}

	if (m_pLightDataList != nullptr)
	{
		delete[] m_pLightDataList;
		m_pLightDataList = nullptr;
	}

	if (m_pWaveList != nullptr)
	{
		delete[] m_pWaveList;
		m_pWaveList = nullptr;
	}

}

void CSaver::DeleteInstance()
{
	CleanUp();
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


void CSaver::Save_WINAPI(CStringW& fileFullPath, CStringW& fileName)
{
	HANDLE hFile;
	CStringW fullPath = fileFullPath;

	MapLoader::OpenMapFileToSave(hFile, fullPath.GetBuffer());

	DWORD err = GetLastError();
	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (err == ERROR_FILE_NOT_FOUND)
		{
			//OutputDebugCode(L"[FAIL]SAVE : FILE NOT FOUND\n");
			//AddLogStr(L"[FAIL]SAVE : FILE NOT FOUND");
		}
		else if (err == ERROR_SHARING_VIOLATION)
		{
			//OutputDebugCode(L"[FAIL]SAVE : The process cannot access the file because it is being used by another process \n");
			//OutputDebugCode(L"Finde error code 32. https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499- \n");
			//AddLogStr(L"[FAIL]Save Map File : The process cannot access the file because it is being used by another process. ");
		}

	}
	else
	{
		DWORD dwRet;		//Written Data size
		dwRet = WriteHeader(hFile, fileName.GetBuffer());
		dwRet = SaveObject(hFile);
		dwRet = SaveCollider(hFile);
		dwRet = SaveLight(hFile);
		dwRet = SaveWave(hFile);

	}//if (hFile == INVALID_HANDLE_VALUE) 


	::CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
	fullPath.Empty();
}


void CSaver::Load_WINAPI(CStringW& fileFullPath, CStringW& fileName)
{
	HANDLE hFile = NULL;
	CStringW fullPath = fileFullPath;
	BOOL res = MapLoader::OpenMapFileToLoad(hFile, fullPath.GetBuffer());
	fullPath.Empty();

	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringW(L"[LOAD_WINAPI] Faile to Oepn HANDLE\n");
		//assert(FALSE);
		//exit(1);
		return;
	}
	else
	{
		ReadHeader(hFile, m_StageHeader);
		ReadObject(hFile, m_StageHeader, m_pObjList);
		ReadCollider(hFile, m_StageHeader, m_pColliderList);
		ReadLight(hFile, m_StageHeader, m_pLightDataList);
		ReadWave(hFile, m_StageHeader, m_pWaveList);
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}

DWORD CSaver::WriteHeader(HANDLE hFile, wchar_t* pFileName)
{
	DWORD dwWritten;

	wcscpy_s(m_StageHeader.mapName, pFileName);
	m_StageHeader.MapVersion	= (int)(eMAP_VERSION::eMAP_VERSION_MAX) - 1;
	m_StageHeader.ObjectVersion = (int)(eOBJ_VERSION::eOBJ_VERSION_MAX) - 1;
	m_StageHeader.iCamCnt		= 0;					//수정
	m_StageHeader.iObjCnt		= m_pDrawInsVector->size();
	m_StageHeader.iColliderCnt	= m_pColliderVector->size();;
	m_StageHeader.iLightCnt		= m_pLightVector->size();


	dwWritten = MapLoader::WriteHeaderFromFile(hFile, m_StageHeader);

	return dwWritten;
}

DWORD CSaver::SaveObject(HANDLE hFile)
{
	DWORD dwRet = 0;
	size_t iObjCnt = m_pDrawInsVector->size();
	if (iObjCnt > 0)
	{
		DWORD size = sizeof(object) * iObjCnt;

		object* pBuf = new object[iObjCnt];

		for (size_t i = 0; i < iObjCnt; i++)
		{
			object* pTemp = m_pDrawInsVector->at(i)->first;
			pTemp->CopyTo(&pBuf[i]);
		}

		MapLoader::WriteObjectFromFile(hFile, pBuf, size);
		delete[] pBuf;
	}

	return dwRet;
}

DWORD CSaver::SaveCollider(HANDLE hFile)
{
	DWORD dwRet = 0;
	size_t iCountOfCollider = m_pColliderVector->size();
	if (iCountOfCollider)
	{
		DWORD size = sizeof(collider) * iCountOfCollider;
		collider* pBuf = new collider[iCountOfCollider];
		for (size_t i = 0; i < iCountOfCollider; i++)
		{
			collider* pTemp = m_pColliderVector->at(i).first;
			pTemp->SaveCollider(&pBuf[i]);
			//CopyCollider(&pBuf[i], pTemp);
		}

		dwRet = MapLoader::WriteColliderFromFile(hFile, pBuf, size);
		delete[] pBuf;
	}

	return dwRet;
}

DWORD CSaver::SaveLight(HANDLE hFile)
{
	DWORD dwRet = 0;
	size_t iLightCnt = m_pLightVector->size();
	if (iLightCnt > 0)
	{
		DWORD size = sizeof(lightData) * iLightCnt;

		lightData* pBuf = new lightData[iLightCnt];

		int i = 0;
		std::vector<Light*>::iterator it;
		for (it = m_pLightVector->begin(); it != m_pLightVector->end();)
		{
			Light* pTemp = *it;
			pTemp->OutData(pBuf[i]);
			it++;
			i++;
		}
		MapLoader::WriteLightFromFile(hFile, pBuf, size);
		//MapLoader::WriteObjectFromFile(hFile, pBuf, size);
		delete[] pBuf;
	}

	return dwRet;
}

DWORD CSaver::SaveWave(HANDLE hFile)
{
	DWORD dwRet = 0;
	int iWaveCnt = static_cast<int>(m_pWaveVector->size());
	if (iWaveCnt > 0)
	{
		DWORD size = sizeof(waveData) * iWaveCnt;		//데이터 전체 크기

		waveData* pBuf = new waveData[iWaveCnt];		//버퍼 할당.

		int i = 0;
		std::vector<WAVE*>::iterator it;
		for (it = m_pWaveVector->begin(); it != m_pWaveVector->end();)
		{
			WAVE* pTemp = *it;
			memcpy(&pBuf[i], &pTemp->info, sizeof(waveData));
			it++;
			i++;
		}
		dwRet = MapLoader::WriteWaveCountFromFile(hFile, &iWaveCnt, sizeof(int));
		dwRet = MapLoader::WriteWaveFromFile(hFile, pBuf, size);
		delete[] pBuf;
	}

	return dwRet;
}

BOOL CSaver::ReadHeader(HANDLE hFile, STAGE_HEADER& out)
{
	return MapLoader::ReadHeader(hFile, out);
}

BOOL CSaver::ReadObject(HANDLE hFile, STAGE_HEADER& header, object* &pObj)
{
	BOOL bResult = FALSE;
	int cntObj = header.iObjCnt;
	if (pObj != NULL)
	{
		delete[] pObj;
		pObj = NULL;
	}	

	bResult = MapLoader::ReadObjectAllVersion(hFile, header, pObj);
	
	return bResult;
}



BOOL CSaver::ReadCollider(HANDLE hFile, STAGE_HEADER& header, collider* &pOut)
{
	if (pOut != NULL)
	{
		delete[] pOut;
		pOut = NULL;
	}
	BOOL bResult = FALSE;
	int cntCollider = header.iColliderCnt;
	
	//bResult = MapLoader::ReadCollider(hFile, header, pOut);
	bResult = MapLoader::ReadColliderAllVersion(hFile, header, pOut);

	return TRUE;
}

BOOL CSaver::ReadLight(HANDLE hFile, STAGE_HEADER& header, lightData*& pOut)
{
	if (pOut != NULL)
	{
		delete[] pOut;
		pOut = NULL;
	}

	BOOL bResult = FALSE;
	int cntLight = header.iLightCnt;

	bResult = MapLoader::ReadLight(hFile, header, pOut);

	return bResult;
}

BOOL CSaver::ReadWave(HANDLE hFile, STAGE_HEADER& header, waveData*& pOut)
{
	if (pOut != NULL)
	{
		delete[] pOut;
		pOut = NULL;
	}

	BOOL bResult = FALSE;

	bResult = MapLoader::ReadWaveCnt(hFile, header, &m_cntWave);
	bResult = MapLoader::ReadWaveAllVersion(hFile, header, m_cntWave, pOut);

	return bResult;
}

void CSaver::SetHeader(STAGE_HEADER& header)
{
	m_StageHeader.iCamCnt = header.iCamCnt;
	m_StageHeader.iHeight = header.iHeight;
	m_StageHeader.iWidth = header.iWidth;
	m_StageHeader.iObjCnt = header.iObjCnt;
	m_StageHeader.offset = header.offset;
	wsprintfW(m_StageHeader.mapName, header.mapName);
}



void CSaver::SetObjList(std::vector<DRAW_INSTANCE*>* pDrawInsList)
{
	m_pDrawInsVector = pDrawInsList;
}


void CSaver::SetColliderList(std::vector<COLLIDER>* pColliderList)
{
	m_pColliderVector = pColliderList;
}

void CSaver::SetLightList(std::vector<Light*>* pLightList)
{
	m_pLightVector = pLightList;
}

void CSaver::SetWaveList(const std::vector<WAVE*>* pList)
{
	m_pWaveVector = (std::vector<WAVE*>*)pList;
}

//return -1 : invaild parameter
// return value is Number of instance copied
int CSaver::MakeObjectArrayFromVector(object* &pOut, const std::vector<DRAW_INSTANCE*>* pDrawInsList)
{
	object* pArray = nullptr;
	if (pDrawInsList == nullptr) return -1;		

	size_t size = pDrawInsList->size();		//복사할 사이즈
	if (size < 1) return 0;

	try
	{
		pArray = new object[size];			//저장할 메모리 할당
	}
	catch (const std::bad_alloc& e)
	{
		//메모리 할당 실패
		OutputDebugStringW(L"[CSaver]MakeObjectArrayFromVector() Fail to MemoryAllocate\n");
	}
	
	int i = 0;
	std::vector<DRAW_INSTANCE*>::const_iterator it;
	for (it = pDrawInsList->begin(); it != pDrawInsList->end(); it++)
	{
		object* pTemp = (*it)->first;
		pTemp->CopyTo(pArray + i);
		i++;
	}

	pOut = pArray;

	return i;
}

int CSaver::MakeColliderArrayFromVector(collider*& pOut, const std::vector<COLLIDER>* pColliderList)
{
	collider* pArray = nullptr;
	if (pColliderList == nullptr) return -1;

	size_t size = pColliderList->size();		//복사할 사이즈
	if (size < 1) return 0;

	try
	{
		pArray = new collider[size];			//저장할 메모리 할당
	}
	catch (const std::bad_alloc& e)
	{
		//메모리 할당 실패
		OutputDebugStringW(L"[CSaver]MakeObjectArrayFromVector() Fail to MemoryAllocate\n");
	}

	int i = 0;
	std::vector<COLLIDER>::const_iterator it;
	for (it = pColliderList->begin(); it != pColliderList->end(); it++)
	{
		collider* pTemp = (*it).first;
		pTemp->CopyTo(pArray + i);
		i++;
	}

	pOut = pArray;

	return i;
}


BOOL APIENTRY TestProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT msg = uMsg;
	switch (msg)
	{
	case WM_KEYDOWN:
		int a = 0;
		break;
	}
	return false;
}

LPCTSTR g_szMapFileFilter = L"map Files (*.stage)|*.stage|All Files (*.*)|*.*||";
LPCTSTR g_szExtFilter = L"stage";
LPCTSTR g_szExt = L".stage";

void CSaver::Save()
{
	CFileDialog saveDlg(FALSE, g_szExtFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, g_szMapFileFilter);
	if (saveDlg.DoModal() == IDOK)
	{
		CString fullpath = saveDlg.GetPathName();
		CString fileName = saveDlg.GetFileName();

		CString ext = fileName.Right(6);
		if (ext.CompareNoCase(g_szExt) != 0)		//문자열이 같을 경우 0반환
		{
			//같지 않다.
			fullpath = fullpath + g_szExt;
			fileName = fileName + g_szExt;
		}
		Save_WINAPI(fullpath, fileName);

		fullpath.Empty();
		fileName.Empty();
	}

}

int CSaver::Load(int* pOut)
{
	int iResult = 0;
	CWnd* pCWnd = CWnd::FromHandle(g_hCenter);
	CFileDialog loadDlg(TRUE, g_szExtFilter, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, g_szMapFileFilter);
	if (loadDlg.DoModal() == IDOK)
	{
		CString fullpath = loadDlg.GetPathName();
		CString fileName = loadDlg.GetFileName();

		//여기서 부터 샌드메세지 이상. ObjList + ColliderList 같이 보낼 것.
		Load_WINAPI(fullpath, fileName);


		fullpath.Empty();
		fileName.Empty();
		
		iResult = 0;		//성공
	}
	else
	{
		iResult = 1;		//사용자 취소
	}
	
	if (pOut != NULL)
		*pOut = iResult;

	return iResult;
}

