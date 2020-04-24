#include "App.h"
#include "Windows/MainWindow.h"
#include "window_tool/EmptyWindow.h"
#include "tools_debug/DebugMess.h"
#include "Log/LogBuffer.h"
#include "window_tool/WindowsPosition.h"

App::App()
{
	HWND hh = FindWindow(WindowClass<MainWindow>()(), 0);
	if (NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		static const int width = 500;
		static const int height = 400;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		hh = WindowTemplate(new MainWindow, (wchar_t *)L"События"
			, (r.right - width) / 2
			, (r.bottom - height) / 2
			, width
			, height
			, IDI_settings
			, (HBRUSH)COLOR_WINDOW
		);
		LONG_PTR s = GetWindowLongPtr(hh, GWL_STYLE);
		s &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
		SetWindowLongPtr(hh, GWL_STYLE, s);
		SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hh, SW_SHOWNORMAL);
	}
}

void App::Destroy()
{
}