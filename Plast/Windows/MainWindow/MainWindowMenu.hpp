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
	//----------------------------------------------
	struct MainOptionTypeSize {};
	MENU_TEXT(L"����������", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct DeadZones { static void Do(HWND) {} };						 																				   //
	struct MainCreateTypesize : AddTypeSizeDlg {};
	struct MainDeleteTypeSize : DelTypeSizeDlg {};
	struct MedianFiltre__ { static void Do(HWND) {} };
	struct DspFiltr : DspFiltrDlg {};
	struct Tresholds { static void Do(HWND) {} };

	MENU_ITEM(L"̸����� ����", DeadZones)
	MENU_ITEM(L"������� ����������", MainCreateTypesize)
	MENU_ITEM(L"������� ����������", MainDeleteTypeSize)
	MENU_ITEM(L"��������� ������", MedianFiltre__)
	MENU_ITEM(L"��������� ����������� �������", DspFiltr)
	MENU_ITEM(L"��������� �������", Tresholds)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef Vlst<
			MenuItem<DeadZones>
			, MenuItem<Tresholds>
			, MenuItem<MedianFiltre__>
			, MenuItem<DspFiltr>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		> list;
	};
	//-------------------------------------------
	struct OptionsFile {};
	MENU_TEXT(L"���������", TopMenu<OptionsFile>)
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

	MENU_ITEM(L"��������� ���������� �����", __LanDlg)

	MENU_TEXT(L"���������� �����", SubMenu<DiscretePlate>)
	MENU_ITEM(L"������� �����", DiscretePlateInputs)
	MENU_ITEM(L"�������� �����", DiscretePlateOutputs)
	MENU_ITEM(L"���������� ���������� �����", DiscriptorBitDlg)

	struct __LirDlg : LirDlg {};
	MENU_ITEM(L"������ ���", __LirDlg)
	//------------------------------------
	template<>struct TopMenu<OptionsFile>
	{
		typedef Vlst<
			SubMenu<DiscretePlate>
			, MenuItem<__LanDlg>
			, MenuItem<__LirDlg>
		> list;
	};
	//-------------------------------------------
	struct MainTest {};
	MENU_TEXT(L"����", TopMenu<MainTest>)
	struct __TestIOPorts : TestIOPorts {};
	struct AScanWindow : AScanWindowDlg {};
	struct Message_ : MessageDlg {};
	struct TestLir_ : TestLirDlg {};

	MENU_ITEM(L"�������� ���������� ������", __TestIOPorts)
	MENU_ITEM(L"�������� �-���� ��������", AScanWindow)
	MENU_ITEM(L"�������� ���������", Message_)
	MENU_ITEM(L"�������� �������� ���", TestLir_)
	//------------------------------------
	template<>struct TopMenu<MainTest>
	{
		typedef Vlst<
			MenuItem<__TestIOPorts>
			, MenuItem<AScanWindow>
			, MenuItem<Message_>
			, MenuItem<TestLir_>
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
		, TopMenu<MainOptionTypeSize>
		, TopMenu<MainTest>
		, TopMenu<OptionsFile>
		, TopMenu<MainAbout>
	> Menu;

}