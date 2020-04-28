#pragma once
#include "Base/TablesDefine.h"
DEFINE_PARAM(LirDescriptor, int, 1)
DEFINE_PARAM(TickPerZoneLir0, double, 1.046)
DEFINE_PARAM(TickPerZoneLir1, double, 1.046)

struct LirParamTable
{
	typedef Vlst<
		LirDescriptor
		, TickPerZoneLir0
		, TickPerZoneLir1
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"LirParamTable"; }
};
