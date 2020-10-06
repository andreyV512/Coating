#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dlg.h"
#include "window_tool/OpenWindow.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "Compute/Compute.h"

namespace MainWindowMenu
{
	struct MainFile {};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	struct LoadDateFile { static void Do(HWND) {} };
	struct SaveDateFile {
		static void Do(HWND) {}
	};
	struct Compute__ { static void Do(HWND h) { Singleton<Compute>::Instance().Recalculation(); } };

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
	MENU_ITEM(L"Перерасчёт", Compute__)
	MENU_ITEM(L"Выход", Exit)
	//------------------------------------
	template<>struct TopMenu<MainFile>
	{
		typedef Vlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, MenuItem<Compute__>
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
	//----------------------------------------------
	struct MainOptionTypeSize {};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct DeadZones { static void Do(HWND) {} };						 																				   //
	struct MainCreateTypesize : AddTypeSizeDlg {};
	struct MainDeleteTypeSize : DelTypeSizeDlg {};
	struct MedianFiltre : MedianFiltreDlg {};
	struct DspFiltr : DspFiltrDlg {};
	struct Tresholds : ThresholdsDlg {};

	MENU_ITEM(L"Мёртвые зоны", DeadZones)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Медианный фильтр", MedianFiltre)
	MENU_ITEM(L"Аналоговый фильтр", DspFiltr)
	MENU_ITEM(L"Настройки порогов", Tresholds)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef Vlst<
			MenuItem<DeadZones>
			, MenuItem<Tresholds>
			, MenuItem<MedianFiltre>
			, MenuItem<DspFiltr>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		> list;
	};
	//-------------------------------------------
	struct OptionsFile {};
	MENU_TEXT(L"Настройки", TopMenu<OptionsFile>)
		struct __LanDlg : LanDlg {};

	struct DiscretePlate {};
	struct DiscretePlateInputs : InputBitDlg {};
	struct DiscretePlateOutputs : OutputBitDlg {};
	struct Color : ColorDlg {};

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
	MENU_ITEM(L"Цвета", Color)

	//struct __LirDlg : LirDlg {};
	//MENU_ITEM(L"Датчик ЛИР", __LirDlg)
	struct __Unit : UnitDlg {};									  
	MENU_ITEM(L"Смешение датчика", __Unit)
	//------------------------------------
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			SubMenu<DiscretePlate>
			, MenuItem<__LanDlg>
			//, MenuItem<__LirDlg>
			, MenuItem<__Unit>
			, MenuItem<Color>
		> list;
	};
	//-------------------------------------------
	struct MainTest {};
	MENU_TEXT(L"Тест", TopMenu<MainTest>)
	struct __TestIOPorts : TestIOPorts {};
	struct AScanWindow : AScanWindowDlg {};
	struct Message_ : MessageDlg {};
//	struct TestLir_ : TestLirDlg {};

	MENU_ITEM(L"Просмотр дискретных портов", __TestIOPorts)
	MENU_ITEM(L"Просмотр А-скан сигналов", AScanWindow)
	MENU_ITEM(L"Просмотр сообщений", Message_)
//	MENU_ITEM(L"Просмотр датчиков ЛИР", TestLir_)

	struct __Zones : OpenTopWindow<ZonesWindow> {};
	MENU_ITEM(L"Просмотр результата по зонам", __Zones)
	//------------------------------------
	template<>struct TopMenu<MainTest>
	{
		typedef Vlst<
			MenuItem<__TestIOPorts>
			, MenuItem<AScanWindow>
			, MenuItem<Message_>
		//	, MenuItem<TestLir_>
			, Separator<0>
			, MenuItem<__Zones>
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
		, TopMenu<MainOptionTypeSize>
		, TopMenu<MainTest>
		, TopMenu<OptionsFile>
		, TopMenu<MainAbout>
	> Menu;

}