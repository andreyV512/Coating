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
	struct DeadZones : TestDeadZonesDlg {};
	struct DspFiltr : TstDspFiltrDlg {};
	struct Thresholds : TestThreshDlg {};
	struct MedianFltr : TstMedianFiltreDlg {};
	struct BiPolar : TestBiPolarDlg {};
	struct XinMM__ : TestXinMMDlg {};
	struct Gain__ : TestGainDlg {};

	MENU_ITEM(L"Неизмеряемые зоны", DeadZones)
	MENU_ITEM(L"Аналоговый фильтр", DspFiltr)
	MENU_ITEM(L"Медианный фильтр", MedianFltr)
	MENU_ITEM(L"Пороги", Thresholds)
	MENU_ITEM(L"Биполярный сигнал", BiPolar)
	MENU_ITEM(L"Ось Х в мм.", XinMM__);
	MENU_ITEM(L"График усиления", Gain__);

	template<>struct EnableMenuInit<MenuItem<XinMM__>>{int operator()(HWND){return MFS_CHECKED;}};

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef Vlst<
			MenuItem<DeadZones>
			, MenuItem<Thresholds>
			, MenuItem<MedianFltr>
			, MenuItem<DspFiltr>
			, Separator<0>
			, MenuItem<BiPolar>
			, MenuItem<XinMM__>
			, MenuItem<Gain__>
		> list;
	};

	typedef Vlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
	> Menu;

}