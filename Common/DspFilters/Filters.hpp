#pragma once
#include "include/DspFilters/ChebyshevI.h"
#include "include/DspFilters/SmoothedFilter.h"
#include "templates/typelist.hpp"
#include "DspFiltrParams.h"
#include "App/AppBase.h"

template<template<class>class TypeFiltre, template<class>class SubTypeFiltre, int MaxOrder>struct DSPFltType;
template<int MaxOrder>struct DSPFltType<Low, ChebI, MaxOrder>
{
	typedef Dsp::ChebyshevI::LowPass<MaxOrder> Result;
};
template<int MaxOrder>struct DSPFltType<High, ChebI, MaxOrder>
{
	typedef Dsp::ChebyshevI::HighPass<MaxOrder> Result;
};
template<int MaxOrder>struct DSPFltType<BandPass, ChebI, MaxOrder>
{
	typedef Dsp::ChebyshevI::BandPass<MaxOrder> Result;
};
template<int MaxOrder>struct DSPFltType<BandStop, ChebI, MaxOrder>
{
	typedef Dsp::ChebyshevI::BandStop<MaxOrder> Result;
};

template<class T>class IDSPFlt
{
public:
	double operator()(double value) { return ((T *)this)->state.process(value, ((T *)this)->filtre); }
	void Clean() { ((T *)this)->state.Clean(((T *)this)->filtre); }
};

class DSPFltDump: public IDSPFlt<DSPFltDump>
{
public:
	struct Filtre{} filtre;
	struct State 
	{
		double process(double value, Filtre) { return value; }
		void Clean(Filtre) {}
	} state;
};

template<template<class>class TypeFiltre, template<class>class SubTypeFiltre, int MaxOrder = 5>class DSPFlt
	: public IDSPFlt<DSPFlt<TypeFiltre, SubTypeFiltre, MaxOrder>>
{
public:
	typedef typename DSPFltType<TypeFiltre,SubTypeFiltre, MaxOrder>::Result T;
	T filtre;
	typename  T::template State<Dsp::DirectFormI> state;
};

template<class T, int N>struct SetupFiltre;
template<template<class>class type, template<class>class sub, int order, int N>struct SetupFiltre<DSPFlt<type, sub, order>, N>
{
	typedef typename DSPFlt<type, sub, order> T;
	template<class P>void operator()(T &t, P &p, int sample_rate)
	{
		t.filtre.setup(
			p.get<Num<type<sub<Order>>, N>>().value
			, sample_rate
			, p.get<Num<type<sub<CutoffFrequency>>, N>>().value
			, p.get<Num<type<sub<Ripple>>, N>>().value
		);
	}
};

template<template<class>class type, template<class>class sub, int order, int N>struct __SetupFiltre__
{
	typedef typename DSPFlt<type, sub, order> T;
	template<class P>void operator()(T &t, P &p, int sample_rate)
	{
		t.filtre.setup(
			p.get< Num<type<sub<Order>>, N>>().value
			, sample_rate
			, p.get<Num<type<sub<CenterFrequency>>, N>>().value
			, p.get<Num<type<sub<WidthFrequency>>, N>>().value
			, p.get<Num<type<sub<Ripple>>, N>>().value
		);
	}
};

template<template<class>class sub, int order, int N>struct SetupFiltre<DSPFlt<BandPass, sub, order>, N>: __SetupFiltre__<BandPass, sub, order, N> {};
template<template<class>class sub, int order, int N>struct SetupFiltre<DSPFlt<BandStop, sub, order>, N>: __SetupFiltre__<BandStop, sub, order, N> {};

template<class List, class T>struct TstTempl;
template<template<class>class type, template<class>class sub, class X, class Head, class ...Tail>struct TstTempl<Vlst<Head, Tail...>, type<sub<X>>>
{
	typedef typename TstTempl<Vlst<Tail...>, type<sub<X>>>::Result Result;
};
template<template<class>class type, template<class>class sub, class X, class Head, class ...Tail>struct TstTempl<Vlst<type<sub<Head>>, Tail...>, type<sub<X>>>
{
	typedef Vlst<> Result;
};
template<template<class>class type, template<class>class sub, class X>struct TstTempl<Vlst<>, type<sub<X>>>
{
	typedef type<sub<X>> Result;
};
template<class X>struct TstTempl<Vlst<>, X>
{
	typedef Vlst<> Result;
};
template<class X, class Head, class ...Tail>struct TstTempl<Vlst<Head, Tail...>, X>
{
	typedef Vlst<> Result;
};

template<class List, class tmp = Vlst<>>struct type_filters;
template<class tmp, class Head, class... Tail>struct type_filters<Vlst<Head, Tail...>, tmp>
{
	typedef typename type_filters<Vlst<Tail...>, typename VL::Append<tmp, typename TstTempl<tmp, Head>::Result>::Result>::Result Result;
};
template<class tmp>struct type_filters<Vlst<>, tmp>
{
	typedef tmp Result;
};

template<class List, class tmp = Vlst<> >struct filters_sens;
template<class tmp, class Head, class ...Tail >struct filters_sens<Vlst<Head, Tail...>, tmp>
{
	typedef typename filters_sens<Vlst<Tail...>, tmp>::Result Result;
};
template<class tmp, class Head, class ...Tail >struct filters_sens<Vlst<Num<Head, 0>, Tail...>, tmp>
{
	typedef typename filters_sens<Vlst<Tail...>
		, typename VL::Append<tmp, Head>::Result
	>::Result Result;
};
template<class tmp>struct filters_sens<Vlst<>, tmp>
{
	typedef tmp Result;
};

typedef filters_sens<FiltersTable::items_list>::Result FiltersTable_items_list;

typedef type_filters<FiltersTable_items_list>::Result type_flites_list;

template<class List>struct Conv;
template<template<class>class Type, template<class>class Sub, class T, class ...Tail>struct Conv<Vlst<Type<Sub<T>>, Tail...>>
{
	typedef typename VL::Append<DSPFlt<Type, Sub>, typename Conv<Vlst<Tail...>>::Result>::Result Result;
};
template<>struct Conv<Vlst<>>
{
	typedef Vlst<> Result;
};

typedef Conv<type_flites_list>::Result filters_list;






