#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace AScanWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	struct LoadDateFile { static void Do(HWND) {} };
	struct SaveDateFile {
		static void Do(HWND) {
			//GridOptions<SelectUsers>::Open((wchar_t *)L"Users");
		}
	};
	struct Compute : OperatorsDlg {};

	struct Exit
	{
		static void Do(HWND h)
		{
			TClose c = { h, WM_CLOSE, 0, 0 };
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile)
		MENU_ITEM(L"Сохранить данные", SaveDateFile)
		MENU_ITEM(L"Перерасчёт", Compute)
		MENU_ITEM(L"Выход", Exit)
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