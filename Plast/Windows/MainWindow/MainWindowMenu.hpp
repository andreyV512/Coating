#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace MainWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"����", TopMenu<MainFile>)
	struct LoadDateFile { static void Do(HWND) {} };
	struct SaveDateFile {
		static void Do(HWND) {}
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
	struct TypeSizeFile {};
	MENU_TEXT(L"����������", TopMenu<TypeSizeFile>)
	struct TypeSize { static void Do(HWND) {} };

	MENU_ITEM(L"����������", TypeSize)
	//------------------------------------
	template<>struct TopMenu<TypeSizeFile>
	{
		typedef Vlst<
			MenuItem<TypeSize>
		> list;
	};
	//-------------------------------------------
	struct OptionsFile {};
	MENU_TEXT(L"���������", TopMenu<OptionsFile>)
	struct __LanDlg : LanDlg {};

	MENU_ITEM(L"��������� ���������� �����", __LanDlg)
	//------------------------------------
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			MenuItem<__LanDlg>
		> list;
	};
	//-------------------------------------------
	struct MainAbout : AboutWindowDlg {};
	MENU_TEXT(L"� ���������", TopMenu<MainAbout>)
	MENU_ITEM(L"� ���������", MainAbout)
	//-----------------------------------
	template<>struct TopMenu<MainAbout>
	{
		typedef Vlst<
			MenuItem<MainAbout>
		> list;
	};
	//-----------------------------------------
	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<OptionsFile>
		, TopMenu<MainAbout>
	> Menu;

}