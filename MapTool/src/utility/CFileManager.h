#pragma once
#include <string>	
#include <vector>

#define STR_MAX 512
struct FOLDER_INFO;

struct FILE_INFO
{
	int index;
	FOLDER_INFO*	pFolder;	//폴더 포인터 
	std::wstring	fileName;	//파일명
	std::wstring	fileExt;	//확장자 명
	std::wstring	fullPath;	//전체 경로
};


struct FOLDER_INFO
{
	int index;
	std::wstring			folderName;
	std::wstring			fullPath;
	std::vector<FILE_INFO*>	fileList;
};


class CFileManager
{
	bool			m_bFindOnlyTargetExtension = false;
	std::wstring	m_CurrentDirectory;
	std::wstring	m_TargetFolder;
	std::wstring	m_Extension;
	
	std::vector<FOLDER_INFO*>	m_FolderList;		//폴더 리스트
	std::vector<FILE_INFO*>		m_pFileList;		//파일 리스트

public:
	//삭제.
	~CFileManager();
	void Cleanup();
	void CleanupFile();
	void CleanupFolder();

	//초기화.
	void Initialize(std::wstring folderName, std::wstring ext);
	void SetExtension(wchar_t* szExt);
	void SetCurrentFolder();
	void SetFolderList(std::wstring& folderName);
	void SetFileListInFolders();

	size_t GetNumberOfFolder();			//폴더 갯수
	size_t GetNumberOfFile();			//파일 총 갯수
	size_t GetNumberOfFileInFolder(std::wstring folder); //해당 폴더 내부의 파일 갯수

	const std::vector<FILE_INFO*>& GetFileInfoList();		//리스트제공.

	std::wstring GetFileName(int index);
	std::wstring GetFilePath(int index);				//파일의 fullPath

	std::string GetFileNameA(int index);
	std::string GetFilePathA(int index);

	std::wstring GetFilePath(std::wstring fileName);	//파일의 fullPat -> 파일이름으로 탐색
	
};

