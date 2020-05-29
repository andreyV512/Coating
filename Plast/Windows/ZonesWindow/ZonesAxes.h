#pragma once
#include "Base/TablesDefine.h"

DEFINE_PARAM(AxeYTop, double, 5)
DEFINE_PARAM(AxeYBottom, double, -5)
DEFINE_PARAM(AxeXWidth, int, 500)
struct ZoneWindowsAxes
{
	typedef Vlst<
		AxeYTop
		, AxeYBottom
		, AxeXWidth
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"ZoneWindowsAxes"; }
};