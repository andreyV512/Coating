#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace AScanWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	struct Exit
	{
		static void Do(HWND h)
		{
			TClose c = { h, WM_CLOSE, 0, 0 };
			SendMessage(MESSAGE(c));
		}
	};
	MENU_ITEM(L"Выход", Exit)
	template<>struct TopMenu<MainFile>
	{
		typedef Vlst<
			MenuItem<Exit>
		> list;
	};
	//----------------------------------------------
	struct OptionsFile {};
	struct Thresholds : ThreshDlg {};
	struct BiPolar : BiPolarDlg {};
	struct DspFiltr : AScanDspFiltrDlg {};

	MENU_TEXT(L"Настройки", TopMenu<OptionsFile>);
	MENU_ITEM(L"Пороги", Thresholds);
	MENU_ITEM(L"Аналоговый фильтр", DspFiltr);
	MENU_ITEM(L"Биполярный сигнал", BiPolar);
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			MenuItem<Thresholds>
			, MenuItem<DspFiltr>
			, Separator<0>
			, MenuItem<BiPolar>
		> list;
	};

	//-------------------------------------------
	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<OptionsFile>
	> Menu;

}