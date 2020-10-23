#pragma once
#include "App/AppBase.h"
#include "Base/TablesDefine.h"
#include "App/App.h"

VAR_ARR(ZonesData, double, 2 * App::count_zones)
VAR_ARR(ZonesStatus, char, App::count_zones)

DEFINE_PARAM_ID(UserTable, int, __id__)
DEFINE_PARAM(StoreTime, COleDateTime, 0.0)

struct DataTable
{
	typedef Vlst<
		StoreTime
		, ID<TresholdsTable>
		, ID<DeadZonesTable>
		, ID<UserTable>
		, ZonesData
		, ZonesStatus
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"DataTable"; }
};

struct StoreBase
{
	typedef Vlst<
		TresholdsTable
		, DeadZonesTable
		, UserTable
		, DataTable
	> type_list;

	typedef VL::Factory<type_list> TTables;
	TTables tables;
	const wchar_t *name() { return L"StoreBase"; }
};

namespace Store
{
	void Result(COleDateTime &tme);
	void Init();
}


