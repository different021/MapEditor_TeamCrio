#include "pch.h"
#include "CFileManager.h"
#include <filesystem>
#include <string.h>

CFileManager::~CFileManager()
{
	Cleanup();
}

void CFileManager::Cleanup()
{
	CleanupFile();
	CleanupFolder();
}

void CFileManager::CleanupFile()
{
	//��������
	std::vector<FILE_INFO*>::iterator file_it;
	for (file_it = m_pFileList.begin(); file_it != m_pFileList.end();)
	{
		FILE_INFO* pFile = *file_it;

		delete pFile;
		pFile = NULL;

		file_it = m_pFileList.erase(file_it);
	}
}

void CFileManager::CleanupFolder()
{
	//Folder ����
	std::vector<FOLDER_INFO*>::iterator folder_it;
	for (folder_it = m_FolderList.begin(); folder_it != m_FolderList.end();)
	{
		FOLDER_INFO* pFolder = *folder_it;

		delete pFolder;
		pFolder = NULL;

		folder_it = m_FolderList.erase(folder_it);
	}
}

void CFileManager::Initialize(std::wstring folderName, std::wstring ext)
{
	SetCurrentFolder();
	SetExtension((wchar_t*)ext.c_str());
	SetFolderList(folderName);
	SetFileListInFolders();
}

//NULL�� ��� ��� ���� �˻�
void CFileManager::SetExtension(wchar_t* szExt)
{
	if (szExt != NULL)
	{
		m_Extension = szExt;
		if (m_Extension.size() != 0)
		{
			m_bFindOnlyTargetExtension = true;
		}
	}
	else
	{
		m_Extension = L"";
		m_bFindOnlyTargetExtension = false;
	}

}

void CFileManager::SetCurrentFolder()
{
	m_CurrentDirectory = std::filesystem::current_path().c_str();
}

void CFileManager::SetFolderList(std::wstring& folderName)
{
	if (m_CurrentDirectory.size() == 0)
		SetCurrentFolder();
	static int folderIndex = 0;

	m_TargetFolder = folderName;

	std::filesystem::directory_iterator it( std::filesystem::current_path() / folderName.c_str() );
	while (it != std::filesystem::end(it))
	{
		const std::filesystem::directory_entry& entry = *it;
		if(entry.is_directory() == true)	//���丮�� ��쿡��.
		{
			std::wstring fullPath = entry.path().c_str();
			std::size_t  slash = fullPath.rfind(L"\\");
			std::size_t  end = fullPath.size();
			std::wstring folderName = fullPath.substr(slash + 1, end - 1);
			
			//����
			FOLDER_INFO* pFolder = new FOLDER_INFO;
			pFolder->folderName = folderName;
			pFolder->fullPath = fullPath;
			pFolder->index = folderIndex;
			folderIndex++;

			m_FolderList.push_back(pFolder);
		}
		it++;
	}
}

void CFileManager::SetFileListInFolders()
{
	static int fileIndex	= 0;
	std::vector<FOLDER_INFO*>::iterator f_iter;
	for (f_iter = m_FolderList.begin(); f_iter != m_FolderList.end(); f_iter++)
	{
		FOLDER_INFO* pFolder = *f_iter;
		//std::vector<std::wstring>::iterator file_iter;
		std::filesystem::path path = std::filesystem::current_path();
		std::wstring folderName = m_TargetFolder + L"\\" + pFolder->folderName;
		std::filesystem::directory_iterator it(path / folderName.c_str());
		while (it != std::filesystem::end(it))
		{
			const std::filesystem::directory_entry& entry = *it;
			if (entry.is_regular_file() == true)		//������ ��쿡��.
			{
				//1. ���� ���� (�̸�, Ȯ����, �ε���, ���� ������)
				//2. ���� �Ŵ����� �߰�
				//3. ������ ������ �߰�.
				
				//���� �о���� & ����
				std::wstring fullPath	= entry.path();
				std::size_t slash		= fullPath.rfind(L"\\");
				std::size_t formatName  = 0;						

				std::size_t dotPoint    = fullPath.rfind(L".");		//��.
				std::size_t endPoint	= fullPath.rfind(L"\0");	//���κ�.
				std::wstring fileName	= fullPath.substr(slash + 1, dotPoint - slash - 1);
				std::wstring ext		= fullPath.substr(dotPoint + 1, dotPoint - endPoint - 1);

				if (m_bFindOnlyTargetExtension == true)
				{
					int comp = _wcsicmp(m_Extension.c_str(), ext.c_str());
					if (comp != 0)
					{
						it++;
						continue;
					}
				}

				//1. ���� ����
				FILE_INFO* pFile = new FILE_INFO;
				pFile->fullPath = fullPath;
				pFile->fileName = fileName;
				//pFile->fileExt  = m_Extension; 
				pFile->fileExt = ext;
				pFile->pFolder  = pFolder;
				pFile->index	= fileIndex;
				fileIndex++;
				
				//2. ���� �Ŵ��� �߰�
				m_pFileList.push_back(pFile);

				//3. ������ �߰�.
				pFolder->fileList.push_back(pFile);
				
			}
			it++;
		}
			
	}

}

size_t CFileManager::GetNumberOfFolder()
{
	return m_FolderList.size();
}

size_t CFileManager::GetNumberOfFile()
{
	return m_pFileList.size();
}

size_t CFileManager::GetNumberOfFileInFolder(std::wstring folder)
{
	size_t result = 0;
	size_t targetSize = folder.size();
	std::vector<FOLDER_INFO*>::iterator it;
	for (it = m_FolderList.begin(); it != m_FolderList.end(); it++)
	{
		FOLDER_INFO* pFolder = *it;
		std::wstring* pFolderName = &pFolder->folderName;
		if ( targetSize != pFolderName->size())
			continue;

		if (_wcsicmp(folder.c_str(), pFolderName->c_str()) == 0)
		{
			result = pFolder->fileList.size();
			break;
		}
	}
	return result;
}

const std::vector<FILE_INFO*>& CFileManager::GetFileInfoList()
{
	return m_pFileList;
}

std::wstring CFileManager::GetFileName(int index)
{
	return m_pFileList.at(index)->fileName;
}

std::wstring CFileManager::GetFilePath(int index)
{
	return m_pFileList.at(index)->fullPath;
}

std::string CFileManager::GetFileNameA(int index)
{
 	std::wstring wName = m_pFileList.at(index)->fileName;
	std::string aPath;
	aPath.assign( wName.begin(), wName.end() );
	return aPath;
}

std::string CFileManager::GetFilePathA(int index)
{
	std::wstring wPath = m_pFileList.at(index)->fullPath;
	std::string aPath;
	aPath.assign(wPath.begin(), wPath.end());
	return aPath;
}

