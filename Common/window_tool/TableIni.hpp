#pragma once
#include "ItemIni.h"
#include "templates/templates.hpp"
#include "DlgTemplates/ParamDlg.hpp"

struct ParametersIni { typedef Vlst<>multy_type_list; };

struct __ok_btn_ini_data__
{
	wchar_t *section;
	wchar_t *fileName;
};

template<class O, class P>struct __ok_btn_ini__
{
	void operator()(O &o, P &p)
	{
		typedef typename VL::Inner<O>::Result::type_value TVal;
		o.value.value = __data_from_widget__<O, TVal>()(o);
		ItemIni::Set(p.section, (wchar_t *)o.value.name(), o.value.value, p.fileName);
	}
};

template<class Base, class Table, class T>struct __ok_table_btn__;

template<class Table, class X>struct __ok_table_btn__<ParametersIni, Table, X>
{
	template<class T>bool operator()(HWND h, T &t)
	{
		if (!VL::find<T::list, __test__>()(t.items, h))return false;
		
		wchar_t path[1024];
		ItemIni::GetPath(path);
		__ok_btn_ini_data__ data = { (wchar_t *)Table().name(), path };
		VL::foreach<T::list, __ok_btn_ini__>()(t.items, data);
		return true;
	}
};

struct StoreIniTable
{
	wchar_t *section;
	wchar_t path[1024];

	template<class O, class P>struct loc
	{
		void operator()(O &o, P &p)
		{
			ItemIni::Set(p.section, o.name(), o.value, p.path);
		}
	};
	template<class Table>StoreIniTable(Table &t)
	{
		section = t.name();
		ItemIni::GetPath(path);
		VL::foreach<typename Table::items_list, loc>()(t.items, *this);
	}
};

struct LoadIniTable
{
	wchar_t *section;
	wchar_t path[1024];

	template<class O, class P>struct loc
	{
		void operator()(O &o, P &p)
		{
			wchar_t val[2048];
			ItemIni::Get(p.section, (wchar_t *)o.name(), val, p.path);
			o.value = Wchar_to<typename O::type_value>()(val);
		}
	};
	template<class Table>LoadIniTable(Table &t)
	{
		section = (wchar_t *)t.name();
		ItemIni::GetPath(path);
		VL::foreach<typename Table::items_list, loc>()(t.items, *this);
	}
};