#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"

namespace MainWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
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
	struct TypeSizeFile {};
	MENU_TEXT(L"Типоразмер", TopMenu<TypeSizeFile>)
	struct TypeSize { static void Do(HWND) {} };

	MENU_ITEM(L"Типоразмер", TypeSize)
	//------------------------------------
	template<>struct TopMenu<TypeSizeFile>
	{
		typedef Vlst<
			MenuItem<TypeSize>
		> list;
	};
	//-------------------------------------------
	struct OptionsFile {};
	MENU_TEXT(L"Настройки", TopMenu<OptionsFile>)
		struct __LanDlg : LanDlg {};

	struct DiscretePlate {};
	struct DiscretePlateInputs : InputBitDlg {};
	struct DiscretePlateOutputs : OutputBitDlg {};

	template<>struct SubMenu<DiscretePlate>
	{
		typedef VL::TypeToTypeLst<
			Vlst<DiscriptorBitDlg, DiscretePlateInputs, DiscretePlateOutputs>
			, MenuItem
		>::Result list;
	};

	MENU_ITEM(L"Настройка аналоговой платы", __LanDlg)

	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выходные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Дескриптор дискретной платы", DiscriptorBitDlg)
	//------------------------------------
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			SubMenu<DiscretePlate>
			, MenuItem<__LanDlg>
		> list;
	};
	//-------------------------------------------
	struct MainTest {};
	MENU_TEXT(L"Тест", TopMenu<MainTest>)
	struct __TestIOPorts : TestIOPorts {};
	struct AScanWindow : AScanWindowDlg {};

	MENU_ITEM(L"Просмотр дискретных портов", __TestIOPorts)
	MENU_ITEM(L"Просмотр А-скан сигналов", AScanWindow)
	//------------------------------------
	template<>struct TopMenu<MainTest>
	{
		typedef Vlst<
			MenuItem<__TestIOPorts>
			, MenuItem<AScanWindow>
		> list;
	};
	//-------------------------------------------
	struct MainAbout : AboutWindowDlg {};
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	MENU_ITEM(L"О программе", MainAbout)
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
		, TopMenu<MainTest>
		, TopMenu<OptionsFile>
		, TopMenu<MainAbout>
	> Menu;

}