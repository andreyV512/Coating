#pragma once
#include "templates/typelist.hpp"
#include "Base/TablesDefine.h"
template<class T>struct  Low;
template<class T>struct  High;
template<class T>struct  BandPass;
template<class T>struct  PandStop;
template<class T>struct ChebyshevI;
struct Order;
struct CutoffFrequency;
struct Ripple;
struct OnFiltre;

DEFINE_PARAM(CurrentFilter, int, 0)

DEFINE_PARAM_WRAP2(Low, ChebyshevI, Order, int, 3)
DEFINE_PARAM_WRAP2(Low, ChebyshevI, CutoffFrequency, int, 50)
DEFINE_PARAM_WRAP2(Low, ChebyshevI, Ripple, double, 0.1)
DEFINE_PARAM_WRAP2(Low, ChebyshevI, OnFiltre, bool, true)

DEFINE_PARAM_WRAP2(High, ChebyshevI, Order, int, 3)
DEFINE_PARAM_WRAP2(High, ChebyshevI, CutoffFrequency, int, 50)
DEFINE_PARAM_WRAP2(High, ChebyshevI, Ripple, double, 0.1)
DEFINE_PARAM_WRAP2(High, ChebyshevI, OnFiltre, bool, true)

struct FiltersTable
{
	typedef Vlst<
		CurrentFilter

		, Low< ChebyshevI< Order>>
		, Low< ChebyshevI< CutoffFrequency>>
		, Low< ChebyshevI< Ripple>>

		, High< ChebyshevI< Order>>
		, High< ChebyshevI< CutoffFrequency>>
		, High< ChebyshevI< Ripple>>
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t* name() { return L"FiltersTable"; }
};

