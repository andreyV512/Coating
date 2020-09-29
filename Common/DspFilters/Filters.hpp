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

class IDSPFlt
{
public:
	virtual double operator()(double value) { return value; };
	virtual void Clean() {};
};

template<template<class>class TypeFiltre, template<class>class SubTypeFiltre, int MaxOrder = 5>class DSPFlt: public IDSPFlt
{
public:
	typedef typename DSPFltType<TypeFiltre,SubTypeFiltre, MaxOrder>::Result T;
public:
	T filtre;
	typename  T::template State<Dsp::DirectFormI> state;
public:
	double operator()(double value)override 
	{
		return state.process(value, filtre);
	}
	void Clean()override
	{

	}
};

template<class T>struct SetupFiltre;
template<template<class>class type, template<class>class sub, int order>struct SetupFiltre<DSPFlt<type, sub, order>>
{
	typedef typename DSPFlt<type, sub, order> T;
	template<class P>void operator()(T &t, P &p, int sample_rate)
	{
		t.filtre.setup(
			p.get<type<sub<Order>>>().value
			, sample_rate
			, p.get<type<sub<CutoffFrequency>>>().value
			, p.get<type<sub<Ripple>>>().value
		);
	}
};

template<template<class>class type, template<class>class sub, int order>struct __SetupFiltre__
{
	typedef typename DSPFlt<type, sub, order> T;
	template<class P>void operator()(T &t, P &p, int sample_rate)
	{
		t.filtre.setup(
			p.get<type<sub<Order>>>().value
			, sample_rate
			, p.get<type<sub<CenterFrequency>>>().value
			, p.get<type<sub<WidthFrequency>>>().value
			, p.get<type<sub<Ripple>>>().value
		);
	}
};

template<template<class>class sub, int order>struct SetupFiltre<DSPFlt<BandPass, sub, order>>: __SetupFiltre__<BandPass, sub, order> {};
template<template<class>class sub, int order>struct SetupFiltre<DSPFlt<BandStop, sub, order>>: __SetupFiltre__<BandStop, sub, order> {};

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

typedef type_filters<FiltersTable::items_list>::Result type_flites_list;

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

//template<class Params>struct __init_filtre_data__
//{
//	IDSPFlt *&proc;
//	Params &params;
//	__init_filtre_data__(IDSPFlt *&proc, Params &params) : proc(proc), params(params) {}
//};
//
//template<class O, class P>struct __init_filtre__
//{
//	bool operator()(O &o, P &p)
//	{
//		if (VL::IndexOf<filters_list, O>::value == p.params.get<CurrentFilter>().value)
//		{
//			SetupFiltre<O>()(o, p.params, Singleton<LanParametersTable>::Instance().items.get<Frequency>().value);
//			p.proc = &o;
//			return false;
//		}
//		return true;
//	}
//};





