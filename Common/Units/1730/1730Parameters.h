#pragma once
#include "Base/TablesDefine.h"

DEFINE_PARAM(iCU, unsigned, 1 << 0) //���� ����������
DEFINE_PARAM(iIn, unsigned, 1 << 1)
DEFINE_PARAM(iOut, unsigned, 1 << 2)

struct InputBitsTable
{
	typedef Vlst<
		iCU
		, iIn
		, iOut
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"InputBitsTable"; }
};

DEFINE_PARAM(oAutomat, unsigned, 1 << 0)
DEFINE_PARAM(oSupply, unsigned, 1 << 1)
DEFINE_PARAM(oMark, unsigned, 1 << 2)

struct OutputBitsTable
{
	typedef Vlst<
		oAutomat
		, oSupply
		, oMark
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"OutputBitsTable"; }
};

STR_PARAM(NamePlate1730, 64, L"PCI-1730,BID#12")
struct NamePlate1730ParametersTable
{
	typedef Vlst<
		NamePlate1730
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"NamePlate1730ParametersTable"; }
};