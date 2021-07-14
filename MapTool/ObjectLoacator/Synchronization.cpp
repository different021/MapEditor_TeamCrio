#include "pch.h"
#include "Synchronization.h"

Synchronization::Synchronization()
{
	hEvent = CreateEvent(NULL, TRUE, FALSE, _T("Wait") );
}

Synchronization::~Synchronization()
{
	End();
}

void Synchronization::Stop()
{
	if (!SetEvent(hEvent))
	{
		WCHAR buf[50];
		wsprintfW(buf, L"SetEvent failed (%d)\n", GetLastError());
		OutputDebugStringW(buf);
		return;
	}
}

void Synchronization::Go()
{
	//¸®¼Â?\

}

void Synchronization::WaitPoint()
{
	//waitforSingle
}

void Synchronization::End()
{
	CloseHandle(hEvent);
}
