#pragma once

#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace ZonesWindowMenu
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
	//------------------------------------
	template<>struct TopMenu<MainFile>
	{
		typedef Vlst<
			MenuItem<Exit>
		> list;
	};
	//---------------------------------------
	struct MainOptionTypeSize {};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct DspFiltr : DspFiltrDlg {};

	MENU_ITEM(L"Настройки аналогового фильтра", DspFiltr)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef Vlst<
			MenuItem<DspFiltr>
		> list;
	};

	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
	> Menu;

}