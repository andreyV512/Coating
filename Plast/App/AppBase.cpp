#include "AppBase.h"
#include "Base/tables.hpp"

const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path);
	path[len - 4] = 0;
	return path;
}

//template<>struct set_default_table<UserTable, CBase>
//{
//	void operator()(UserTable &table, CBase &base)
//	{
//		Insert_Into<UserTable>(table, base).Execute();
//		wchar_t *query = (wchar_t *)L"ALTER TABLE UserTable ADD UNIQUE (UserName)";	   .......
//		const auto _ = base.conn->Execute(query, NULL, ADODB::adCmdText);
//	}
//};

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
		VL::for_each<typename X::items_list, __default_param__XXX>()(x.items, base);
	}
};
template<class O, class P>struct __default_param__XXX : __default_param__<O, P> {};
template<class X, class P>struct __default_param__XXX<ID<X>, P> : __default_param__<ID<X>, P> {};

void AppBase::InitTypeSizeTables(CBase &base)
{
	CurrentParametersTable x;
	Select<CurrentParametersTable>(base).ID(1).Execute();
	ParametersTable &p = Singleton<ParametersTable>::Instance();
	Select<ParametersTable>(base).ID(x.items.get<CurrentID>().value).Execute(p);
	VL::for_each<typename ParametersTable::items_list, __default_param__>()(p.items, base);
}
