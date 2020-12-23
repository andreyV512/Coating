#pragma once
#include "App/AppBase.h"

template<class List>struct ALLParamsList;
template<class Head, class ...Tail>struct ALLParamsList<Vlst<Head, Tail...>>
{
	typedef typename VL::Append<VL::Factory<typename Head::items_list>, typename ALLParamsList<Vlst<Tail...>>::Result>::Result Result;
};
template<>struct ALLParamsList<Vlst<>>
{
	typedef Vlst<> Result;
};

struct ALLPatrams
{
	template<class O, class P>struct loc
	{
		void operator()(P &p)
		{
			VL::CopyFromTo(Singleton<O>::Instance().items, p.items.get<VL::Factory<O::items_list>>());
		}
	};
public:
	typedef Vlst<LanParametersTable, PARAM_ID> list;
	typedef ALLParamsList<list>::Result items_list;
	VL::Factory<items_list> items;

	void SetParams()
	{
		VL::foreach<list, loc>()(*this);
	}

	template<class T>void GetParam(T &t)
	{
		VL::CopyFromTo(items.get<T>(), t);
	}
	template<class T>void SetParam(T &t)
	{
		VL::CopyFromTo(t, items.get<T>());
	}
	template<class T>typename T::TItems &Items()
	{
		return items.get<typename T::TItems>();
	}
};
