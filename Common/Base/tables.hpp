#pragma once
#include "templates/typelist.hpp"
#include "Base.hpp"
#include "templates/templates.hpp"
#include <ATLComTime.h>
//#include "tools_debug\DebugMess.h"

#pragma warning(disable:4995 4996)
#define ARG_TO_STR(x) #x
#define LINE(x) ARG_TO_STR(x)


#pragma warning(disable : 4996)
//template<int COUNT = 128>struct Holder
//{
//	typedef wchar_t TBufer[COUNT];
//	static const int count = COUNT;
//	TBufer buffer;
//	Holder &operator=(wchar_t *b)
//	{
//		wcsncpy(buffer, b, count - 1);
//		buffer[count - 1] = '\0';
//		return *this;
//	}
//	template<int C>Holder &operator=(Holder<C> &b)
//	{
//		wcsncpy(buffer, b.buffer, count - 1);
//		buffer[count - 1] = '\0';
//		return *this;
//	}
//	operator wchar_t *(){return buffer;}
//};

template<int COUNT = 128>struct CharHolder
{
	typedef char TBufer[COUNT];
	static const int count = COUNT;
	TBufer buffer;
	CharHolder &operator=(char *b)
	{
		strncpy(buffer, b, count - 1);
		buffer[count - 1] = '\0';
		return *this;
	}
	template<int C>CharHolder &operator=(CharHolder<C> &b)
	{
		strncpy(buffer, b.buffer, count - 1);
		buffer[count - 1] = '\0';
		return *this;
	}
	operator char *(){return buffer;}
};

template<class T, int COUNT>struct VArr
{
	typedef T TBuffer[COUNT];
	static const int maxCount = COUNT;
	int count;
	TBuffer buffer;
	VArr() : count(COUNT) {}
	bool  Set(T *date, int cnt)
	{
		count = cnt;
		bool b = maxCount > cnt;
		count = b ? cnt : maxCount;
		memmove(buffer, date, sizeof(T) * count);
		return !b;
	}
	bool Get(T *date, int &cnt)
	{
		if (count < cnt)
		{
			cnt = count;
			memmove(date, buffer, cnt * sizeof(T));
			return true;
		}
		return false;
	}
};

template<class T>struct len
{
	int operator()(T& o) { return 0; }
};
template<int N>struct len<Holder<N>>
{
	int operator()(Holder<N> &o){return (int)wcslen(o.buffer) + 1;}
};

template<int N>struct len<CharHolder<N>>
{
	int operator()(CharHolder<N> &o){return (int)strlen(o.buffer) + 1;}
};

template<class T, int N>struct len<T[N]>
{
	int operator()(T(&t)[N]){return sizeof(t);}
};

template<class T, int N>struct len<VArr<T, N>>
{
	int operator()(VArr<T, N>(&t)) { return sizeof(T) * t.count; }
};

	template<class T>struct Tpe
	{
		T operator()(T t){return t;}
	};

	template<int N>struct Tpe<Holder<N>>
	{
		wchar_t *operator()(Holder<N> &t){return (wchar_t *)t.buffer;}
	};	

	template<int N>struct Tpe<CharHolder<N>>
	{
		char *operator()(CharHolder<N> &t){return t.buffer;}
	};

	template<>struct Tpe<COleDateTime>
	{
		_variant_t operator()(COleDateTime t)
		{
			return _variant_t(t, VT_DATE);
		}
	};
	template<class A, int N>struct Tpe<A[N]>
	{
		_variant_t operator()(A(&t)[N])
		{				
			A *f;
			SAFEARRAY *s = SafeArrayCreateVector(VT_UI1, 0, N * sizeof(A));
			SafeArrayAccessData(s, (void **)&f);
			memmove(f, t, sizeof(A) * N);
			SafeArrayUnaccessData(s);
			_variant_t v;
			v.vt = VT_ARRAY | VT_UI1;
			v.parray = s;
			return v;
		}
	};
	template<class A, int N>struct Tpe<VArr<A, N>>
	{
		_variant_t operator()(VArr<A, N>(&t))
		{
			A *f;
			int size = sizeof(A) * t.count;
			SAFEARRAY *s = SafeArrayCreateVector(VT_UI1, 0, size);
			SafeArrayAccessData(s, (void **)&f);
			memmove(f, t.buffer, size);
			SafeArrayUnaccessData(s);
			_variant_t v;
			v.vt = VT_ARRAY | VT_VARIANT;
			v.parray = s;
			return v;
		}
	};
