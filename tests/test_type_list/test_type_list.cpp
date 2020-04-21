// test_type_list.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "templates/typelist.hpp"

template<class List>struct __get_height_tmpl__;
template<class Head, class ...Tail>struct __get_height_tmpl__<Vlst<Head, Tail...> >
{
	static const int value = Head::DY + __get_height_tmpl__<Vlst<Tail...> >::value;
};
template<>struct __get_height_tmpl__<Vlst<>>
{
	static const int value = 0;
};
template<class T, class Owner>struct DlgItem2 { static const int DY = 10; };

template<class T>struct GroupBox
{
	typedef T Result;
	static const int DY = __get_height_tmpl__<VL::TypeToTypeLstParam1<Result, DlgItem2, NullType>::Result>::value;
};

#define GROUP_BOX(...) GroupBox<Vlst<__VA_ARGS__>>

template<class List>struct __del_group_box__;
template<class Head, class ...Tail>struct __del_group_box__<Vlst<Head, Tail...>>
{
	typedef typename VL::Append<Head, typename __del_group_box__<Vlst<Tail...>>::Result>::Result Result;
};
template<class List, class ...Tail>struct __del_group_box__<Vlst<GroupBox<List>, Tail...> >
{
	typedef typename VL::Append<List, typename __del_group_box__<Vlst<Tail...>>::Result>::Result Result;
};
template<>struct __del_group_box__<Vlst<> >
{
	typedef Vlst<> Result;
};

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

int main()
{
	typedef GROUP_BOX(B, C, D) g;

	typedef __del_group_box__<g::Result>::Result t;

	printf("%s\n", typeid(__del_group_box__ < Vlst<A, g, E, F>>::Result).name());

    std::cout << "Hello World!\n";
}
