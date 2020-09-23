#include "App.h"
#include <CommCtrl.h>
#include "window_tool/Emptywindow.h"
#include "window_tool/WindowsPosition.h"
#include "window_tool/HookKey.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AppBase.h"
#include "Dlg/Dlg.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "Units/Lan/Lan.h"
#include "Automat/Automat.h"
#include "Log/LogMessageToTopLabel.h"
#include "Units/Lir/Lir.h"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/ItemIni.h"
#include "window_tool/OpenWindow.hpp"
namespace App
{
	void Init()
	{
		Performance::Init();
		AppBase().Init();
#if 1
		MainWindow& w = Singleton<MainWindow>::Instance();
		//ZonesWindow &w = Singleton<ZonesWindow>::Instance();
		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);

	//	w.EnableMenu(false);

		wchar_t name[64];
		CurrentOperatorName(name);
		StatusBar(App::operator_status_section, name);

		StartKeyHook(&AppKeyHandler::KeyPressed);
 //инициализация АЦП
		Lan &l = Singleton<Lan>::Instance();
	    //l.SetHandler
		RshInitMemory p{};
		l.SetParams(p);
		wchar_t mess[256];
		U32 st = l.Init(1, l.device1, p);
		if (l.Err(st, mess)) 
		{
			MessageBox(h, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(1)!!!", MB_ICONEXCLAMATION);
			return;
		}
		st = l.Init(2, l.device2, p);
		if (l.Err(st, mess))
		{
			MessageBox(h, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(2)!!!", MB_ICONEXCLAMATION);
			return;
		}

		Lir &lir = Singleton<Lir>::Instance();

		if (!lir.Init(Singleton<LirParamTable>::Instance().items.get<LirDescriptor>().value))
		{
			MessageBox(h, L"Плата не инициализирована", (wchar_t *)L"Ошибка платы ЛИР", MB_ICONEXCLAMATION);
			return;
		}

		Singleton<LogMessageToTopLabel>::Instance().Run();
		Automat::Init();
		
#else
		//DspFiltrDlg::Do(0);
		OpenTopWindow<ZonesWindow>::Do(NULL);
#endif
	}

	void Destroy()
	{
		Singleton<LogMessageToTopLabel>::Instance().Stop();
		Automat::Destroy();
	}
	void TopLabel(wchar_t(&mess)[128])
	{
		MainWindow &w = Singleton<MainWindow>::Instance();
		w.topLabelViewer.SetMessage((wchar_t *)mess);
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
	bool __run__ = false;
	bool &IsRun() { return __run__; }
}

Device1730 device1730;