#pragma warning(default : 4996)

	struct Access{};
	struct MSsql{};

template<class Base, typename T>struct value_type;

template<class Base, typename T, int N>struct value_type<Base, T[N]>
{
	static wchar_t *Type()
	{		
		return L"LongBinary";	
	}
};
template<typename T, int N>struct value_type<MSsql, VArr<T, N>>
{
	static wchar_t *Type()
	{		
		return (wchar_t *)L"Varbinary(MAX)";	
	}
};
template<class Base>struct value_type<Base, int>
{
	static const wchar_t *Type()
	{
		return L"Integer";
	}
};
template<class Base>struct value_type<Base, unsigned>
{
	static wchar_t *Type()
	{
		return (wchar_t *)L"Integer";
	}
};
template<class Base>struct value_type<Base, double>
{
	static wchar_t *Type()
	{
		return (wchar_t *)L"Double";
	}
};
template<>struct value_type<MSsql, double>
{
	static const wchar_t *Type()
	{
		return L"Float";
	}
};
template<class Base>struct value_type<Base, float>
{
	static wchar_t *Type()
	{
		return L"Single";
	}
};
template<class Base>struct value_type<Base, char *>
{
	static wchar_t *Type()
	{
		return L"VARCHAR";
	}
};
template<class Base>struct value_type<Base, wchar_t *>
{
	static wchar_t *Type()
	{
		return L"VARCHAR";
	}
};

template<class Base>struct value_type<Base, bool>
{
	static wchar_t *Type()
	{
		return (wchar_t *)L"bit";
	}
};

template<class Base>struct value_type<Base, char>
{
	static wchar_t *Type()
	{
		return L"byte";
	}
};

template<class Base>struct value_type<Base, unsigned char>
{
	static wchar_t *Type()
	{
		return L"byte";
	}
};

struct __value_type_holder_N
{
	wchar_t *operator()(int N)
	{
		static wchar_t buf[128];
		wsprintf(buf, L"VARCHAR(%d)", N);
		return buf;
	}
};

template<class Base, int N>struct value_type<Base, Holder<N>>
{
	static wchar_t *Type()
	{
		return __value_type_holder_N()(N);
	}
};

template<class Base>struct value_type<Base, COleDateTime>
{
	static wchar_t *Type()
	{
		return (wchar_t *)L"DateTime";
	}
};

template<typename T>struct TypeToInt;
template<>struct TypeToInt<int>{static const ADODB::DataTypeEnum value = ADODB::adInteger;};
template<>struct TypeToInt<unsigned>{static const ADODB::DataTypeEnum value = ADODB::adInteger;};
template<>struct TypeToInt<double>{static const ADODB::DataTypeEnum value = ADODB::adDouble;};
template<>struct TypeToInt<float>{static const ADODB::DataTypeEnum value = ADODB::adSingle;};
template<>struct TypeToInt<float *>{static const ADODB::DataTypeEnum value = ADODB::adSingle;};
template<class A, int N>struct TypeToInt<A[N]>{static const ADODB::DataTypeEnum value = ADODB::adLongVarBinary;};
template<>struct TypeToInt<wchar_t *>{static const ADODB::DataTypeEnum value = ADODB::adWChar;};
template<>struct TypeToInt<char *>{static const ADODB::DataTypeEnum value = ADODB::adChar;};
template<>struct TypeToInt<char>{static const ADODB::DataTypeEnum value = ADODB::adTinyInt;};
template<>struct TypeToInt<unsigned char>{static const ADODB::DataTypeEnum value = ADODB::adTinyInt;};
template<>struct TypeToInt<bool>{static const ADODB::DataTypeEnum value = ADODB::adBoolean;};
template<>struct TypeToInt<double *>{static const ADODB::DataTypeEnum value = ADODB::adDouble;};
template<>struct TypeToInt<COleDateTime>{static const ADODB::DataTypeEnum value = ADODB::adDate;};
template<>struct TypeToInt<SAFEARRAY *>{static const ADODB::DataTypeEnum value = ADODB::adArray;};

template<int N>struct TypeToInt<Holder<N>>{static const ADODB::DataTypeEnum value = ADODB::adWChar;};
template<int N>struct TypeToInt<CharHolder<N>>{static const ADODB::DataTypeEnum value = ADODB::adChar;};

