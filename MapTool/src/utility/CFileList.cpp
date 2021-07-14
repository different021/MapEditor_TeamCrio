#include "CFileList.h"
//#include <Windows.h>
#include <wchar.h>

FILE_NAME* CFileList::GetFile(int index)
{
	FILE_NAME* pResult = NULL;
	if (index < 0)	return pResult;

	for (int i = 0; i < m_iNumOfFolder; i++)
	{
		std::vector<FILE_NAME>::iterator it;
		for (it = m_vFileList[i].begin(); it != m_vFileList[i].end(); it++)
		{
			if (it->second.first == index)
			{
				pResult = &(*it);
				return pResult;
			}
		}
	}

	return pResult;
}

CFileList::CFileList()
{
}

CFileList::~CFileList()
{
	ClearFileList();
	ClearFolderList();
}

void CFileList::ClearFolderList()
{
	std::vector<INT_pWCHAR>::iterator it;
	for (it = m_vFolderList.begin(); it != m_vFolderList.end(); )
	{
		wchar_t* pName = (*it).second;
		delete pName;
		pName = NULL;
		*it->second = NULL;

		it = m_vFolderList.erase(it);
	}
}

void CFileList::ClearFileList()
{
	std::vector<FILE_NAME>::iterator it;
	for (int i = 0; i < m_iNumOfFolder; i++)
	{
		for (it = m_vFileList[i].begin(); it != m_vFileList[i].end();)
		{
			(*it).first		= NULL;
			wchar_t* pName	= (*it).second.second;

			delete pName;
			pName = NULL;
			*it->second.second = NULL;

			it = m_vFileList[i].erase(it);
		}
	}

	delete[] m_vFileList;
	m_vFileList = NULL;
}

void CFileList::GetPath(wchar_t* szPath)
{
	if (szPath == NULL)
	{
		::GetCurrentDirectoryW(MAX_PATH, m_path);
		wcsncat_s(m_path, MAX_PATH, L"\\", MAX_PATH);
	}
	else
	{
		size_t size = wcsnlen_s(szPath, MAX_PATH);
		if (size > MAX_PATH)
		{
			OutputDebugStringW(L"[FAIL] : path is Too Long\n");
			assert(false);
		}

		errno_t err = wmemcpy_s(m_path, MAX_PATH, szPath, size);
		if (err != 0)
		{
			wchar_t buffer[256];
			wmemset(buffer, 0, 256);
			wprintf(buffer, L"[FAIL: Copy PATH ]CODE : %d", err);
			OutputDebugStringW(buffer);
		}
	}

}

//FolderNameList가 완성되고 만들어야 유효하다.
void CFileList::CreateFileList(int numOfFolder)
{
	if (numOfFolder < 0) return;
	m_iNumOfFolder = numOfFolder;
	m_vFileList = new std::vector<FILE_NAME>[m_iNumOfFolder];
}

int CFileList::AddFolder(wchar_t* szFolderName)
{
	if (!szFolderName) return -1;		//잘못된 파라미터
	static int index = 0;				//인덱스
	m_vFolderList.push_back(INT_pWCHAR(index, szFolderName));
	
	return index++;
}

//성공시 해당 인덱스 반환.
//return -1 은 첫번째 파라미터 오류
//return -2 은 두번째 파라미어 오류
int CFileList::AddFileName(int folderIndex, wchar_t* szFileName)
{
	if (folderIndex < 0)					return -1;	
	if (m_vFileList->size() < folderIndex)	return -1;
	if (!szFileName)						return -2;
	static int index = 0;
	INT_pWCHAR* folder = &m_vFolderList[(folderIndex)];
	m_vFileList[folderIndex].push_back( FILE_NAME(folder, INT_pWCHAR(index, szFileName)) );

	return index++;
}

/*
	길이 리턴
*/
int CFileList::GetFullFolderName(int index, int nBufferLength, wchar_t* pOut)
{
	if (pOut == NULL)		return 0;
	if (nBufferLength < 0)	return 0;
	
	wmemcpy_s(pOut, nBufferLength, m_path, wcsnlen_s(pOut, MAX_PATH) );
	int size = wcsnlen_s(pOut, MAX_PATH);
	return size;
}

int CFileList::GetFullFileName(int index, int nBufferLength, wchar_t* pOut)
{
	if (pOut == NULL)		return 0;
	if (nBufferLength < 0)	return 0;

	FILE_NAME* pFile	= GetFile(index);
	wchar_t* pFolderName= pFile->first->second;
	wchar_t* pFileName	= pFile->second.second;

	size_t pathSize		= wcsnlen_s(m_path, MAX_PATH);
	size_t folderSize	= wcsnlen_s(m_path, MAX_PATH);
	size_t fileSize		= wcsnlen_s(m_path, MAX_PATH);
	
	size_t total		= pathSize + folderSize + fileSize;

	if (total > MAX_PATH)
	{
		OutputDebugStringW(L"[Lack BUffer Size]\n");
		assert(false);
		return 0;
	}

	wmemcpy_s(pOut, nBufferLength, m_path,		wcsnlen_s(pOut, MAX_PATH));
	wcsncat_s(pOut, nBufferLength, pFolderName, wcsnlen_s(pFolderName, MAX_PATH) );
	wcsncat_s(pOut, nBufferLength, L"\\",		MAX_PATH);
	wcsncat_s(pOut, nBufferLength, pFileName,	wcsnlen_s(pFileName, MAX_PATH) );

	size_t size = wcsnlen_s(pOut, MAX_PATH);
	return size;
}
