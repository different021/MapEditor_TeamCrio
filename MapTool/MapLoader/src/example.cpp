#include <stdio.h>
#include <assert.h>

#include "Defines.h"
#include "MAPLOAD.h"

#include "utility/HgUtility.h"
#include "utility/CTextReader.h"


#define MAX_BUFFER 256

FILE* stream, * stream2;

int main()
{
	//
	//MAP* pMap;
	////MapLoader::Load((WCHAR*)(L"C:\\Users\\kocca\\Desktop\\map\\test.map"), pMap);
	//MapLoader::Load((L"E:\\svn_fin\\1_Executable\\MapTool\\maptoolVer2\\Media\\Stage\\S_stage1.stage"), pMap);
	//MapLoader::DeleteMap(pMap);
	//

	const wchar_t* pFileName = L"newfile.txt";
	std::vector<wchar_t*> fList;

	CTextReader* pReader = new CTextReader;
	pReader->OpenTxt(L"newfile.txt");
	//pReader->OpenTxt(pFileName);
	int Filesize = pReader->GetFileSize();
	printf("File Size : %d\n", Filesize);	
	pReader->ReadTxt(NULL);
	pReader->GetFileList(fList);

	pReader->CloseTxt();
	
	int a = 0;
	
	std::vector<wchar_t*>::iterator it;
	for (it = fList.begin(); it != fList.end(); it++)
	{
		wprintf(L"[LIST] ");
		wchar_t* pStr = *it;
		wprintf(pStr);
		wprintf(L"\n");
	}


	int numcolsed = _fcloseall();
	printf("Number of files closed by _fcloseall: %u\n", numcolsed);

	for (it = fList.begin(); it != fList.end();)
	{
		delete[] *it;
		it = fList.erase(it);
	}

	return 0;

	/*delete[] pTest;
	pTest = NULL;*/

	/*
	errno_t  err;
	FILE* fp;
	err = fopen_s(&fp, "newfile.txt", "a+, ccs=UTF-8");
	if (err != 0)
	{
		int a = 0;
		if (fp)
		{
			err = fclose(fp);
		}
	}

	wchar_t buffer[MAX_BUFFER];
	memset( buffer, 0, sizeof(wchar_t) * MAX_BUFFER );

	if (fp)
	{
		size_t dwWritten = fread_s(buffer, MAX_BUFFER * sizeof(wchar_t), sizeof(wchar_t), MAX_BUFFER, fp);
		fclose(fp);
	}
	
	*/


	/*
	err = _wfopen_s(&stream, L"E:/svn_fin/5_Project/MapTool/ObjectLoacator/Media/Model/modelList.txt", L"r");
	if (err != 0)
	{
		//Fail to Open file
		//assert(FALSE);
		int a = 0;
	}


	err = _wfopen_s(&stream2, L"modelList.txt", L"w+");
	if (err != 0)
	{
		//Fail to Open file
		int a = 0;
	}


	if (stream)
	{
		err = fclose(stream);
		if (err != 0)
		{
			//file not Colse;
			int a = 0;
		}
		else
		{
			int a = 0;
			//file Close
		}
	}
	*/

}

