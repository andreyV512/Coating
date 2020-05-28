#pragma once
#include "templates/typelist.hpp"
#include "Base/TablesDefine.h"
/*
order,
sampleRate,
centerFrequency,
widthFrequency
*/
template<class T>struct Low;
template<class T>struct High;
template<class T>struct BandPass;
template<class T>struct BandStop;
template<class T>struct ChebI;

struct Order;
struct CutoffFrequency;
struct Ripple;
struct CenterFrequency;
struct WidthFrequency;

DEFINE_PARAM(CurrentFilter, int, 0)

DEFINE_PARAM_WRAP2(Low, ChebI, Order, int, 3)
DEFINE_PARAM_WRAP2(Low, ChebI, CutoffFrequency, int, 50)
DEFINE_PARAM_WRAP2(Low, ChebI, Ripple, double, 0.1)

DEFINE_PARAM_WRAP2(High, ChebI, Order, int, 3)
DEFINE_PARAM_WRAP2(High, ChebI, CutoffFrequency, int, 50)
DEFINE_PARAM_WRAP2(High, ChebI, Ripple, double, 0.1)

DEFINE_PARAM_WRAP2(BandPass, ChebI, Order, int, 3)
DEFINE_PARAM_WRAP2(BandPass, ChebI, CenterFrequency, int, 50)
DEFINE_PARAM_WRAP2(BandPass, ChebI, WidthFrequency, double, 10)
DEFINE_PARAM_WRAP2(BandPass, ChebI, Ripple, double, 0.1)

DEFINE_PARAM_WRAP2(BandStop, ChebI, Order, int, 3)
DEFINE_PARAM_WRAP2(BandStop, ChebI, CenterFrequency, int, 50)
DEFINE_PARAM_WRAP2(BandStop, ChebI, WidthFrequency, double, 10)
DEFINE_PARAM_WRAP2(BandStop, ChebI, Ripple, double, 0.1)

struct FiltersTable
{
	typedef Vlst<
		CurrentFilter

		, Low< ChebI< Order>>
		, Low< ChebI< CutoffFrequency>>
		, Low< ChebI< Ripple>>

		, High< ChebI< Order>>
		, High< ChebI< CutoffFrequency>>
		, High< ChebI< Ripple>>

		, BandPass< ChebI< Order>>
		, BandPass< ChebI< CenterFrequency>>
		, BandPass< ChebI< WidthFrequency>>
		, BandPass< ChebI< Ripple>>

		, BandStop< ChebI< Order>>
		, BandStop< ChebI< CenterFrequency>>
		, BandStop< ChebI< WidthFrequency>>
		, BandStop< ChebI< Ripple>>
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"FiltersTable"; }
};


