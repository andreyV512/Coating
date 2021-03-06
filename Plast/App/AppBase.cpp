#include "AppBase.h"
#include "Base/tables.hpp"
#include "tools_debug/DebugMess.h"

const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wsprintf(&path[wcslen(path)], L"\\Params");
	return path;
}

template<class O, class P>struct row_table
{
	void operator()(P &p)
	{
		O &o = Singleton<O>::Instance();
		Select<O>(p).ID(__id__).Execute(o);
	}
};

void AppBase::Init()
{
	ParametersBase param;
	CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> > createDateBase;
	CBase base(
		(wchar_t *)param.name()
		, createDateBase
		, param.tables
	);
	if (base.IsOpen())
	{
		VL::foreach<ParametersBase::one_row_table_list, row_table>()(base);
		InitTypeSizeTables(base);
	}
}

template<class O, class P>struct __default_param__XXX;
template<class O, class P>struct __default_param__
{
	void operator()(O &o, P &) {}
};
template<class X, class P>struct __default_param__<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O &o, P &base)
	{
		X &x = Singleton<X>::Instance();
		Select<X>(base).ID(o.value).Execute(x);
		VL::foreach<typename X::items_list, __default_param__XXX>()(x.items, base);
	}
};
template<class O, class P>struct __default_param__XXX : __default_param__<O, P> {};
template<class X, class P>struct __default_param__XXX<ID<X>, P> : __default_param__<ID<X>, P> {};

void AppBase::InitTypeSizeTables(CBase &base)
{
	CurrentParametersTable x;
	Select<CurrentParametersTable>(base).ID(__id__).Execute(x);
	ParametersTable &p = Singleton<ParametersTable>::Instance();
	Select<ParametersTable>(base).ID(x.items.get<CurrentID>().value).Execute(p);
	VL::foreach<typename ParametersTable::items_list, __default_param__>()(p.items, base);
}
