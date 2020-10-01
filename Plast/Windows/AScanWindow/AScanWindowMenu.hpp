#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace AScanWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"����", TopMenu<MainFile>)
	struct Exit
	{
		static void Do(HWND h)
		{
			TClose c = { h, WM_CLOSE, 0, 0 };
			SendMessage(MESSAGE(c));
		}
	};
	MENU_ITEM(L"�����", Exit)
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

	MENU_TEXT(L"���������", TopMenu<OptionsFile>);
	MENU_ITEM(L"������", Thresholds);
	MENU_ITEM(L"���������� ������", BiPolar);
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			MenuItem<Thresholds>
			, MenuItem<BiPolar>
		> list;
	};

	//-------------------------------------------
	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<OptionsFile>
	> Menu;

}