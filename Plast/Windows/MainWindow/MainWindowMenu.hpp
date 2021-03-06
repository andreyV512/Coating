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
	struct LoadDateFile : OpenFileDlg {};
	struct SaveDateFile : SaveFileDlg {};
	struct Compute__ { static void Do(HWND h) { Singleton<Compute>::Instance().Recalculation(); } };
	struct ComputeWithParam__ { static void Do(HWND h) { Singleton<Compute>::Instance().RecalculationWithParam(); } };

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
	MENU_ITEM(L"Перерасчёт с параметрами типоразмера", ComputeWithParam__)
	MENU_ITEM(L"Выход", Exit)
	//------------------------------------
	template<>struct TopMenu<MainFile>
	{
		typedef Vlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, MenuItem<Compute__>
			, MenuItem<ComputeWithParam__>
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
	struct DeadZones : DeadZonesDlg {};						 																				   //
	struct MainCreateTypesize : AddTypeSizeDlg {};
	struct MainDeleteTypeSize : DelTypeSizeDlg {};
	struct MedianFiltre : MedianFiltreDlg {};
	struct DspFiltr : DspFiltrDlg {};
	struct Tresholds : ThresholdsDlg {};

	MENU_ITEM(L"Неизмеряемые зоны", DeadZones)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Медианный фильтр", MedianFiltre)
	MENU_ITEM(L"Аналоговый фильтр", DspFiltr)
	MENU_ITEM(L"Пороги", Tresholds)

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
	struct StoreFiles : StoreFilesDlg {};

	template<>struct SubMenu<DiscretePlate>
	{
		typedef VL::TypeToTypeLst<
			Vlst<DiscriptorBitDlg, DiscretePlateInputs, DiscretePlateOutputs>
			, MenuItem
		>::Result list;
	};

	struct ComPort__ : ComPortDlg {};

	MENU_ITEM(L"Настройка аналоговой платы", __LanDlg)
	MENU_ITEM(L"Настройка инвертора", ComPort__)

	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выходные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Дескриптор дискретной платы", DiscriptorBitDlg)
	MENU_ITEM(L"Цвета", Color)
	MENU_ITEM(L"Автоматическое сохранение файлов", StoreFiles)

	struct __Unit : UnitDlg {};									  
	MENU_ITEM(L"Смешение датчика", __Unit)
	//------------------------------------
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			SubMenu<DiscretePlate>
			, MenuItem<__LanDlg>
			, MenuItem<ComPort__>
			, MenuItem<Color>
			, MenuItem<StoreFiles>
		> list;
	};
	//-------------------------------------------
	struct MainTest {};
	MENU_TEXT(L"Тест", TopMenu<MainTest>)
	struct __TestIOPorts : TestIOPorts {};
	struct AScanWindow : AScanWindowDlg {};
	struct Message_ : MessageDlg {};

	MENU_ITEM(L"Просмотр дискретных портов", __TestIOPorts)
	MENU_ITEM(L"Просмотр А-скан сигналов", AScanWindow)
	MENU_ITEM(L"Просмотр сообщений", Message_)

	struct __Zones : OpenWindow<ZonesWindow> {};
	MENU_ITEM(L"Просмотр результата по зонам", __Zones)
	//------------------------------------
	template<>struct TopMenu<MainTest>
	{
		typedef Vlst<
			MenuItem<__TestIOPorts>
			, MenuItem<AScanWindow>
			, MenuItem<Message_>
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