#include "CTextReader.h"
#include <Windows.h>	
#include <strsafe.h>
#include <wchar.h>
#include <tchar.h>
#include <assert.h>

void CTextReader::ClearBuffer()
{
	memset(m_Buffer, 0, sizeof(wchar_t) * BUFFER_MAX);
	m_pRead = m_Buffer;
}

bool CTextReader::OpenTxt(wchar_t* szFileName)
{
	bool bResult = true;
	errno_t err = _wfopen_s(&m_pFile, (const wchar_t*)szFileName, L"rt, ccs=UTF-8");
	if (err != 0)
	{
		fclose(m_pFile);
		bResult = false;
		OutputDebugStringW(L"[FAIL]File Open\n");
	}

	return bResult;
}

bool CTextReader::CloseTxt()
{
	bool bResult = false;
	if (m_pFile)
	{
		fclose(m_pFile);
		bResult = true;
	}
	
	int numOfClose = _fcloseall();
	if (numOfClose > 0)
	{
		//추가적으로 닫아준 갯수.	
		wsprintf(m_Buffer, L"추가적으로 닫아준 갯수 : %d", numOfClose);
		OutputDebugStringW(m_Buffer);
		ClearBuffer();
	}

	return bResult;
}

size_t CTextReader::ReadTxt(wchar_t* dest)
{
	//assert(m_pFile == NULL);
	size_t lRead = 0;
	ClearBuffer();

	if (m_pFile)
	{
		int elementSize = sizeof(wchar_t);
		int count = BUFFER_MAX;
		int size = elementSize * BUFFER_MAX;

		lRead = fread_s(m_Buffer, size, elementSize, count, m_pFile);
	}

	return lRead;
}

size_t CTextReader::GetFileList(std::vector<wchar_t*>& fList)
{
	//assert(m_pFile == NULL);
	wchar_t temp[BUFFER_MAX];
	memset(temp, 0, BUFFER_MAX * sizeof(wchar_t));

	int dwWritten	 = 0;
	int fileSize = GetFileSize();

	int iSizeOfName = 0;
	while (true)
	{
		
		if (m_Buffer[dwWritten] == L'\n')
		{
			temp[dwWritten] = L'\0';
			int size = (iSizeOfName + 1) * sizeof(wchar_t);
			wchar_t* pFileName = new wchar_t[iSizeOfName + 1];
			memcpy_s(pFileName, size, temp, size);
			memset(&temp, 0, iSizeOfName * sizeof(wchar_t));
		
			fList.push_back(pFileName);

			iSizeOfName = 0;
			dwWritten++;
			

			if (m_Buffer[dwWritten] == L'\0') break;
			continue;
		}
		if (fileSize + 1 == dwWritten)
		{
			temp[dwWritten] = L'\0';
			int size = (iSizeOfName + 1) * sizeof(wchar_t);
			wchar_t* pFileName = new wchar_t[iSizeOfName + 1];
			memcpy_s(pFileName, size, temp, size);
			memset(&temp, 0, iSizeOfName * sizeof(wchar_t));

			fList.push_back(pFileName);

			iSizeOfName = 0;
			dwWritten++;
			break;
		}

		temp[iSizeOfName] = m_Buffer[dwWritten];

		iSizeOfName++;
		dwWritten++;

	}


	int size = fList.size();
	printf("size = %d\n\n[LIST]\n", size);

	return 0;
}

//해당 폴더 내부의 모든 파일을 리스트로 만들어 준다.
size_t CTextReader::MakeFileListFromFolder(wchar_t* pFolderName, std::vector<wchar_t*>& fileList)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	StringCchCatW(pFolderName, MAX_PATH, L"\\*.png");		//모든 png파일 대상.

	hFind = FindFirstFileW(pFolderName, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//FAIL
		assert(FALSE);
		return 0;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)		//파일만 검색
		{
			//OutputDebugStringW(FindFileData.cFileName);
			//OutputDebugStringW(L"\n");
			size_t length = wcsnlen_s(FindFileData.cFileName, 255) + 1;
			size_t size = length * sizeof(wchar_t);
			wchar_t* buffer = new wchar_t[length];
			memcpy_s(buffer, size, FindFileData.cFileName, size);
			buffer[length - 1] = L'\0';

			fileList.push_back(buffer);
		}
	} 	
	while (FindNextFileW(hFind, &FindFileData) != NULL);

	FindClose(hFind);

	return fileList.size();;
}



//Not Implments
size_t CTextReader::WriteTxt(wchar_t* Out)
{
	size_t lWritten = 0;
	assert(FALSE);

	return lWritten;
}

int CTextReader::GetFileSize()
{
	int iSize = 0;
	int curPos = fseek(m_pFile, SEEK_CUR, SEEK_END);
	
	fseek(m_pFile, 0, SEEK_END);
	iSize = ftell(m_pFile);
	fseek(m_pFile, curPos, SEEK_SET);

	return iSize;
}

