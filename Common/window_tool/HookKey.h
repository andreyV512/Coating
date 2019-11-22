#pragma once
#include "windows.h"
#undef HOOK_KEY_DLL
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall SetHWND(HWND h);
#pragma comment (linker, "/export:SetHWND=_SetHWND@4")
extern "C"__declspec(dllexport) void __stdcall StartKeyHook(HWND h);
#pragma comment (linker, "/export:StartKeyHook=_StartKeyHook@4")
#else
void  StartKeyHook(void(*)(unsigned));
#endif
;


