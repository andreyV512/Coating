#include "window_tool/HookKey.h"

#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include "tools_debug\DebugMess.h"

#include "window_tool/message.h"

HHOOK hKeyHook;
void dump(unsigned){}
void(*__proc)(unsigned) = &dump;
 
__declspec(dllexport) LRESULT CALLBACK KeyEvent (
  int nCode,      // The hook code
  WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
  KBDLLHOOKSTRUCT *hooked   // A pointer to a struct with information about the pressed key
) {
    if  ((nCode == HC_ACTION) &&       // HC_ACTION means we may process this event
        ((wParam == WM_SYSKEYDOWN) ||  // Only react if either a system key ...
        (wParam == WM_KEYDOWN)))       // ... or a normal key have been pressed.
    {
		(*__proc)((unsigned)hooked->vkCode);
    }
    return CallNextHookEx(hKeyHook, nCode, wParam, (LPARAM)hooked);
}

DWORD WINAPI KeyLogger(LPVOID lpParameter)
{
    hKeyHook = SetWindowsHookEx (  // install the hook:
        WH_KEYBOARD_LL,            // as a low level keyboard hook
        (HOOKPROC) KeyEvent,       // with the KeyEvent function from this executable
        (HINSTANCE)::GetModuleHandle(NULL),                      // and the module handle to our own executable
        NULL                       // and finally, the hook should monitor all threads.
    );
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    UnhookWindowsHookEx(hKeyHook);
    return 0;
}

#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall StartKeyHook(HWND h)
#else
void StartKeyHook(void(*proc)(unsigned))
#endif
{
	__proc = proc;
	CloseHandle(CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)KeyLogger, NULL, NULL, NULL));
}
