#include <Windows.h>

HANDLE g_hFile;
OVERLAPPED g_OL;

//unsigned char BYTE 
DWORD ReadData(BYTE* pBuff, DWORD nToRead)
{
	DWORD dwRead;
	DWORD dwError;
	DWORD dwErrorFlag;
	COMSTAT comstat;
	
	bool bResult = ReadFile(g_hFile, pBuff, nToRead, &dwRead, &g_OL);
	if (!bResult)
	{
		dwError = GetLastError();
		if (dwError == ERROR_IO_PENDING)
		{
			while ( !GetOverlappedResult(g_hFile, &g_OL, &dwRead, TRUE) )
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(g_hFile, &dwErrorFlag, &comstat);
					break;
				}
			}

		}
	}
	else
	{
		dwRead = 0;
		ClearCommError(g_hFile, &dwErrorFlag, &comstat);
	}

	return dwRead;
}