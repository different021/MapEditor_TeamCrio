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
	m_StageHeader.iCamCnt		= 0;					//����
	//m_StageHeader.iObjCnt		= m_pDrawInsVector->size();
	m_StageHeader.iObjCnt		= m_ObjectCnt;
	m_StageHeader.iColliderCnt	= m_ColliderCnt;
	m_StageHeader.iLightCnt		= m_LightCnt;


	dwWritten = MapLoader::WriteHeaderFromFile(hFile, m_StageHeader);

	return dwWritten;
}

DWORD CSaver::SaveObject(HANDLE hFile)
{
	DWORD dwRet = 0;
	//size_t iObjCnt = m_pDrawInsVector->size();
	//if (iObjCnt > 0)
	//{
	//	DWORD size = sizeof(object) * iObjCnt;

	//	object* pBuf = new object[iObjCnt];

	//	for (size_t i = 0; i < iObjCnt; i++)
	//	{
	//		object* pTemp = m_pDrawInsVector->at(i)->first;
	//		pTemp->CopyTo(&pBuf[i]);
	//	}

	//	MapLoader::WriteObjectFromFile(hFile, pBuf, size);
	//	delete[] pBuf;
	//}
	DWORD size = sizeof(object) * m_ObjectCnt;
	MapLoader::WriteObjectFromFile(hFile, m_pObjList, size);


	return dwRet;
}

DWORD CSaver::SaveCollider(HANDLE hFile)
{
	DWORD dwRet = 0;
	//size_t iCountOfCollider = m_pColliderVector->size();
	//if (iCountOfCollider)
	//{
	//	DWORD size = sizeof(collider) * iCountOfCollider;
	//	collider* pBuf = new collider[iCountOfCollider];
	//	for (size_t i = 0; i < iCountOfCollider; i++)
	//	{
	//		collider* pTemp = m_pColliderVector->at(i).first;
	//		pTemp->SaveCollider(&pBuf[i]);
	//		//CopyCollider(&pBuf[i], pTemp);
	//	}

	//	dwRet = MapLoader::WriteColliderFromFile(hFile, pBuf, size);
	//	delete[] pBuf;
	//}

	DWORD size = sizeof(collider) * m_ColliderCnt;
	dwRet = MapLoader::WriteColliderFromFile(hFile, m_pColliderList, size);

	return dwRet;
}

DWORD CSaver::SaveLight(HANDLE hFile)
{
	DWORD dwRet = 0;
	//size_t iLightCnt = m_pLightVector->size();
	//if (iLightCnt > 0)
	//{
	//	DWORD size = sizeof(lightData) * iLightCnt;

	//	lightData* pBuf = new lightData[iLightCnt];

	//	int i = 0;
	//	std::vector<Light*>::iterator it;
	//	for (it = m_pLightVector->begin(); it != m_pLightVector->end();)
	//	{
	//		Light* pTemp = *it;
	//		pTemp->OutData(pBuf[i]);
	//		it++;
	//		i++;
	//	}
	//	MapLoader::WriteLightFromFile(hFile, pBuf, size);
	//	//MapLoader::WriteObjectFromFile(hFile, pBuf, size);
	//	delete[] pBuf;
	//}

	DWORD size = sizeof(lightData) * m_LightCnt;
	MapLoader::WriteLightFromFile(hFile, m_pLightDataList, size);

	return dwRet;
}

