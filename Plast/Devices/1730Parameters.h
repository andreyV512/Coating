#pragma once
#include "Base/TablesDefine.h"

DEFINE_PARAM(iCU, unsigned, 1 << 0) //цепи управления
DEFINE_PARAM(iIn, unsigned, 1 << 1)
DEFINE_PARAM(iOut, unsigned, 1 << 2)
DEFINE_PARAM(iStrobe, unsigned, 1 << 3)	

//DEFINE_PARAM(iAdditionalBit0, unsigned, 1 << 1)
//DEFINE_PARAM(iAdditionalBit1, unsigned, 1 << 2)
//DEFINE_PARAM(iAdditionalBit2, unsigned, 1 << 8)

struct InputBitsTable
{
	typedef Vlst<
		iCU
		, iIn
		, iOut
		, iStrobe
		//, iAdditionalBit0
		//, iAdditionalBit1
		//, iAdditionalBit2
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"InputBitsTable"; }
};

DEFINE_PARAM(oAutomat, unsigned, 1 << 0)
DEFINE_PARAM(oSupply, unsigned, 1 << 1)
DEFINE_PARAM(oMark, unsigned, 1 << 2)
DEFINE_PARAM(oGenerator, unsigned, 1 << 8)

//DEFINE_PARAM(oAdditionalBit0, unsigned, 1 << 1)
//DEFINE_PARAM(oAdditionalBit1, unsigned, 1 << 2)
//DEFINE_PARAM(oAdditionalBit2, unsigned, 1 << 8)

struct OutputBitsTable
{
	typedef Vlst<
		oAutomat
		, oSupply
		, oMark
		, oGenerator
		//, oAdditionalBit0
		//, oAdditionalBit1
		//, oAdditionalBit2
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"OutputBitsTable"; }
};

STR_PARAM(NamePlate1730, 64, L"PCIE-1730,BID#0")
struct NamePlate1730ParametersTable
{
	typedef Vlst<
		NamePlate1730
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"NamePlate1730ParametersTable"; }
};