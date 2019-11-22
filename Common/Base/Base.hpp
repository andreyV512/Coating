#pragma once
#pragma warning( disable : 4146 )
#import "c:\program files\common files\system\ado\msado15.dll" rename("EOF", "EndOfFile")
#import "c:\program files\common files\system\ado\msadox.dll" 
namespace
{
#if defined _M_IX86
static wchar_t *const udl = (wchar_t *)L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
#else
	static wchar_t *const udl = (wchar_t *)L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";
#endif
static wchar_t *const mdb = (wchar_t *)L".mdb;Mode=ReadWrite;Persist Security Info=False";
static wchar_t *const mdbRead = (wchar_t *)L".mdb;Mode=Read;Persist Security Info=False";
}
//------------------------------------------------------------------------------------------
class Initialize
{
public:
	Initialize(){auto _ = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);}
	~Initialize(){CoUninitialize();}
};
//------------------------------------------------------------------------------------------
class CBase
{	
public:
	ADODB::_ConnectionPtr  conn;
	CBase(){}
	template<typename ConstructorTables, typename Tables>CBase(const wchar_t *path, ConstructorTables &o, Tables &tables)
	{
		_bstr_t strcnn(udl);
		strcnn += path;
		strcnn += mdb;
		try
		{
			try
			{
				conn.CreateInstance(__uuidof(ADODB::Connection)); 
				conn->Open( strcnn, L"", L"", NULL);
			}
			catch(...)
			{
				_bstr_t cn(udl);
				cn += path;
				cn += L".mdb";
				ADOX::_CatalogPtr catalog;
				catalog.CreateInstance(__uuidof ( ADOX::Catalog));
				auto _ = catalog->Create(cn);
				conn->Open( strcnn, L"", L"", NULL);	
				o(tables, *this);
			}
		}
		catch(...){}
	}

	template<typename Tables>CBase(const wchar_t *path, Tables &tables)
	{
		try
		{
			_bstr_t strcnn(udl);
			strcnn += path;
			strcnn += mdb;
			conn.CreateInstance(__uuidof(ADODB::Connection)); 
			conn->Open( strcnn, L"", L"", NULL);
		}
		catch(...){}
	}

	CBase(const wchar_t *path, bool ReadOnly = false)
	{
		_bstr_t strcnn(udl);
		strcnn += path;
		if(ReadOnly) strcnn += mdbRead; else strcnn += mdb;
	
		try
		{
		  conn.CreateInstance(__uuidof(ADODB::Connection)); 
		  conn->Open( strcnn, L"", L"", NULL);
		}
		catch(...){}
	}

	~CBase()
	{
		if(IsOpen())  conn->Close();
	}

	void Close()
	{
        if(IsOpen())  conn->Close();
	}

	bool IsOpen()
	{
		try
		{
			return conn->State == ADODB::adStateOpen;
		}
		catch(...)
		{
			return false;
		}
	}

	bool RecordSetOpen(wchar_t *strsql, ADODB::_RecordsetPtr &rst)
	{
		try
		{
			rst.CreateInstance(__uuidof(ADODB::Recordset));
			rst->Open(_bstr_t(strsql), conn.GetInterfacePtr(),
				ADODB::adOpenStatic, ADODB::adLockOptimistic, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	ADODB::_RecordsetPtr ConnectionSQL(wchar_t *strsql)
	{
		 return conn->Execute(strsql, NULL, ADODB::adExecuteNoRecords);
	}
	bool OpenTable(const wchar_t *table, ADODB::_RecordsetPtr &rec)
	{
		try
		{
			rec.CreateInstance(__uuidof(ADODB::Recordset));
			rec->Open(_variant_t(table), conn.GetInterfacePtr()
				, ADODB::adOpenStatic
				, ADODB::adLockOptimistic,ADODB::adCmdTable);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
};