#pragma once
#include <string>	
#include <vector>

#define STR_MAX 512
struct FOLDER_INFO;

struct FILE_INFO
{
	int index;
	FOLDER_INFO*	pFolder;	//���� ������ 
	std::wstring	fileName;	//���ϸ�
	std::wstring	fileExt;	//Ȯ���� ��
	std::wstring	fullPath;	//��ü ���
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
	
	std::vector<FOLDER_INFO*>	m_FolderList;		//���� ����Ʈ
	std::vector<FILE_INFO*>		m_pFileList;		//���� ����Ʈ

public:
	//����.
	~CFileManager();
	void Cleanup();
	void CleanupFile();
	void CleanupFolder();

	//�ʱ�ȭ.
	void Initialize(std::wstring folderName, std::wstring ext);
	void SetExtension(wchar_t* szExt);
	void SetCurrentFolder();
	void SetFolderList(std::wstring& folderName);
	void SetFileListInFolders();

	size_t GetNumberOfFolder();			//���� ����
	size_t GetNumberOfFile();			//���� �� ����
	size_t GetNumberOfFileInFolder(std::wstring folder); //�ش� ���� ������ ���� ����

	const std::vector<FILE_INFO*>& GetFileInfoList();		//����Ʈ����.

	std::wstring GetFileName(int index);
	std::wstring GetFilePath(int index);				//������ fullPath

	std::string GetFileNameA(int index);
	std::string GetFilePathA(int index);

	std::wstring GetFilePath(std::wstring fileName);	//������ fullPat -> �����̸����� Ž��
	
};

