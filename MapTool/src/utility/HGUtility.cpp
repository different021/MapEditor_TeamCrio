#include "HGUtility.h"

#include <memory.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


#pragma warning(disable:4996)

const wchar_t* g_szStrTexW[eTEXTURE_MAX] =
{
	L"_abedo",
	L"_roughness",
	L"_metallic",
	L"_ao",
	L"_normal",
	L"_height",
};
const wchar_t* g_szPngW = L".png";
const wchar_t* g_szDdsW = L".dds";

const char* g_szStrTexA[eTEXTURE_MAX] =
{
	"_abedo",
	"_roughness",
	"_metallic",
	"_ao",
	"_normal",
	"_height",
};
const char* g_szPngA = ".png";
const char* g_szDdsA = ".dds";

bool HGUtility::GetEnvDir_A(char* szEnvDirName, char* pOut, int bufferSize)
{
	return GetEnvDir_A((const char*)szEnvDirName, pOut, bufferSize);
}

bool HGUtility::GetEnvDir_A(const char* szEnvDirName, char* pOut, int bufferSize)
{
	char* buf = NULL;
	size_t sz = 0;
	bool bResult = false;

	int  res = _dupenv_s(&buf, &sz, szEnvDirName);
	if (res == 0 && buf != NULL)
	{
		size_t sizeOfPath = strlen(buf);
		if (sizeOfPath > bufferSize)	return bResult;		//실패
		else
		{
			errno_t res = strcpy_s(pOut, bufferSize, buf);
			if (res == 0)	bResult = true;					//성공
		}

		printf("EnvVarName = %s\n", buf);
		free(buf);
	}

	return bResult;
}


bool HGUtility::GetEnvDir_W(wchar_t* szEnvDirName, wchar_t* pOut, int bufferSize)
{
	return GetEnvDir_W((const wchar_t*)szEnvDirName, pOut, bufferSize);
}

bool HGUtility::GetEnvDir_W(const wchar_t* szEnvDirName, wchar_t* pOut, int bufferSize)
{
	wchar_t* buf = NULL;
	size_t sz = 0;
	bool bResult = false;

	int  res = _wdupenv_s(&buf, &sz, szEnvDirName);
	if (res == 0 && buf != NULL)
	{
		size_t sizeOfPath = wcslen(buf);
		if (sizeOfPath > bufferSize)	return bResult;		//실패
		else
		{
			errno_t res = wcscpy_s(pOut, bufferSize, buf);
			if (res == 0)	bResult = true;					//성공
		}

		wprintf(L"EnvVarName = %s\n", buf);
		free(buf);
	}

	return bResult;
}


//return 1 when File Exist
int HGUtility::IsFileExists(char* szPathName)
{
	_finddatai64_t c_file;
	intptr_t hFile;
	int result = 0;

	hFile = _findfirst64(szPathName, &c_file);
	if (hFile == -1L)							//파일이 없다
		result = 0;
	else if (c_file.attrib & _A_SUBDIR)			//있어도 디렉토리 이름이면 거짓
		return 0;
	else
		return 1;		//존재한다 ^오^

	_findclose(hFile);

	return result;
}



int HGUtility::IsFileExists(wchar_t* szPathName)
{
	_wfinddatai64_t c_file;
	intptr_t hFile;
	int result = 0;

	hFile = _wfindfirst64(szPathName, &c_file);
	if (hFile == -1L)							//파일이 없다
		result = 0;
	else if (c_file.attrib & _A_SUBDIR)			//있어도 디렉토리 이름이면 거짓
		return 0;
	else
		return 1;		//존재한다 ^오^

	_findclose(hFile);

	return result;
}


//return 1 when Dir Exist
int HGUtility::IsDirExists(char* szDir)
{
	_finddatai64_t c_file;
	intptr_t hFile;
	int result = 0;

	hFile = _findfirst64(szDir, &c_file);
	if (c_file.attrib & _A_SUBDIR)
		result = 1;

	_findclose(hFile);

	return result;
}


int HGUtility::IsDirExists(wchar_t* szDir)
{
	_wfinddatai64_t c_file;
	intptr_t hFile;
	int result = 0;

	hFile = _wfindfirst64(szDir, &c_file);
	if (c_file.attrib & _A_SUBDIR)
		result = 1;

	_findclose(hFile);

	return result;
}


