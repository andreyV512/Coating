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
	struct DspFiltr : TstDspFiltrDlg {};

	MENU_ITEM(L"Настройки аналогового фильтра", DspFiltr)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef Vlst<
			MenuItem<DspFiltr>
		> list;
	};


	struct Graph {};
	MENU_TEXT(L"График", TopMenu<Graph>)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
		struct Axes : ZonesAxesDlg {};

	MENU_ITEM(L"Оси графика", Axes)

		template<>struct TopMenu<Graph>
	{
		typedef Vlst<
			MenuItem<Axes>
		> list;
	};

	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Graph>
	> Menu;

}