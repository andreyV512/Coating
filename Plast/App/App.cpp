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
#include "MessageText/status.h"
#include "Compute/Compute.h"
#include "../LanProcess/LanDirect/LanDirect.h"
#include "App/Config.h"
#include "tools_debug/DebugMess.h"
#include "Devices/LanDevice.h"
#include "window_tool/ProgrammDir.h"
#include "Data/StoreBase.h"

template<class O, class P>struct __params__
{
	void operator()(O &o)
	{
		Wchar_from<typename O::type_value> p(o.value);
		dprint("app %S %S\n", o.name(), p());
	}
};

namespace App
{
	unsigned(&packet_size_buffer())[6]
	{
		static unsigned arr[6] = {218, 474, 986, 2010, 4058, 8154 };
		return arr;
	}

	void Init()
	{
		unsigned start = GetTickCount();
		Performance::Init();
		AppBase().Init();
		Singleton<Compute>::Instance().Start();

		ProgrammDir pd;
		wsprintf(pd.tail, L"\\..\\Store\\");
		CreateDirectory(pd.path, NULL);

	///	Store::Init();

#ifndef INNER_LAN
		Singleton<LanDevice>::Instance().Reload();
#endif
		LanParametersTable &table = Singleton<LanParametersTable>::Instance();
		VL::foreach<LanParametersTable::items_list, __params__>()(table.items);

		StatusData::UpdateColor();
#if 1
		MainWindow& w = Singleton<MainWindow>::Instance();
		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(&w, (wchar_t *)L"Control", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);

		wchar_t name[64];
		CurrentOperatorName(name);
	//	StatusBar(App::operator_status_section, name);

		StartKeyHook(&AppKeyHandler::KeyPressed);
		Singleton<LogMessageToTopLabel>::Instance().Run();

		unsigned stop = GetTickCount();
		dprint("time load %d\n", stop - start);

		if (!device1730.Init(Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value))
		{
			MessageBox(h, L"???? ???????? ???????????????????????? ?????????? 1730", L"????????????!!!", MB_ICONERROR);
			return;
		}
 //?????????????????????????? ??????
#ifdef INNER_LAN
		Lan &l = Singleton<Lan>::Instance();
		RshInitMemory p{};
		l.SetParams(p);
		wchar_t mess[256];
		U32 st = l.Init(1, l.device1, p);
		if (RSH_API_SUCCESS != st)
		{
			l.Err(st, mess);
			MessageBox(h, mess, (wchar_t *)L"???????????? ?????????? La-n108-100PCI(1)!!!", MB_ICONEXCLAMATION);
			return;
		}
		st = l.Init(2, l.device2, p);
		if (RSH_API_SUCCESS != st)
		{
			l.Err(st, mess);
			MessageBox(h, mess, (wchar_t *)L"???????????? ?????????? La-n108-100PCI(2)!!!", MB_ICONEXCLAMATION);
			return;
		}
#else
		//Singleton <LanRead>::Instance();
#endif

		
		Automat::Init();
		
#else
		//DspFiltrDlg::Do(0);
		OpenTopWindow<ZonesWindow>::Do(NULL);
#endif
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
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