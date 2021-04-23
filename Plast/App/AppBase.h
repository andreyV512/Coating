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
//struct SensorOff {};

DEFINE_PARAM_WRAP(Clr, Norm              , unsigned, 0xff00ff00)
DEFINE_PARAM_WRAP(Clr, Defect			 , unsigned, 0xffff0000)
DEFINE_PARAM_WRAP(Clr, NoBottomReflection, unsigned, 0xff4286f4)
DEFINE_PARAM_WRAP(Clr, DeadZone			 , unsigned, 0xff939393)
//DEFINE_PARAM_WRAP(Clr, SensorOff         , unsigned, 0xff939393)
DEFINE_PARAM_WRAP_LIST(Clr, unsigned, 0xffffff00, NoBottomReflection, Defect)

struct ColorTable
{
	typedef Vlst<
		Clr<Norm>											  
		, Clr<Defect			>
		, Clr<NoBottomReflection>
		, Clr<DeadZone			>
		//, Clr<SensorOff			>
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

DEFINE_PARAM(CountStoredFiles, int, 500)
DEFINE_PARAM(StoreFileOn, bool, true)
struct StoreTable
{
	typedef Vlst<
		CountStoredFiles
		, StoreFileOn
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"StoreTable"; }
};

DEFINE_PARAM(ComPortAddr, int, 1)
DEFINE_PARAM(BaudRate, int, 19200)
DEFINE_PARAM(Parity, int, NOPARITY)
DEFINE_PARAM(StopBits, int, ONESTOPBIT)
DEFINE_PARAM(Abonent, int, 0)
DEFINE_PARAM(InverterFrequency, int, 120)
struct ComPortTable
{
	typedef Vlst<
		ComPortAddr
		, BaudRate
		, Parity
		, StopBits
		, Abonent
		, InverterFrequency
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"ComPortTable"; }
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

struct MedianFiltreWidth{};
struct MedianFiltreON	{};

DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreWidth, 0, int, 5)
DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreON, 0, bool, true)
DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreWidth, 1, int, 5)
DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreON, 1, bool, true)
DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreWidth, 2, int, 5)
DEFINE_PARAM_Wrap_NUM(Num, MedianFiltreON   , 2, bool, true)

struct MedianFiltreTable
{
	typedef Vlst<
		  Num<MedianFiltreWidth, 0>
		, Num<MedianFiltreON, 0>
		, Num<MedianFiltreWidth, 1>
		, Num<MedianFiltreON, 1>
		, Num<MedianFiltreWidth, 2>
		, Num<MedianFiltreON   , 2>
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"MedianFiltreTable"; }
};

struct AlarmThresh				  {};
struct AlarmThreshStart           {};  
struct AlarmThreshStop            {};  
struct AlarmGainStart             {};  
struct AlarmGainStop              {};  
struct BottomReflectionThresh     {};  
struct BottomReflectionThreshStart{};
struct BottomReflectionThreshStop {};  
struct BottomReflectionGainStart  {};  
struct BottomReflectionGainStop   {};  
struct BottomReflectionOn         {};

DEFINE_PARAM_Wrap_NUM(Num, AlarmThresh, 0, double, 50)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStart, 0, double, 10)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStop, 0, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStart, 0, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStop, 0, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThresh, 0, double, 15)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStart, 0, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStop, 0, double, 100)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStart, 0, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStop, 0, double, 1.0) 
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionOn, 0, bool, true)

DEFINE_PARAM_Wrap_NUM(Num, AlarmThresh, 1, double, 50)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStart, 1, double, 10)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStop, 1, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStart, 1, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStop, 1, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThresh, 1, double, 15)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStart, 1, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStop, 1, double, 100)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStart, 1, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStop, 1, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionOn, 1, bool, true)

DEFINE_PARAM_Wrap_NUM(Num, AlarmThresh, 2, double, 50)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStart, 2, double, 10)
DEFINE_PARAM_Wrap_NUM(Num, AlarmThreshStop, 2, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStart, 2, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, AlarmGainStop, 2, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThresh, 2, double, 15)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStart, 2, double, 80)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionThreshStop, 2, double, 100)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStart, 2, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionGainStop, 2, double, 1.0)
DEFINE_PARAM_Wrap_NUM(Num, BottomReflectionOn, 2, bool, true)

DEFINE_PARAM(SoundSpeed, double, 3130)

struct TresholdsTable
{
	typedef Vlst<
		  Num<AlarmThresh, 0>
		, Num<AlarmThreshStart, 0>
		, Num<AlarmThreshStop, 0>
		, Num<AlarmGainStart, 0>
		, Num<AlarmGainStop, 0>
		, Num<BottomReflectionThresh, 0>
		, Num<BottomReflectionThreshStart, 0>
		, Num<BottomReflectionThreshStop, 0>
		, Num<BottomReflectionGainStart, 0>
		, Num<BottomReflectionGainStop, 0>
		, Num<BottomReflectionOn, 0>
		, Num<AlarmThresh, 1>
		, Num<AlarmThreshStart, 1>
		, Num<AlarmThreshStop, 1>
		, Num<AlarmGainStart, 1>
		, Num<AlarmGainStop, 1>
		, Num<BottomReflectionThresh, 1>
		, Num<BottomReflectionThreshStart, 1>
		, Num<BottomReflectionThreshStop, 1>
		, Num<BottomReflectionGainStart, 1>
		, Num<BottomReflectionGainStop, 1>
		, Num<BottomReflectionOn, 1>
		, Num<AlarmThresh				 , 2>
		, Num<AlarmThreshStart			 , 2>
		, Num<AlarmThreshStop			 , 2>
		, Num<AlarmGainStart			 , 2>
		, Num<AlarmGainStop				 , 2>
		, Num<BottomReflectionThresh	 , 2>
		, Num<BottomReflectionThreshStart, 2>
		, Num<BottomReflectionThreshStop , 2>
		, Num<BottomReflectionGainStart	 , 2>
		, Num<BottomReflectionGainStop	 , 2>
		, Num<BottomReflectionOn		 , 2>
		, SoundSpeed
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"TresholdsTable"; }
};

DEFINE_PARAM(DeadZoneStart, int, 200)
DEFINE_PARAM(DeadZoneStop , int, 200)

struct DeadZonesTable
{
	typedef Vlst<
		DeadZoneStart
		, DeadZoneStop
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"DeadZonesTable"; }
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

#define PARAM_ID FiltersTable, TresholdsTable, MedianFiltreTable, DeadZonesTable
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
		, StoreTable
		, ComPortTable
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


