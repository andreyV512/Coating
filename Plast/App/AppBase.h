#pragma once
#include "Base/TablesDefine.h"
#include "Base/tables.hpp"
#include "templates/typelist.hpp"
#include "DspFilters/DspFiltrParams.h"
#include "Devices/LanParameters.h"
#include "Devices/1730Parameters.h"

template<class ...T>struct Clr {};

struct Norm {};
struct NoBottomReflection {};
struct Defect {};
struct DeadZone {};
struct SensorOff {};

DEFINE_PARAM_WRAP(Clr, Norm              , unsigned, 0xff00ff00)
DEFINE_PARAM_WRAP(Clr, Defect			 , unsigned, 0xffff0000)
DEFINE_PARAM_WRAP(Clr, NoBottomReflection, unsigned, 0xff4286f4)
DEFINE_PARAM_WRAP(Clr, DeadZone			 , unsigned, 0xff939393)
DEFINE_PARAM_WRAP(Clr, SensorOff         , unsigned, 0xff939393)
DEFINE_PARAM_WRAP_LIST(Clr, unsigned, 0xffffff00, NoBottomReflection, Defect)

struct ColorTable
{
	typedef Vlst<
		Clr<Norm>											  
		, Clr<Defect			>
		, Clr<NoBottomReflection>
		, Clr<DeadZone			>
		, Clr<SensorOff			>
		, Clr<NoBottomReflection, Defect>		
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

DEFINE_PARAM(OffsetSensorBegMM, int, 20)
DEFINE_PARAM(OffsetSensorEndMM, int, 20)
struct UnitTable
{
	typedef Vlst<
		OffsetSensorBegMM
		, OffsetSensorEndMM
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"UnitTable"; }
};

DEFINE_PARAM(MedianFiltreWidth, int, 5)
DEFINE_PARAM(MedianFiltreON, bool, true)
struct MedianFiltreTable
{
	typedef Vlst<
		MedianFiltreWidth
		, MedianFiltreON
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"MedianFiltreTable"; }
};

DEFINE_PARAM(AlarmThresh              , double, 50)
DEFINE_PARAM(AlarmThreshStart         , int, 10)
DEFINE_PARAM(AlarmThreshStop          , int, 80)
DEFINE_PARAM(AlarmGainStart           , double, 1.0)
DEFINE_PARAM(AlarmGainStop            , double, 1.0)
DEFINE_PARAM(BottomReflectionThresh         , double, 15)
DEFINE_PARAM(BottomReflectionThreshStart    , int, 80)
DEFINE_PARAM(BottomReflectionThreshStop     , int, 100)
DEFINE_PARAM(BottomReflectionGainStart, double, 1.0)
DEFINE_PARAM(BottomReflectionGainStop , double, 1.0)
DEFINE_PARAM(BottomReflectionOn, bool, true)

struct TresholdsTable
{
	typedef Vlst<
		AlarmThresh
		, AlarmThreshStart
		, AlarmThreshStop
		, AlarmGainStart
		, AlarmGainStop
		, BottomReflectionThresh
		, BottomReflectionThreshStart
		, BottomReflectionThreshStop
		, BottomReflectionGainStart
		, BottomReflectionGainStop
		, BottomReflectionOn
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"TresholdsTable"; }
};

static const int __id__ = 2;
																 
DEFINE_PARAM(CurrentID, int, __id__)
DEFINE_PARAM(CurrentUserNameID, int, __id__)
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

#define PARAM_ID FiltersTable, TresholdsTable, MedianFiltreTable
#define PARAM_IDM(n)DEFINE_PARAM_ID(n, int, __id__)
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
		, UnitTable
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
	Select<CurrentParametersTable>(base).ID(__id__).Execute(current);
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


