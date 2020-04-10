#include "App.h"
#include <CommCtrl.h>
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "window_tool/HookKey.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AppBase.h"
#include "Dlg/Dlg.h"
namespace App
{
	void Init()
	{
		AppBase().Init();

#if 1
		MainWindow& w = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);

		wchar_t name[64];
		CurrentOperatorName(name);
		StatusBar(App::operator_status_section, name);

		StartKeyHook(&AppKeyHandler::KeyPressed);
#else
		//DspFiltrDlg::Do(0);
		TestTest();
#endif
	}

	void Destroy()
	{
	}
	void TopLabel(wchar_t(&)[128])
	{
	}

	void StatusBar(int id, wchar_t *mess)
	{
		MainWindow &w = Singleton<MainWindow>::Instance();
		SendMessage(w.hStatuisBar, SB_SETTEXT, id, (LPARAM)mess);
	}

	void CurrentOperatorName(wchar_t(&name)[64])
	{
		name[0] = '\0';
		CBase base((wchar_t *)ParametersBase().name());
		if (base.IsOpen())
		{
			UserTable t;
			int id = Singleton<CurrentParametersTable>::Instance().items.get<CurrentUserNameID>().value;
			Select<UserTable>(base).ID(id).Execute(t);
			wcsncpy(name, t.items.get<UserName>().value, dimention_of(name));
		}
	}

	wchar_t *Salt()
	{
		return (wchar_t *)L"Coating";
	}
}

Device1730 device1730;