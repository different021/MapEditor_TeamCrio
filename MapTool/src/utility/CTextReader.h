#pragma once

#include <stdio.h>
#include <vector>

#define BUFFER_MAX 512
#define SMALL 3

struct FILELIST_HEADER
{
	int version;
	int numOfFile;
};

//개선 필요 버퍼 사이즈 이상 읽어오기
class CTextReader
{
	wchar_t		m_Buffer[BUFFER_MAX];
	wchar_t*	m_pRead;
	FILE*		m_pFile;

private:
	void ClearBuffer();

public:
	bool OpenTxt(wchar_t* szFileName);
	bool OpenTxt(const wchar_t szFileName[]);
	bool CloseTxt();
	
	size_t  ReadTxt(wchar_t* dest);
	size_t  GetFileList(std::vector<wchar_t*>& fList);
	size_t  MakeFileListFromFolder(wchar_t* pFolderName, std::vector<wchar_t*>& fileList);
	size_t  WriteTxt(wchar_t* Out);
	int		GetFileSize();

};

