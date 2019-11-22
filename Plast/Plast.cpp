// Plast.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
#include <commctrl.h>
#include "Base/Base.hpp"
#include "App/App.h"
//#include "window_tool/HookKey.h"
#include <gdiplus.h>
//#include "tcp/InitTcp.h"
//#include "tools_debug\DebugMess.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

wchar_t typeWindow[] = L"plast_190304";

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CreateSemaphore(0, 0, 1, typeWindow);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(typeWindow, 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}
	INITCOMMONCONTROLSEX* iccx = new INITCOMMONCONTROLSEX;
	iccx->dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx->dwICC = 0;
	InitCommonControlsEx(iccx);

	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput* gdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, gdiplusStartupInput, NULL);

	Initialize initialize;
	App::Init();

    MSG msg;

    // Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	App::Destroy();

	return (int)msg.wParam;
}

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")
