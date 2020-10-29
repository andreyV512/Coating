#pragma once

template<class T>void StoreBaseX(CBase &base, typename T::TItems &from)
{
	auto &to = Singleton<T>::Instance();
	VL::CopyFromTo(from, to.items);
	int id = CurrentId<ID<T> >();
	if (1 == CountId<ID<T> >(base, id))
	{
		UpdateWhere<T>(to, base).ID(id).Execute();
	}
	else
	{
		Insert_Into<T>(to, base).Execute();
	}
}

template<class O, class P>struct __compare_tresh__
{
	bool operator()(O &o, P &p)
	{
		return o.value == p.get<O>().value;
	}
};

template<class T>bool TestX(typename T::TItems &x)
{
	return !VL::find<T::items_list, __compare_tresh__>()(Singleton<T>::Instance().items, x);
}