template<class A, int N>struct TypeToInt<VArr<A, N>> { static const ADODB::DataTypeEnum value = ADODB::adLongVarBinary; };
//--------------------------------------------------------------------------------------------------
template<typename T, int COUNT>struct TArray
{
	typedef T *type_value;
	static const int count = COUNT;
	T *value;

};
//-----------------------------------------------------------------------------------------------
template<class T>struct IsUNIQUE
{
	template<class Z>static double Is(Z *, typename Z::UNIQUE * = NULL);
	template<class Z>static char Is(...);
	static const int value = sizeof(double) == sizeof(Is<T>((T *)0));
};
//------------------------------------------------------------------------
template<typename T>int CountCharts(T &)
{
	return 0;
}
template<int C>int CountCharts(Holder<C> &t)
{
	return wcslen(t);
}
template<int C>int CountCharts(CharHolder<C> &t)
{
	return strlen(t);
}
template<int N>int CountCharts(float (&t)[N])
{
	return sizeof(float);
}
//-------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996 4995)
template<class TypeBase, typename TParam, typename List, int N = 4096>struct QuerySQL_CreateTable
{	
		QuerySQL_CreateTable(TParam &param)
		{
			 wcscpy(buf, L"create table ");
			 wcscat(buf, param.name());
			 wcscat(buf, L"(ID int IDENTITY(2,1) PRIMARY KEY ");
			 VL::foreach<List, init>()(param.items, buf);
			 wcscat(buf, L")");
		}
		wchar_t *operator()(){return buf;}
private:
	    wchar_t buf[N];

		template<bool b>struct __is_unique__
		{
			wchar_t *operator()(wchar_t *){return (wchar_t *)L"";}
		};
		template<>struct __is_unique__<true>
		{
			wchar_t *operator()(wchar_t *s){return wcscat(s, L" NOT NULL UNIQUE");}
		};
		
		template<typename O, typename Str>struct init
		{
			void operator()(O &o, Str &str)
			{
				type<O::type_value>()(o, str);
			}
			template<typename T>struct type
			{
				void operator()(O &o, Str &str)
				{
					wcscat(str, L",");
					wcscat(str, o.name());
					wcscat(str, L" ");
					wcscat(str, value_type<TypeBase, O::type_value>::Type());
					__is_unique__<IsUNIQUE<O>::value>()(str);
				}
			};
			template<>struct type<char *>
			{
				void operator()(O &o, Str &str)
				{
					wcscat(str, L",");
					wcscat(str, o->name_param);
					wcscat(str, L" ");
					wcscat(str, value_type<O::type_value>::Type());
				}
			};
			template<>struct type<wchar_t *>
			{
				void operator()(O &o, Str &str)
				{
					wcscat(str, L",");
					wcscat(str, o.name());
					wcscat(str, L" ");
					wcscat(str, value_type<O::type_value>::Type());
				}
			};
			template<typename T>struct type<T *>
			{
				void operator()(O &o, Str&str)
				{
					wchar_t const *type_name = value_type<T>::Type();
					for(int i = 0; i < O::count; ++i)
					{
						wcscat(str, L",d");
						_itow(i, str + wcslen(str), 10);
						wcscat(str, L" ");
						wcscat(str, type_name);
					}
				}
			};
            template<>struct type<SAFEARRAY *>
			{
				typedef SAFEARRAY *T;
				void operator()(O &o, Str &str)
				{
					wcscat(str, L",");
					wcscat(str, o.name());
					wcscat(str, L" ");
					wcscat(str, value_type<O::type_value>::Type());
				}
			};
		};
};
//-------------------------------------------------------------------------------------------
template<class T>struct SetDefaultParam
{
 template<typename Tables, typename Base>void operator()(Tables &tables, Base &base)
 {
	 T()(tables, base);
 }
};
template<>struct SetDefaultParam<Vlst<>>
{
	template<typename Tables, typename Base>void operator()(Tables &tables, Base &base){}
};
template<typename SubList, typename SetDefault = Vlst<>, typename TypeBase = Access>class CreateDataBase
{
	template<typename O, typename P>struct init
	{
		void operator()(O &o, P &p)
		{
			auto _ = p.ConnectionSQL(
			    QuerySQL_CreateTable<TypeBase, O, O::items_list, 8 * 1024>(o)()
			);
		}
	};
public:
    template<typename Tables, typename Base>void operator()(Tables &tables, Base &base)
	{
		VL::foreach<SubList, init>()(tables, base);
		SetDefaultParam<SetDefault>()(tables, base);
	}
};
#pragma warning( disable : 4996 )
//---------------------------------------------------------------------------------------------------------
template<typename Table> struct Insert_Into;
template<typename Table, typename Base>struct set_default_table
{
	void operator()(Table &table, Base &base)
	{
		Insert_Into<Table>(table, base).Execute();
	}
};
template<typename List>struct SetDefault
{	
	template<typename Tables, typename Base>void operator()(Tables &tables, Base &base)
	{
        VL::foreach<List, set_default_table>()(tables, base);
	}
};
//---------------------------------------------------------------------------------------------------------
template<typename Table> struct Insert_Into
{
   wchar_t head[1024];
   wchar_t tail[1024];
   Insert_Into(Table &tableX, CBase &baseX)
	   : table(tableX)
	   , base(baseX)
   {
	   wcscpy(head, L"INSERT INTO ");
	   wcscat(head, table.name());
	   wcscat(head, L"(");
	   wcscpy(tail, L")VALUES(");
   }
   void Execute()
   {
	  cmd.CreateInstance(__uuidof(ADODB::Command));
	  cmd->ActiveConnection = base.conn;
	  cmd->CommandType = ADODB::adCmdText;
	  VL::foreach<Table::items_list, init>()(table.items, *this);
	  wcscpy(head + wcslen(head) - 1, tail);
	  head[wcslen(head) - 1] = ')';
	  cmd->CommandText = head;
	  cmd->Execute(NULL, NULL, ADODB::adCmdText);
   }
   template<class List>void Execute()
   {
	   cmd.CreateInstance(__uuidof(ADODB::Command));
	   cmd->ActiveConnection = base.conn;
	   cmd->CommandType = ADODB::adCmdText;
	   VL::foreach<List, init>()(table.items, *this);
	   wcscpy(head + wcslen(head) - 1, tail);
	   head[wcslen(head) - 1] = ')';
	   cmd->CommandText = head;
	   _variant_t rowsAffected; 
	   cmd->Execute(&rowsAffected, 0, ADODB::adCmdText);
   }
  
private:
   Table &table;
   CBase &base;
   ADODB::_CommandPtr cmd;
																	  
   template<typename T>struct insertX
   {	
	   template<typename T, int COUNT, typename P>void set(TArray<T, COUNT> &o, P &p)
	   {
		   for(int i = 0; i < TArray<T, COUNT>::count; ++i)
		   {
			   wcscat(p.head, L"d");
			   _itow(i, wcslen(p.head) + p.head, 10);
			   wcscat(p.head, L",");
			   wcscat(p.tail, L"?,");
			   p.cmd->Parameters->Append(
				   p.cmd->CreateParameter( 
				   ""
				   , TypeToInt<TArray<T, COUNT>::type_value>::value
				   , ADODB::adParamInput
				   , 0
				   , o.value[i]
			       )
				   );
		   }
	   }
	   template<typename O, typename P>void set(O &o, P &p)
	   {
		   wcscat(p.head, o.name());
		   wcscat(p.head, L",");
           wcscat(p.tail, L"?,");
		   p.cmd->Parameters->Append(
			   p.cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(o.value)
				 , Tpe<O::type_value>()(o.value)
				 )
		   );
	   }
	   
	   template<typename O, typename P>void operator()(O &o, P &p)
	   {
		    set(o, p);
	   }
   };

   template<typename O, typename P>struct init
   {
	   void operator()(O &o, P &p)
	   {
		   insertX<O::type_value>()(o, p);
	   }
   };
};
#pragma warning( disable : 4996 )