int HGUtility::MakeFullPath(char* path, char* fileName, int bufferSize, char* pOut)
{
	int iResult = 0;
	size_t sizeOfPath = strlen(path);
	size_t sizeOfName = strlen(fileName);

	if ((sizeOfPath + sizeOfName) > bufferSize) return iResult;	//버퍼 사이즈가 부족하다.
	if (fileName == NULL) return iResult;	//파일이름이 없당.
	if (pOut == NULL) return iResult;		//버퍼가 없당.

	strcat(pOut, path);
	strcat(pOut, fileName);
	iResult = 1;

	return iResult;
}

int HGUtility::MakeFullPath(wchar_t* path, wchar_t* fileName, int capacityOfcharacters, wchar_t* pOut)
{
	int iResult = 0;
	size_t sizeOfPath = wcslen(path);
	size_t sizeOfName = wcslen(fileName);

	if ((sizeOfPath + sizeOfName) > capacityOfcharacters) return iResult;	//버퍼 사이즈가 부족하다.
	if (fileName == NULL) return iResult;	//파일이름이 없당.
	if (pOut == NULL) return iResult;		//버퍼가 없당.

	wcscat(pOut, path);
	wcscat(pOut, fileName);
	iResult = 1;

	return iResult;
}

void HGUtility::NormalizePathA(char* pIn, char* pTarget, char* newSimbol, char* pOut, size_t bufSize)
{
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	char* DoubleSlash = pTarget;
	size_t sizeOfTarget = strlen(DoubleSlash);

	char* changeTo = newSimbol;
	size_t sizeOfSibol = strlen(changeTo);

	size_t size = strlen(pIn);
	char* pResult = pOut;
	memset(pResult, '\0', bufSize);

	char* pStrRead = pIn;

	int iWritePos = 0;
	for (int iReadPos = 0; iReadPos < size;)
	{
		bool bIsSame = true;
		for (int j = 0; j < sizeOfTarget; j++)
		{
			if (pStrRead[j] != DoubleSlash[j])
			{
				bIsSame = false;
				break;
			}
		}

		if (bIsSame)
		{
			for (int k = 0; k < sizeOfSibol; k++)
			{
				memset(pResult + iWritePos + k, changeTo[k], sizeof(char));
			}

			iReadPos += sizeOfTarget;
			pStrRead += sizeOfTarget;
			iWritePos += sizeOfSibol;
		}
		else
		{
			memset(pResult + iWritePos, *pStrRead, sizeof(char));
			iReadPos++;
			pStrRead += 1;
			iWritePos += 1;
		}

	}

	memset(pResult + iWritePos, '\n', sizeof(char));	//마지막에 널문자
}

void HGUtility::NormalizePathW(wchar_t* pSrc, wchar_t* pTarget, wchar_t* pNewSimbol, wchar_t* pDest, size_t nBufSize)
{
	if (pSrc == NULL) return;
	if (pDest == NULL) return;

	wchar_t* DoubleSlash = pTarget;
	size_t sizeOfTarget = wcslen(DoubleSlash);

	wchar_t* simbols = pNewSimbol;
	size_t sizeOfSimbol = wcslen(simbols);

	size_t size = wcslen(pSrc);
	wchar_t* pResult = pDest;
	size_t sizeT = wcslen(pDest);
	wmemset(pResult, L'\0', nBufSize);

	wchar_t* pRead = pSrc;

	int writePos = 0;
	for (int readPos = 0; readPos < size;)
	{
		bool bIsSame = true;
		for (int j = 0; j < sizeOfTarget; j++)
		{
			if (pRead[j] != DoubleSlash[j])
			{
				bIsSame = false;
				break;
			}
		}

		if (bIsSame)
		{
			for (int k = 0; k < sizeOfSimbol; k++)
			{
				wmemcpy(pResult + writePos + k, (&simbols[k]), sizeof(wchar_t));
			}

			readPos += sizeOfTarget;
			pRead += sizeOfTarget;
			writePos += sizeOfSimbol;
		}
		else
		{
			wmemcpy(pResult + writePos, pRead, sizeof(wchar_t));
			readPos++;
			pRead += 1;
			writePos++;
		}

	}
	pResult[writePos] = L'\0';
	//wmemset(pResult + writePos, L'\0', sizeof(wchar_t));	//마지막에 널문자
}



void HGUtility::CreateFolderA(char* pFolderName)
{
	//파일 경로 생성.
	if ((_access(pFolderName, 0)) == -1)
	{
		//OutputDebugStringA(L"Create Folder\n");
		CreateDirectoryA(pFolderName, NULL);
	}

}



