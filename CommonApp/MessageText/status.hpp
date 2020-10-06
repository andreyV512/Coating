#pragma once
#include "templates/typelist.hpp"

namespace ZoneStatus
{
	template<int N, class List>struct OffsetAt;
	template<int N, class Head, class ...Tail>struct OffsetAt<N, Vlst<Head, Tail...>>
	{
		static const int num = N >> 1;
		typedef typename VL::_if<0 != (N & 1), Head, Vlst<>>::Result Val;
		typedef typename VL::Append<
			Val
			, typename OffsetAt <num, Vlst<Tail...>>::Result
		>::Result Result;
	};

	template<int N>struct OffsetAt<N, Vlst<>>
	{
		typedef Vlst<> Result;
	};

	template<class A, class B>struct Add;
	template<class B>struct Add<Vlst<>, B>
	{
		typedef Vlst<B> Result;
	};
	template<class A>struct Add<A, Vlst<>>
	{
		typedef A Result;
	};
	template<class ...A, class B>struct Add<Vlst<A...>, B>
	{
		typedef Vlst<A..., B> Result;
	};

	template<class T>struct GetHead;
	template<class Head, class ...Tail>struct GetHead<Vlst<Head, Tail...>>
	{
		typedef Head Result;
	};
	template<class T>struct GetTail;
	template<class Head, class ...Tail>struct GetTail<Vlst<Head, Tail...>>
	{
		typedef Vlst<Tail...> Result;
	};

	template<class T, class List>struct SkipList;
	template<class T, class Head, class ... Tail>struct SkipList<T, Vlst<Head, Tail...>>
	{
		typedef typename VL::_if<
			VL::Equally<T, typename GetHead<Head>::Result>::value
			, typename GetTail<Head>::Result
			, typename SkipList<T, Vlst<Tail...>>::Result
		>::Result Result;
	};
	template<class T>struct SkipList<T, Vlst<>>
	{
		typedef Vlst<> Result;
	};

	template<class T, class skip>struct СrossLists;
	template<class skip, class Head, class ...Tail>struct СrossLists<Vlst<Head, Tail...>, skip>
	{
		static const bool value = VL::InList<skip, Head>::value || СrossLists<Vlst<Tail...>, skip>::value;
	};
	template<class skip>struct СrossLists<Vlst<>, skip>
	{
		static const bool value = false;
	};

	template<class List, class testList = List>struct Skip;

	template<class testList, class Head, class ...Tail>struct Skip<Vlst<Head, Tail...>, testList>
	{
		typedef typename SkipList<Head, __skip_list__>::Result skipList;
		static const bool value = СrossLists<testList, skipList>::value || Skip<Vlst<Tail...>, testList>::value;
	};

	template<class testList>struct Skip<Vlst<>, testList>
	{
		static const bool value = false;
	};

	template<bool b, class List, class tmp = Vlst<>>struct Once;

	template<bool b, class tmp, class Head, class ...Tail>struct Once <b, Vlst<Head, Tail...>, tmp>
	{
		typedef typename Once<
			b
			, Vlst<Tail...>
			, typename VL::_if<b == (1 == VL::Length<Head>::value), tmp, typename Add<tmp, Head>::Result>::Result
		>::Result Result;
	};

	template<bool b, class tmp>struct Once <b, Vlst<>, tmp>
	{
		typedef tmp Result;
	};

	template<int start, int stop, class tmp = Vlst<>>struct xGen
	{
		typedef typename OffsetAt<start, zone_status_list>::Result item_list;
		typedef typename VL::_if<
			!Skip<item_list>::value
			, typename Add<tmp, item_list>::Result
			, tmp
		> ::Result val;
		typedef typename xGen<1 + start, stop, val>::Result Result;
	};
	template<int stop, class tmp>struct xGen<stop, stop, tmp>
	{
		typedef typename VL::Append < typename Once<false, tmp>::Result, typename Once<true, tmp>::Result>::Result Result;
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////
	typedef  xGen<1, 1 << VL::Length<zone_status_list>::value>::Result status_list;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T, class List>struct sub_del
	{
		typedef typename VL::Erase<List, T>::Result erase;
		typedef typename SkipList<T, __skip_list__>::Result skipList;
		typedef typename VL::_if<СrossLists<erase, skipList>::value
			, Vlst<>
			, T
		>::Result Result;
	};

	template<class items_list, class order_by, class tmp = Vlst<>>struct sort;
	template<class items_list, class tmp, class Head, class ...Tail>struct sort<items_list, Vlst<Head, Tail...>, tmp>
	{

		typedef typename sort<items_list, Vlst<Tail...>
			, typename VL::Append<
			tmp
			, typename VL::_if<VL::InList<items_list, Head>::value, Head, Vlst<>>::Result
			>::Result
		>::Result Result;
	};

	template<class items_list, class tmp>struct sort<items_list, Vlst<>, tmp>
	{
		typedef tmp Result;
	};

	template<class ListItems, class List = ListItems, class tmp = Vlst<>> struct del;
	template<class List, class Head, class tmp, class ...Tail>struct del<Vlst<Head, Tail...>, List, tmp>
	{
		typedef typename del<Vlst<Tail...>, List, typename VL::Append<tmp, typename sub_del<Head, List>::Result>::Result>::Result Result;
	};
	template<class List, class tmp> struct del<Vlst<>, List, tmp>
	{
		typedef typename sort<tmp, zone_status_list>::Result Result;
	};

	template<class List0, class List1>struct attach
	{
		typedef typename VL::Append<List0, List1>::Result add;
		typedef typename VL::EraseDouble<add>::Result no_double;
		typedef typename del<no_double>::Result Result;
	};
}