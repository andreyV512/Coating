#include "App.h"
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "Windows/MainWindow/MainWindow.h"

namespace App
{
	void Init()
	{
		MainWindow& w = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
	}

	void Destroy()
	{
	}
	wchar_t* Salt()
	{
		return (wchar_t *)L"Coating";
	}
}