struct SQL
{
	wchar_t head[1024];
	ADODB::_CommandPtr cmd;
	CBase &base;
	explicit SQL(CBase &base)
		: base(base)
	{
		cmd.CreateInstance(__uuidof(ADODB::Command));
		cmd->ActiveConnection = base.conn;
		cmd->CommandType = ADODB::adCmdText;
		head[0] = 0;
	}
	void Clear(){head[0] = 0;}
	SQL &Txt(wchar_t *w)
	{
		wcscat(head, w);
		return *this;
	}
	template<class T>SQL &Val(T &p)
	{
		cmd->Parameters->Append(
			cmd->CreateParameter( 
			""
			, TypeToInt<T>::value
			, ADODB::adParamInput
			, len<T>()(p)
			, Tpe<T>()(p)
			)
			);
		return *this;
	}
	
	template<class T>SQL &Name()
	{
		wcscat(head, T().name());
		return *this;
	}
	int operator()(ADODB::_RecordsetPtr &rec)
	{
		try
		{
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}
    int operator()(wchar_t *query, ADODB::_RecordsetPtr &rec)
	{
		try
		{
			cmd->CommandText = query;
			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}
};


template<typename Table>struct Select
{
	wchar_t head[4096];
	ADODB::_CommandPtr cmd;
	CBase &base;
    explicit Select(CBase &base)
	: base(base)
	{
	   wcscpy(head, L"SELECT * from ");
	   wcscat(head, Table().name());
	   wcscat(head, L" WHERE ");
	   cmd.CreateInstance(__uuidof(ADODB::Command));
	   cmd->ActiveConnection = base.conn;
	   cmd->CommandType = ADODB::adCmdText;
	}

	template<typename O>Select &eq(typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, L"=?   AND ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	template<typename O>Select &op(wchar_t *operand, typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, operand);
		wcscat(head, L"?   AND ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	template<typename O>Select &compare(wchar_t *cp, typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, cp);
		wcscat(head, L"?   AND ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	template<typename O>Select &eqOR(typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, L"=?    OR ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	Select &ID(int p)
	{
		wcscat(head, L"ID=?   AND ");
		cmd->Parameters->Append(
			cmd->CreateParameter( 
			""
			, TypeToInt<int>::value
			, ADODB::adParamInput
			, 0
			, p
			)
			);
		return *this;
	}
	template<typename O, typename P>struct eq_all_
	{
		void operator()(O &o, P &p)
		{
			wcscat(p.head, o.name());
			wcscat(p.head, L"=?   AND ");
			p.cmd->Parameters->Append(
				p.cmd->CreateParameter( 
				""
				, TypeToInt<O::type_value>::value
				, ADODB::adParamInput
			    , len<typename O::type_value>()(o.value)
			    , Tpe<O::type_value>()(o.value)
				)
				);
		}
	};
	template< typename List, typename F>Select &eq_all(F &f)
	{
		VL::foreach<List, eq_all_>()(f, *this);
		return *this;
	}
	template<class T>struct convert
	{
		template<class O, class P>void operator()(O &o, P p)
		{
			o.value = Tpe<T>()(p);
		}
	};

	template<class T, int N>struct convert<T[N]>
	{
		template<class O, class P>void operator()(O &o, P p)
		{
			T *f;
			SafeArrayAccessData(p.parray, (void **)&f);
			memmove(o.value, f, sizeof(o.value));
			SafeArrayUnaccessData(p.parray);
		}
	};

	template<class T, int N>struct convert<VArr<T, N>>
	{
		template<class O, class P>void operator()(O *o, P p)
		{
			T *f;
			SafeArrayAccessData(p.parray, (void **)&f);
			LONG len = 0;
			SafeArrayGetUBound(p.parray, 1, &len);
			++len;

			memmove(o->value.buffer, f, len);
			SafeArrayUnaccessData(p.parray);

			len /= sizeof(T);
			o->value.count = N < len ? N : len;
		}
	};

	template<int N>struct convert<Holder<N>>
	{
		template<class O, class P>void operator()(O &o, P p)
		{
			o.value = _bstr_t(p);
		}
	};
	template<typename O, typename P>struct set_to_
	{
		void operator()(O &o, P p)
		{
			convert<O::type_value>()(o, p->Fields->GetItem(o.name())->GetValue());
		}
	};
	template<typename F>int Execute(F &f)
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			VL::foreach<F::items_list, set_to_>()(f.items, rec.GetInterfacePtr());
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}
	int Execute(ADODB::_RecordsetPtr &rec)
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}	
	template<template<class, class>class Proc, class Data>bool ExecuteLoop(Data &d)
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			Table table;
			while (!rec->EndOfFile) 
			{
				VL::foreach<Table::items_list, set_to_>()(table.items, rec.GetInterfacePtr());
				if(Proc<Table, Data>()(rec->Fields->GetItem(L"ID")->GetValue(), table, d)) return true;
				rec->MoveNext(); 
			}
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
	template<template<class, class>class Proc, class list, class Data>bool ExecuteLoop(Data &d)
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected;
			ADODB::_RecordsetPtr rec = cmd->Execute(&rowsAffected, 0, ADODB::adCmdText);
			Table table;
			while (!rec->EndOfFile)
			{
				VL::foreach<list, set_to_>()(table.items, rec.GetInterfacePtr());
				if (Proc<Table, Data>()(rec->Fields->GetItem(L"ID")->GetValue(), table, d)) return true;
				rec->MoveNext();
			}
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	template<class O, class P>struct __order_by__
	{
		void operator()(O *, P *p)
		{
			wcscat(p, O().name());
			wcscat(p, L",");
		}
	};
	template<class ORDER_BY_LIST>int Execute(ADODB::_RecordsetPtr &rec)
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			wcscat(head, L" ORDER BY ");
			VL::foreach<ORDER_BY_LIST, __order_by__>()(head);
			head[wcslen(head) - 1] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}	
	int Execute()
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return rec->Fields->GetItem(L"ID")->GetValue();
		}
		catch(...)
		{
			return 0;
		}
	}

};