DWORD HGUtility::Convert_UTF8_To_Unicode(wchar_t* pOutUnicode, int bufferSize, const char* pInUTF8, const int UTF8_size)
{
	DWORD error = 0;

	do {
		if (pOutUnicode == NULL)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		if ((pInUTF8 == nullptr) || (UTF8_size == 0))
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		//CONVERT
		int result = ::MultiByteToWideChar(
			CP_UTF8,
			0,
			pInUTF8,
			static_cast<int>(UTF8_size),
			pOutUnicode,
			bufferSize);

		if (result == 0)
		{
			error = ::GetLastError();
			break;
		}

	} while (false);

	return 0;
}


DWORD HGUtility::Convert_Unicode_To_UTF8(char* pOutUTF8, const int bufferSize, const wchar_t* pInUnicode, const int unicode_Size)
{
	DWORD error = 0;

	do {
		if (pOutUTF8 == NULL)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		if ((pInUnicode == nullptr) || (unicode_Size == 0))
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		//CONVERT
		int result = ::WideCharToMultiByte(
			CP_UTF8,
			0,
			pInUnicode,
			static_cast<int>(unicode_Size),
			pOutUTF8,
			bufferSize,
			NULL, NULL);			//UTF8일 경우 반듯이 NULL로 해줄 것.

		if (result == 0)
		{
			error = ::GetLastError();
			break;
		}

	} while (false);

	return 0;
}


DWORD HGUtility::Convert_Unicode_To_MultiByte(char*& pOutChar, const wchar_t* pInUnicode, size_t uniSize)
{
	DWORD error = 0;

	do {
		if (pOutChar != NULL)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		if ((pInUnicode == nullptr))
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		int required = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			NULL, 0,
			NULL, NULL
		);

		if (required == 0)
		{
			error = ::GetLastError();
			break;
		}

		int size = required + 1;

		pOutChar = new char[size];
		memset(pOutChar, 0, size);

		//CONVERT
		int result = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			pOutChar,
			size,
			NULL, NULL);

		if (result == 0)
		{
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}


DWORD HGUtility::Convert_Unicode_To_MultiByte(char*& pOutChar, const wchar_t* pInUnicode)
{
	DWORD error = 0;
	size_t uniSize = wcslen(pInUnicode);

	do {
		if (pOutChar != NULL)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		if ((pInUnicode == nullptr))
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		int required = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			NULL, 0,
			NULL, NULL
		);

		if (required == 0)
		{
			error = ::GetLastError();
			break;
		}

		int size = required + 1;

		pOutChar = new char[size];
		memset(pOutChar, 0, size);

		//CONVERT
		int result = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			pOutChar,
			size,
			NULL, NULL);

		if (result == 0)
		{
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD HGUtility::Convert_Unicode_To_MultiByte(char* pOutChar, size_t bufferSize, const wchar_t* pInUnicode, size_t uniSize)
{
	DWORD error = 0;

	do {
		if (pOutChar == NULL)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		if ((pInUnicode == nullptr))
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}

		memset(pOutChar, 0, bufferSize);

		int required = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			NULL, 0,
			NULL, NULL
		);

		if (required == 0)
		{
			error = ::GetLastError();
			break;
		}

		//CONVERT
		int result = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pInUnicode,
			static_cast<int>(uniSize),
			pOutChar,
			required,
			NULL, NULL);

		if (result == 0)
		{
			error = ::GetLastError();
			break;
		}

	} while (false);

	return error;
}

DWORD HGUtility::Convert_MutiByte_To_Unicode(wchar_t*& pOutUnicode, const char* pInChar, size_t charSize)
{
	return 0;
}


DirectX::XMFLOAT3 HGUtility::QuaternionToOiler(DirectX::SimpleMath::Quaternion q) {
	DirectX::XMFLOAT3 angles;

	double yaw = atan2(2.0 * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	double pitch = asin(-2.0 * (q.x * q.z - q.w * q.y));
	double roll = atan2(2.0 * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
	double rad = 180.f / DirectX::XM_PI;
	angles.x = yaw * rad;
	angles.y = pitch * rad;
	angles.z = roll * rad;

	return angles;
}

#include <atlstr.h>
void HGUtility::SetDlgFloat(HWND hWnd, int controllerID, float value)
{
	CString ret;
	ret.Format(L"%0.2f", value);
	SetDlgItemTextW(hWnd, controllerID, ret);
	//GetDlgItem(hWnd, controllerID);
	ret.ReleaseBuffer();
	ret.Empty();
}

void HGUtility::SetDlgText(HWND hWnd, int controllerID, WCHAR* str)
{
	SetDlgItemTextW(hWnd, controllerID, str);
}

