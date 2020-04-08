#pragma once
#include "Base/TablesDefine.h"
#include "Base/tables.hpp"
#include "templates/typelist.hpp"
#include "Dlg/DspFiltrParams.h"

template<class>struct Clr;

struct Nominal;

DEFINE_PARAM_WRAP(Clr, Nominal, int, 0xff00ff00)

struct ColorTable
{
	typedef Vlst<
		Clr<Nominal>
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"ColorTable"; }
};

DEFINE_PARAM(iCU, unsigned, 1 << 0) //цепи управления
DEFINE_PARAM(iKM2_DC, unsigned, 1 << 4)
DEFINE_PARAM(iKM3_AC, unsigned, 1 << 5)
DEFINE_PARAM(iCycle, unsigned, 1 << 8)
DEFINE_PARAM(iP1, unsigned, 1 << 9)
DEFINE_PARAM(iP2, unsigned, 1 << 10)
DEFINE_PARAM(iCOPT, unsigned, 1 << 11)
DEFINE_PARAM(iControl, unsigned, 1 << 12)

struct InputBitsTable
{
	typedef Vlst<
		iCU
		, iKM2_DC
		, iKM3_AC
		, iCycle
		, iP1
		, iP2
		, iCOPT
		, iControl
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"InputBitsTable"; }
};

DEFINE_PARAM(oDC_ON1, unsigned, 1 << 0)
DEFINE_PARAM(oAC_ON, unsigned, 1 << 1)
DEFINE_PARAM(oDC_ON2, unsigned, 1 << 2)
DEFINE_PARAM(oWork, unsigned, 1 << 3)
DEFINE_PARAM(oStart, unsigned, 1 << 4)
DEFINE_PARAM(oToShift, unsigned, 1 << 5)
DEFINE_PARAM(oC1, unsigned, 1 << 6)
DEFINE_PARAM(oC2, unsigned, 1 << 7)

struct OutputBitsTable
{
	typedef Vlst <
		oDC_ON1
		, oAC_ON
		, oDC_ON2
		, oWork
		, oStart
		, oToShift
		, oC1
		, oC2
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"OutputBitTable"; }
};

STR_UNIQUE_PARAM(UserName, 64, L"Nobady")
DEFINE_UNIQUE_PARAM(UserPersonnelNumber, int, 1234)

struct UserTable
{
	typedef Vlst<
		UserName
		, UserPersonnelNumber
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"UserTable"; }
};

DEFINE_PARAM(BelowNominal, double, 3.3)
struct TresholdsTable
{
	typedef Vlst<
		BelowNominal
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"TresholdsTable"; }
};

DEFINE_PARAM(CurrentID, int, 1)
struct CurrentParametersTable
{
	typedef Vlst<
		CurrentID
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"CurrentParametersTable"; }
};

STR_PARAM(NameParam, 128, L"Noname")

#define PARAM_ID FiltersTable, TresholdsTable
#define PARAM_IDM(n)DEFINE_PARAM_ID(n, int, 1)
	FOR_EACH(PARAM_IDM, PARAM_ID)
#undef PARAM_IDM
#define PARAM_IDX(n)ID<n>,

struct ParametersTable
{
	typedef Vlst<
		FOR_EACH(PARAM_IDX, PARAM_ID)
		NameParam
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"ParametersTable"; }
};
#undef PARAM_IDX

struct ParametersBase
{
	typedef Vlst<
		ColorTable
		, UserTable
		, InputBitsTable
		, OutputBitsTable
	> one_row_table_list;

	//typedef Vlst<
	//	CurrentParametersTable
	//	, TresholdsTable
	//> multy_row_table_list;
	typedef VL::Append<
		Vlst<CurrentParametersTable, ParametersTable >
		, Vlst<PARAM_ID> 
	>::Result multy_row_table_list;

	typedef Vlst<
		one_row_table_list
		, multy_row_table_list
	> multy_type_list;

	typedef VL::SubMultyList<
		multy_type_list
	>::Result type_list;

	typedef VL::Factory<type_list> TTables;	  
	TTables tables;
	wchar_t path[512];
	const wchar_t *name();
};

class CBase;
struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

//unsigned TopID();

template<class T>void UpdateId(CBase &base, int num)
{
	//int ID = 0;
	//wchar_t* query = (wchar_t*)L"SELECT TOP 1 ID FROM CurrentParametersTable";
	//CMD(base).CommandText(query).GetValue((wchar_t*)L"ID", ID);

	CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	Select<CurrentParametersTable>(base)/*.ID(TopID())*/.Execute(current);
	ParametersTable &t = Singleton<ParametersTable>::Instance();
	t.items.get<T>().value = num;
	UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>int CountId(CBase &base, int num)
{
	//ADODB::_RecordsetPtr rec;
	//Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	//int i = 0;
	//while (!rec->EndOfFile)
	//{
	//	++i;
	//	rec->MoveNext();
	//}
	int cnt = 0;
	wchar_t query[128];
	wsprintf(query, L"SELECT COUNT(*) AS cnt FROM ParametersTable WHERE %s = %d", T().name(), num);
	CMD(base).CommandText(query).GetValue((wchar_t*)L"cnt", cnt);
	return cnt;
}

template<class T>int CurrentId()
{
	return Singleton<ParametersTable>::Instance().items.get<T>().value;
}