template<typename Table, int N = 1024>struct Update
{
	wchar_t head[N];
	ADODB::_CommandPtr cmd;
	CBase &base;
    Update(CBase &base)
		: base(base)
	{
	   wcscpy(head, L"UPDATE ");
	   wcscat(head, Table().name());
	   wcscat(head, L" SET ");
	   cmd.CreateInstance(__uuidof(ADODB::Command));
	   cmd->ActiveConnection = base.conn;
	   cmd->CommandType = ADODB::adCmdText;
	}
	

	template<typename O>Update &set(typename O::type_value &p)
	{
        wcscat(head, O().name());
		wcscat(head, L"=?,");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );		
		return *this;
	}	
	Update &Where()
	{
		wcscpy(head + wcslen(head) - 1, L" WHERE "); 
		return *this;
	}
	Update &ID(int p)
   {
	   wcscat(head, L"ID=?");
	   wcscat(head, L" AND ");
	   cmd->Parameters->Append(
		   cmd->CreateParameter( 
		   ""
		   , TypeToInt<int>::value
		   , ADODB::adParamInput
		   , 0
		   , p
		   )
		   );
	   return *this;
   }
	template<typename O>Update &eq(typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, L"=?");
		wcscat(head, L" AND ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	bool Execute()
	{
		try
		{
			head[wcslen(head) - 5] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
};
//------------------------------------------------------------------------------------
template<typename Table> struct UpdateWhere
{
   wchar_t head[1024];
   UpdateWhere(Table &tableX, CBase &baseX)
	   : table(tableX)
	   , base(baseX)
   {
	   wcscpy(head, L"UPDATE ");
	   wcscat(head, table.name());
	   wcscat(head, L" SET ");
	   cmd.CreateInstance(__uuidof(ADODB::Command));
	   cmd->ActiveConnection = base.conn;
	   cmd->CommandType = ADODB::adCmdText;
	   VL::foreach<Table::items_list, insert>()(table.items, *this);
	   wcscpy(head + wcslen(head) - 1, L" WHERE ");
   }
   
   void Execute()
   {
	   head[wcslen(head) - 5] = '\0';
	   cmd->CommandText = head;
	   _variant_t rowsAffected; 
	   cmd->Execute( &rowsAffected, 0, ADODB::adCmdText | ADODB::adExecuteNoRecords);
   }
   template<typename O>UpdateWhere &eq(typename O::type_value p)
   {
	   wcscat(head, O().name());
	   wcscat(head, L"=? AND ");	  
	   cmd->Parameters->Append(
		   cmd->CreateParameter( 
		   ""
		   , TypeToInt<O::type_value>::value
		   , ADODB::adParamInput
		   , len<typename O::type_value>()(p)
		   , Tpe<O::type_value>()(p)
		   )
		   );
	   return *this;
   }
   UpdateWhere &ID(int p)
   {
	   wcscat(head, L"ID=? AND ");
	   cmd->Parameters->Append(
		   cmd->CreateParameter( 
		   ""
		   , TypeToInt<int>::value
		   , ADODB::adParamInput
		   , 0
		   , p
		   )
		   );	   
	   return *this;
   }
private:
   Table &table;
   CBase &base;
   ADODB::_CommandPtr cmd;
   template<typename O, typename P>struct insert
   {
	   template<typename O, typename P>void set(O &o, P &p)
	   {
		   wcscat(p.head, o.name());
		   wcscat(p.head, L"=?,");
		   p.cmd->Parameters->Append(
			   p.cmd->CreateParameter( 
			   ""
			   , TypeToInt<O::type_value>::value
			   , ADODB::adParamInput
			    , len<typename O::type_value>()(o.value)
				 , Tpe<O::type_value>()(o.value)
			   )
			   );
	   }
	   void operator()(O &o, P &p)
	   {
		   set(o, p);
	   }
   };
   /*
   template<typename P>struct insert<double *, P>
   {
	   void operator()(double **o, P *p)
	   {
		   wcscat(p->head, o->name());
		   wcscat(p->head, L"=?,");
		   p->cmd->Parameters->Append(
			   p->cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
				 , 0
				 , o->value()
				 )
		   );
	   }
   };
   */
};
//-------------------------------------------------------------------------------------
template<typename Table>struct Delete
{
	wchar_t head[4096];
	ADODB::_CommandPtr cmd;
	CBase &base;
    explicit Delete(CBase &base)
	: base(base)
	{
	   wcscpy(head, L"DELETE from ");
	   wcscat(head, Table().name());
	   wcscat(head, L" WHERE ");
	   cmd.CreateInstance(__uuidof(ADODB::Command));
	   cmd->ActiveConnection = base.conn;
	   cmd->CommandType = ADODB::adCmdText;
	}
	template<typename O>Delete &eq(typename O::type_value p)
	{
        wcscat(head, O().name());
		wcscat(head, L"=?");
		wcscat(head, L" AND ");
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
			     , len<typename O::type_value>()(p)
				 , Tpe<O::type_value>()(p)
				 )
		   );
		return *this;
	}
	Delete &ID(int p)
	{
		wcscat(head, L"ID=?");
		wcscat(head, L" AND ");
		cmd->Parameters->Append(
			cmd->CreateParameter( 
			""
			, TypeToInt<int>::value
			, ADODB::adParamInput
			, 0
			, p
			)
			);
		return *this;
	}
	template<typename O, typename P>struct eq_all_
	{
		void operator()(O *o, P *p)
		{
			wcscat(p->head, o->name());
			wcscat(p->head, L"=?");
			wcscat(p->head, L" AND ");
			p->cmd->Parameters->Append(
				p->cmd->CreateParameter( 
				""
				, TypeToInt<O::type_value>::value
				 , ADODB::adParamInput
			     , len<typename O::type_value>()(o->value)
				 , Tpe<O::type_value>()(o->value)
				)
				);
		}
	};
	template< typename List, typename F>Delete &eq_all(F *f)
	{
		VL::foreach<List, eq_all_>()(f, this);
		return *this;
	}
	//template<typename O, typename P>struct set_to_
	//{
	//	void operator()(O &o, P &p)
	//	{
	//		VARIANT v = p.Fields->GetItem(o.name())->GetValue();
	//		//TODO o.value = *(O::type_value *)&v.punkVal;
	//	}
	//};
	bool Execute()
	{
		try
		{
			head[wcslen(head) - 5] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
	bool All()
	{
		try
		{
			head[wcslen(head) - 7] = '\0';
			cmd->CommandText = head;
			_variant_t rowsAffected; 
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
//------------------------------------------------------------------
    template<class T>struct Inner;
	template<template<class>class Wrap, class T>struct Inner<Wrap<T> >
	{
		typedef T Result;
	};
};

struct CMD
{
	ADODB::_CommandPtr cmd;
	CBase &base;
	CMD(CBase &b)
		: base(b)
	{
		cmd.CreateInstance(__uuidof(ADODB::Command));
		cmd->ActiveConnection = base.conn;
		cmd->CommandType = ADODB::adCmdText;
	}
	CMD &CommandText(wchar_t *txt)
	{
		cmd->CommandText = txt;
		return *this;
	}
	template<class T>CMD &GetValue(wchar_t *nameParam, T &value)
	{
		_variant_t rowsAffected; 
		ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
		value = rec->Fields->GetItem(nameParam)->GetValue();
		return *this;
	}
	void Execute()
	{
		_variant_t rowsAffected; 
		ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
	}
	template<typename T>CMD &Param(T &t)
	{       
		cmd->Parameters->Append(
			   cmd->CreateParameter( 
			     ""
				 , TypeToInt<T>::value
				 , ADODB::adParamInput
				 , len<T>()(t)
				 , Tpe<T>()(t)
				 )
		   );
		return *this;
	}
};

#pragma warning( disable : 4996 )