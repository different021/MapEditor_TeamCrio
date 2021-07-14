#pragma once

#include <crtdbg.h>
#include <cassert>
#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

class CheckMemoryLeakForWin32
{
public:
    CheckMemoryLeakForWin32() {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }
    ~CheckMemoryLeakForWin32() {
        assert(_CrtCheckMemory());
    }
}CheckMemoryLeakForWin32;

#endif
