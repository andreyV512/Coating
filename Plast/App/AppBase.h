#pragma once
#include "Base/TablesDefine.h"
#include "Base/tables.hpp"
#include "templates/typelist.hpp"
#include "Dlg/DspFiltrParams.h"
#include "Units/Lan/LanParameters.h"
#include "Units/1730/1730Parameters.h"

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
DEFINE_PARAM(CurrentUserNameID, int, 1)
struct CurrentParametersTable
{
	typedef Vlst<
		CurrentID
		, CurrentUserNameID
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
		, NamePlate1730ParametersTable
		, LanParametersTable
	> one_row_table_list;

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

template<class T>void UpdateId(CBase &base, int num)
{
	CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	Select<CurrentParametersTable>(base).Execute(current);
	ParametersTable &t = Singleton<ParametersTable>::Instance();
	t.items.get<T>().value = num;
	UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>int CountId(CBase &base, int num)
{
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


