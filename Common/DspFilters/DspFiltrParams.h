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
template<class T, int N>struct Num : T { typedef T OBJ; static const int NUM = N; };
template<int N, class T>Num<T, N> &xz_type(T &t) { return (Num<T, N> &)t; }

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
		  Num<CurrentFilter						, 0>
		, Num<Low< ChebI< Order>>				, 0>
		, Num<Low< ChebI< CutoffFrequency>>		, 0>
		, Num<Low< ChebI< Ripple>>				, 0>
		, Num<High< ChebI< Order>>				, 0>
		, Num<High< ChebI< CutoffFrequency>>	, 0>
		, Num<High< ChebI< Ripple>>				, 0>
		, Num<BandPass< ChebI< Order>>			, 0>
		, Num<BandPass< ChebI< CenterFrequency>>, 0>
		, Num<BandPass< ChebI< WidthFrequency>>	, 0>
		, Num<BandPass< ChebI< Ripple>>			, 0>
		, Num<BandStop< ChebI< Order>>			, 0>
		, Num<BandStop< ChebI< CenterFrequency>>, 0>
		, Num<BandStop< ChebI< WidthFrequency>>	, 0>
		, Num<BandStop< ChebI< Ripple>>			, 0>

		, Num<CurrentFilter, 1>
		, Num<Low< ChebI< Order>>, 1>
		, Num<Low< ChebI< CutoffFrequency>>, 1>
		, Num<Low< ChebI< Ripple>>, 1>
		, Num<High< ChebI< Order>>, 1>
		, Num<High< ChebI< CutoffFrequency>>, 1>
		, Num<High< ChebI< Ripple>>, 1>
		, Num<BandPass< ChebI< Order>>, 1>
		, Num<BandPass< ChebI< CenterFrequency>>, 1>
		, Num<BandPass< ChebI< WidthFrequency>>, 1>
		, Num<BandPass< ChebI< Ripple>>, 1>
		, Num<BandStop< ChebI< Order>>, 1>
		, Num<BandStop< ChebI< CenterFrequency>>, 1>
		, Num<BandStop< ChebI< WidthFrequency>>, 1>
		, Num<BandStop< ChebI< Ripple>>, 1>

		, Num<CurrentFilter, 2>
		, Num<Low< ChebI< Order>>, 2>
		, Num<Low< ChebI< CutoffFrequency>>, 2>
		, Num<Low< ChebI< Ripple>>, 2>
		, Num<High< ChebI< Order>>, 2>
		, Num<High< ChebI< CutoffFrequency>>, 2>
		, Num<High< ChebI< Ripple>>, 2>
		, Num<BandPass< ChebI< Order>>, 2>
		, Num<BandPass< ChebI< CenterFrequency>>, 2>
		, Num<BandPass< ChebI< WidthFrequency>>, 2>
		, Num<BandPass< ChebI< Ripple>>, 2>
		, Num<BandStop< ChebI< Order>>, 2>
		, Num<BandStop< ChebI< CenterFrequency>>, 2>
		, Num<BandStop< ChebI< WidthFrequency>>, 2>
		, Num<BandStop< ChebI< Ripple>>, 2>
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"FiltersTable"; }
};


