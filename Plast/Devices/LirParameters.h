#pragma once
#include "Base/TablesDefine.h"
DEFINE_PARAM(LirDescriptor, int, 1)
DEFINE_PARAM(TickPerMMLir0, double, 1.046)
DEFINE_PARAM(TickPerMMLir1, double, 1.046)

struct LirParamTable
{
	typedef Vlst<
		LirDescriptor
		, TickPerMMLir0
		, TickPerMMLir1
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"LirParamTable"; }
};
