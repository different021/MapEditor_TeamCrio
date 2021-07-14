#pragma once
#include <WinBase.h>
#include <vector>
#include <utility>

typedef std::pair<int, wchar_t*> INT_pWCHAR;
typedef std::pair<INT_pWCHAR*, INT_pWCHAR> FILE_NAME;

class CFolder
{
	int						m_FolderIndex;
	wchar_t					m_szFolderName[MAX_PATH];
	std::vector<wchar_t*>	m_FileNameList;

public:
	const wchar_t* GetFolderName();
	const wchar_t* GetFileName(int index);
	
	size_t GetNumberOfFile();
};

class CFileList
{
public:
	int						version;			
	wchar_t					m_path[MAX_PATH];
	std::vector<CFolder>	m_FolderList;

	//아래 부분 삭제
	int		m_iNumOfFolder;
	std::vector<INT_pWCHAR> m_vFolderList;
	std::vector<FILE_NAME>* m_vFileList;

	FILE_NAME* GetFile(int index);

public:
	CFileList();
	~CFileList();

	void ClearFolderList();
	void ClearFileList();

	void GetPath(wchar_t* pPath = NULL);
	void CreateFileList(int numOfFolder);

	int  AddFolder(wchar_t* szFolderName);
	int  AddFileName(int folderIndex, wchar_t* szFileName);

	int  GetFullFolderName(int index, int nBufferLength, wchar_t* pOut);
	int  GetFullFileName(int index, int nBufferLength, wchar_t* pOut);
};

