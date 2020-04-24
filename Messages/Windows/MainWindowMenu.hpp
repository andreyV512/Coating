#pragma once
#include "Dlg\Dialogs.h"
#include "window_tool/WindowsPosition.h"
namespace MainWindowMenu
{
struct MainFile{};
struct MainOptionUnits{};
struct SaveCoordinateWindow: WindowPositionDlg<MainWindow>{}; 
struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

template<>struct TopMenu<MainFile>
{
	typedef Vlst<
		 MenuItem<SaveDateFile>
		, Separator<0>		
	    , MenuItem<MainExit>
	> list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef Vlst<	
		MenuItem<SaveCoordinateWindow>
	> list;
};

typedef Vlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
> Menu;
//--------------------------------------------------------------------------------------------------------
MENU_TEXT(L"Файл", TopMenu<MainFile>)	
MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)

MENU_ITEM(L"Выход", MainExit)
MENU_ITEM(L"Сохранить координаты окна", SaveCoordinateWindow)
MENU_ITEM(L"Сохранить сообщения", SaveDateFile)
}
