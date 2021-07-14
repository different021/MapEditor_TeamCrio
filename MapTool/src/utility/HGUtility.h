#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

#define MAX_PATH 512

enum eTEXTURES
{
	eTEX_Albedo = 0,
	eTEX_Roughness,
	eTEX_Metallic,
	eTEX_Ao,
	eTEX_Normal,
	eTEX_Height,
	eTEXTURE_MAX,
};

extern const wchar_t* g_szStrTexW[eTEXTURE_MAX];
extern const wchar_t* g_szPngW;
extern const wchar_t* g_szDdsW;

extern const char* g_szStrTexA[eTEXTURE_MAX];
extern const char* g_szPngA;
extern const char* g_szDdsA;


struct TextureSet
{
	wchar_t tex[eTEXTURE_MAX][MAX_PATH];
};

namespace HGUtility
{
	bool GetEnvDir_A(char* szEnvDirName, char* pOut, int bufferSize);				//환경변수 획득 - 멀티바이트
	bool GetEnvDir_A(const char* szEnvDirName, char* pOut, int bufferSize);
	bool GetEnvDir_W(wchar_t* szEnvDirName, wchar_t* pOut, int bufferSize);			//환경변수 획득 - 유니코드
	bool GetEnvDir_W(const wchar_t* szEnvDirName, wchar_t* pOut, int bufferSize);

	int IsFileExists(char* szPathName);
	int IsFileExists(wchar_t* szPathName);
	int IsDirExists(char* szPath);
	int IsDirExists(wchar_t* szPath);

	int MakeFullPath(char* path, char* fileName, int bufferSize, char* pOut);
	int MakeFullPath(wchar_t* path, wchar_t* fileName, int capacityOfcharacters, wchar_t* pOut);

	void NormalizePathA(char* path, char* target, char* newSimbol, char* pOut, size_t bufSize);		//Target을 newSimbol로 변환.
	void NormalizePathW(wchar_t* pSrc, wchar_t* pTarget, wchar_t* pNewSimbol, wchar_t* pDest, size_t nbufSize);

	void CreateFolderA(char* szFolderName);

	//String converter
	DWORD Convert_UTF8_To_Unicode(wchar_t* pOutUnicode, int bufferSize, const char* pInUTF8, const int UTF8_size);
	DWORD Convert_Unicode_To_UTF8(char* pOutUTF8, const int bufferSize, const wchar_t* pInUnicode, const int unicode_Size);
	DWORD Convert_Unicode_To_MultiByte(char* &pOutChar, const wchar_t* pInUnicode, size_t uniSize);
	DWORD Convert_Unicode_To_MultiByte(char*& pOutChar, const wchar_t* pInUnicode);
	DWORD Convert_Unicode_To_MultiByte(char* pOutChar, size_t bufferSize, const wchar_t* pInUnicode, size_t uniSize);
	DWORD Convert_MutiByte_To_Unicode(wchar_t*& pOutUnicode, const char* pInChar, size_t charSize);

	
	//3d
	DirectX::XMFLOAT3 QuaternionToOiler(DirectX::SimpleMath::Quaternion q);
	
	//MFC
	void SetDlgFloat(HWND hWnd, int controllerID, float value);
	void SetDlgText(HWND hWnd, int controllerID, WCHAR* str);


};


