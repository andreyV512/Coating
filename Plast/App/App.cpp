#include "App.h"
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "window_tool/HookKey.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AppBase.h"
#include "Windows/GridWindow/Test.h"
#include "Dlg/Dlg.h"
namespace App
{
	void Init()
	{
		AppBase().Init();

#if 0
		MainWindow& w = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);

		StartKeyHook(&AppKeyHandler::KeyPressed);
#else
		DspFiltrDlg::Do(0);
		TestTest();
#endif
	}

	void Destroy()
	{
	}
	void TopLabel(wchar_t(&)[128])
	{
	}
	wchar_t* Salt()
	{
		return (wchar_t *)L"Coating";
	}
}

Device1730 device1730;