DWORD CSaver::SaveWave(HANDLE hFile)
{
	DWORD dwRet = 0;
	//int iWaveCnt = static_cast<int>(m_pWaveVector->size());
	//if (iWaveCnt > 0)
	//{
	//	DWORD size = sizeof(waveData) * iWaveCnt;		//������ ��ü ũ��

	//	waveData* pBuf = new waveData[iWaveCnt];		//���� �Ҵ�.

	//	int i = 0;
	//	std::vector<WAVE*>::iterator it;
	//	for (it = m_pWaveVector->begin(); it != m_pWaveVector->end();)
	//	{
	//		WAVE* pTemp = *it;
	//		memcpy(&pBuf[i], &pTemp->info, sizeof(waveData));
	//		it++;
	//		i++;
	//	}
	//	dwRet = MapLoader::WriteWaveCountFromFile(hFile, &iWaveCnt, sizeof(int));
	//	dwRet = MapLoader::WriteWaveFromFile(hFile, pBuf, size);
	//	delete[] pBuf;
	//}

	dwRet = MapLoader::WriteWaveCountFromFile(hFile, &m_WaveCnt, sizeof(int));		//wave ���� ����
	DWORD size = sizeof(waveData) * m_WaveCnt;										//waveDataList ũ��
	dwRet = MapLoader::WriteWaveFromFile(hFile, m_pWaveList, size);					//waveData ����

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

	bResult = MapLoader::ReadWaveCnt(hFile, header, &m_WaveCnt);
	bResult = MapLoader::ReadWaveAllVersion(hFile, header, m_WaveCnt, pOut);

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


//return -1 : invaild parameter
// return value is Number of instance copied
int CSaver::MakeObjectArrayFromVector(const std::vector<DRAW_INSTANCE*>* pDrawInsList)
{
	object* pArray = nullptr;
	if (pDrawInsList == nullptr) return -1;		

	size_t size = pDrawInsList->size();		//������ ������
	if (size < 1) return 0;

	try
	{
		pArray = new object[size];			//������ �޸� �Ҵ�
	}
	catch (const std::bad_alloc& e)
	{
		//�޸� �Ҵ� ����
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

	m_pObjList = pArray;
	m_ObjectCnt = i;

	return i;
}

int CSaver::MakeColliderArrayFromVector(const std::vector<COLLIDER>* pColliderList)
{
	collider* pArray = nullptr;
	if (pColliderList == nullptr) return -1;
	
	size_t size = pColliderList->size();		//������ ������
	if (size < 1) return 0;

	try
	{
		pArray = new collider[size];			//������ �޸� �Ҵ�
	}
	catch (const std::bad_alloc& e)
	{
		//�޸� �Ҵ� ����
		OutputDebugStringW(L"[CSaver]MakeColliderArrayFromVector() Fail to MemoryAllocate\n");
	}

	int i = 0;
	std::vector<COLLIDER>::const_iterator it;
	for (it = pColliderList->begin(); it != pColliderList->end(); it++)
	{
		collider* pTemp = (*it).first;
		pTemp->CopyTo(pArray + i);
		i++;
	}

	m_pColliderList = pArray;
	m_ColliderCnt = i;		//�ݶ��̴� ���� ������Ʈ.

	return i;
}

int CSaver::MakeLightArrayFromVector(const std::vector<Light*>* pLightList)
{
	lightData* pArray = nullptr;
	if (pLightList == nullptr) return -1;

	size_t size = pLightList->size();		//������ ������
	if (size < 1) return 0;

	try
	{
		pArray = new lightData[size];			//������ �޸� �Ҵ�
	}
	catch (const std::bad_alloc& e)
	{
		//�޸� �Ҵ� ����
		OutputDebugStringW(L"[CSaver]MakeLightArrayFromVector() Fail to MemoryAllocate\n");
	}

	int i = 0;
	std::vector<Light*>::const_iterator it;
	for (it = pLightList->begin(); it != pLightList->end(); it++)
	{
		Light* pTemp = (*it);		//����� ���� �ִ� Ŭ������ ��.
		pTemp->CopyData(pArray[i]);
		i++;
	}

	m_pLightDataList = pArray;
	m_LightCnt = i;

	return i;
}

int CSaver::MakeWaveArrayFromVector(const std::vector<WAVE*>* pWaveList)
{
	waveData* pArray = nullptr;
	if (pWaveList == nullptr) return -1;

	size_t size = pWaveList->size();		//������ ������
	if (size < 1) return 0;

	try
	{
		pArray = new waveData[size];			//������ �޸� �Ҵ�
	}
	catch (const std::bad_alloc& e)
	{
		//�޸� �Ҵ� ����
		OutputDebugStringW(L"[CSaver]MakeWaveArrayFromVector() Fail to MemoryAllocate\n");
	}

	int i = 0;
	std::vector<WAVE*>::const_iterator it;
	for (it = pWaveList->begin(); it != pWaveList->end(); it++)
	{
		WAVE* pTemp = (*it);		//����� ���� �ִ� Ŭ������ ��.
		pTemp->CopyTo(pArray + i);
		i++;
	}

	m_pWaveList = pArray;
	m_WaveCnt = i;

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
		if (ext.CompareNoCase(g_szExt) != 0)		//���ڿ��� ���� ��� 0��ȯ
		{
			//���� �ʴ�.
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

		//���⼭ ���� ����޼��� �̻�. ObjList + ColliderList ���� ���� ��.
		Load_WINAPI(fullpath, fileName);


		fullpath.Empty();
		fileName.Empty();
		
		iResult = 0;		//����
	}
	else
	{
		iResult = 1;		//����� ���
	}
	
	if (pOut != NULL)
		*pOut = iResult;

	return iResult;
}

