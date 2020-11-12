#include "StoreBase.h"
#include "Base/ExpressBase.hpp"
#include "Base/tables.hpp"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"

template<class T>int GetID(CBase &base)
{
	auto &t = Singleton<T>::Instance();
	int id = Select<T>(base).Execute(t);
	if (0 == id)
	{
		Insert_Into<T>(t, base).Execute();
		id = Select<T>(base).Execute(t);
	}
	return id;
}

void Store::Result(COleDateTime &tme)
{
	StoreBase param;
	CreateDataBase<StoreBase::type_list, Vlst<>, MSsql> createDateBase;
	CExpressBase base(
		(wchar_t *)param.name()
		, createDateBase
		, param.tables
	);
	if (base.IsOpen())
	{
		auto &dt = Singleton<DataTable>::Instance();
		auto &rd = Singleton<Data::ResultData>::Instance();

		double *d = dt.items.get<ZonesData>().value.buffer;
		int &count = dt.items.get<ZonesData>().value.count;

		memmove(d, rd.minData, rd.count * sizeof(double));
		d += rd.count;
		memmove(d, rd.maxData, rd.count * sizeof(double));
		count = 2 * rd.count;

		char *st = dt.items.get<ZonesStatus>().value.buffer;
		memmove(st, rd.status, rd.count * sizeof(char));
		dt.items.get<ZonesStatus>().value.count = rd.count;

		dt.items.get<ID<TresholdsTable>>().value = GetID<TresholdsTable>(base);
		dt.items.get<ID<DeadZonesTable>>().value = GetID<DeadZonesTable>(base);
		dt.items.get<ID<UserTable>>().value = GetID<UserTable>(base);

		Insert_Into<DataTable>(dt, base).Execute();
	}
}

struct CreateStored
{
	template<class Tables, class Base>void operator()(Tables &t, Base &b)
	{
		char query[] =
#include "Data\querys\StoreBase.sql"
			;
		b.conn->Execute(query, NULL, ADODB::adExecuteNoRecords);
	}
};

void Store::Init()
{
	StoreBase param;
	CreateDataBase<StoreBase::type_list, CreateStored, MSsql> createDateBase;
	CExpressBase base(
		(wchar_t *)param.name()
		, createDateBase
		, param.tables
	);
	if (base.IsOpen())
	{
		dprint("Store base ok");
	}
}
