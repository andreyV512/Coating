#pragma once
#include "window_tool/MenuApi.h"
#include "Windows/SelectUsers/SelectUserWindow.h"

namespace MainWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"����", TopMenu<MainFile>)
		//-------------------------------------------------
		struct LoadDateFile { static void Do(HWND) {} };
	struct SaveDateFile { static void Do(HWND) {} };
	struct Compute { static void Do(HWND) {
		//Common::OpenWindow<SelectUserWindow>::Do((HBRUSH)COLOR_WINDOW);
		GridOptions<SelectUsers>::Open((wchar_t *)L"Users");
	} };

	struct Exit
	{
		static void Do(HWND h)
		{
			TClose c = { h, WM_CLOSE, 0, 0 };
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"��������� ������", LoadDateFile)
		MENU_ITEM(L"��������� ������", SaveDateFile)
		MENU_ITEM(L"����������", Compute)
		MENU_ITEM(L"�����", Exit)
		//------------------------------------
		template<>struct TopMenu<MainFile>
	{
		typedef Vlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, MenuItem<Compute>
			, Separator<0>
			, MenuItem<Exit>
		> list;
	};
	//-------------------------------------------
	typedef Vlst<
		TopMenu<MainFile>
	> Menu;

}