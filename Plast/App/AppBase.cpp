#include "AppBase.h"
#include "Base/tables.hpp"
#include "tools_debug/DebugMess.h"

const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path);
	path[len - 4] = 0;
	return path;
}

//template<>struct set_default_table<CurrentParametersTable, CBase>
//{
//	void operator()(UserTable &table, CBase &base)
//	{
//		//Insert_Into<UserTable>(table, base).Execute();
//		//wchar_t *query = (wchar_t *)L"ALTER TABLE UserTable ADD UNIQUE (UserName)";	   .......
//		//const auto _ = base.conn->Execute(query, NULL, ADODB::adCmdText);
//		wchar_t *query = (wchar_t *)L"SELECT TOP 1 ID FROM CurrentParametersTable";
//		int ID = 0;
//		CMD(base).CommandText(query).GetValue((wchar_t *)L"ID", ID);
//		Update< CurrentParametersTable>(base).set<CurrentID>(ID).Where().ID(ID).Execute();
//		dprint("id %d\n", ID);
//	}
//};

template<typename Base>struct set_default_table<CurrentParametersTable, Base>
{
	void operator()(CurrentParametersTable& table, Base& base)
	{
		Insert_Into<CurrentParametersTable>(table, base).Execute();

		int ID = 0;
		wchar_t* query = (wchar_t*)L"SELECT TOP 1 ID FROM CurrentParametersTable";
		CMD(base).CommandText(query).GetValue((wchar_t *)L"ID", ID);
		Update< CurrentParametersTable>(base).set<CurrentID>(ID).Where().ID(ID).Execute();
	}
};

template<class T>struct __set__XXX_data__
{
	T& query;
	int id;
	__set__XXX_data__(T& query, int id) : query(query), id(id) {}
};
template<class O, class P>struct __set__XXX__
{
	void operator()(P& p) {}
};
template<class X, class P>struct __set__XXX__<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(P& p) 
	{
		p.query.set<O>(p.id);
	}
};

template<typename Base>struct set_default_table<ParametersTable, Base>
{
	void operator()(ParametersTable& table, Base& base)
	{
		Insert_Into<ParametersTable>(table, base).Execute();

		int ID = 0;
		wchar_t* query = (wchar_t*)L"SELECT TOP 1 ID FROM ParametersTable";
		CMD(base).CommandText(query).GetValue((wchar_t*)L"ID", ID);
		Update< ParametersTable> update(base);// .set<CurrentID>(ID).Where().ID(ID).Execute();
		__set__XXX_data__ data = { update, ID };
		VL::foreach<ParametersTable::items_list, __set__XXX__>()(data);
		update.Where().ID(ID).Execute();
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

//static unsigned ___top_ID___ = 0;

void AppBase::InitTypeSizeTables(CBase &base)
{
	//wchar_t* query = (wchar_t*)L"SELECT TOP 1 ID FROM CurrentParametersTable";
	//CMD(base).CommandText(query).GetValue((wchar_t*)L"ID", ___top_ID___);

	CurrentParametersTable &x = Singleton<CurrentParametersTable>::Instance();
	Select<CurrentParametersTable>(base)./*ID(___top_ID___).*/Execute(x);
	ParametersTable &p = Singleton<ParametersTable>::Instance();
	Select<ParametersTable>(base).ID(x.items.get<CurrentID>().value).Execute(p);
	VL::foreach<typename ParametersTable::items_list, __default_param__>()(p.items, base);
}

//unsigned TopID()
//{
//	return ___top_ID___;
//}

