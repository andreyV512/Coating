#include "App.h"
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "window_tool/HookKey.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AppBase.h"
namespace App
{
	void Init()
	{
		AppBase().Init();
		MainWindow& w = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);

		StartKeyHook(&AppKeyHandler::KeyPressed);
	}

	void Destroy()
	{
	}
	wchar_t* Salt()
	{
		return (wchar_t *)L"Coating";
	}
}