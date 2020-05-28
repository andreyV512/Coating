// test_type_list.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "templates/typelist.hpp"

template<class T>struct  Low {};
template<class T>struct  High {};
template<class T>struct  BandPass;
template<class T>struct  PandStop;
template<class T>struct  ChebI;
struct Order;
struct CutoffFrequency;
struct Ripple;
struct OnFiltre;

struct CurrentFilter {};

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
	> items_list;
	typedef VL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name() { return L"FiltersTable"; }
};

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

int main()
{
	typedef TstTempl<Vlst<High< ChebI< Order>>>, Low< ChebI< CutoffFrequency>>>::Result lst;
    std::cout << typeid(lst).name() << "\n";

	typedef TstTempl<FiltersTable::items_list, CurrentFilter>::Result lst1;
	std::cout << typeid(lst1).name() << "\n";

	typedef type_filters<FiltersTable::items_list>::Result lst2;
	std::cout << typeid(lst2).name() << "\n";
}
