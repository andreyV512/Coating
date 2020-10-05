#include "Dlg.h"
#include "DspFilters/DspFiltrParams.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "tools_debug/DebugMess.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/AScanWindow/AScanWindow.h"

template<class List>struct __orders__;
template<class Head, class ...Tail>struct __orders__<Vlst<Head, Tail...>>
{
	typedef typename __orders__<Vlst<Tail...>>::Result Result;
};
template<template<class>class X, template<class>class Y, class ...Tail>struct __orders__<Vlst<X<Y<Order>>, Tail...>>
{
	typedef typename VL::Append< X<Y<Order>>, typename __orders__<Vlst<Tail...>>::Result>::Result Result;
};
template<>struct __orders__<Vlst<>>
{
	typedef Vlst<> Result;
};

template<template<class>class X, template<class>class Y, class List>struct __filtr__;
template<template<class>class X, template<class>class Y, class Head, class ...Tail>struct __filtr__<X, Y, Vlst<Head, Tail...> >
{
	typedef typename __filtr__<X, Y, Vlst<Tail...>>::Result Result;
};
template<template<class>class X, template<class>class Y, class Head, class ...Tail>struct __filtr__<X, Y, Vlst<X<Y<Head>>, Tail...> >
{
	typedef typename VL::Append < X<Y<Head>>, typename __filtr__<X, Y, Vlst<Tail...>>::Result>::Result Result;
};
template<template<class>class X, template<class>class Y>struct __filtr__<X, Y, Vlst<> >
{
	typedef Vlst<> Result;
};

typedef VL::Append<__orders__<FiltersTable::items_list>::Result, CurrentFilter>::Result __orders_list__;

template<class T>struct __current_filtre_param_data__
{
	T obj;
	HWND h;
	bool close;
	typename T::TItems *pitems;
};

MIN_EQUAL_VALUE(Low<ChebI<Order>>, 1)
MAX_EQUAL_VALUE(Low<ChebI<Order>>, 10)
DO_NOT_CHECK(Low<ChebI<CutoffFrequency>>)
DO_NOT_CHECK(Low<ChebI<Ripple>>)

MIN_EQUAL_VALUE(High<ChebI<Order>>, 1)
MAX_EQUAL_VALUE(High<ChebI<Order>>, 10)
DO_NOT_CHECK(High<ChebI<CutoffFrequency>>)
DO_NOT_CHECK(High<ChebI<Ripple>>)

MIN_EQUAL_VALUE(BandPass<ChebI<Order>>, 1)
MAX_EQUAL_VALUE(BandPass<ChebI<Order>>, 10)
DO_NOT_CHECK(BandPass<ChebI<CenterFrequency>>)
DO_NOT_CHECK(BandPass<ChebI<WidthFrequency>>)
DO_NOT_CHECK(BandPass<ChebI<Ripple>>)

MIN_EQUAL_VALUE(BandStop<ChebI<Order>>, 1)
MAX_EQUAL_VALUE(BandStop<ChebI<Order>>, 10)
DO_NOT_CHECK(BandStop<ChebI<CenterFrequency>>)
DO_NOT_CHECK(BandStop<ChebI<WidthFrequency>>)
DO_NOT_CHECK(BandStop<ChebI<Ripple>>)

template<class T>struct CbItem;
template<>struct CbItem<Low<ChebI<Order>>>
{
	wchar_t* operator()() { return (wchar_t *)L"�������������� ������"; }
};
template<>struct CbItem<High<ChebI<Order>>>
{
	wchar_t* operator()() { return (wchar_t*)L"��������������� ������"; }
};
template<>struct CbItem<BandPass<ChebI<Order>>>
{
	wchar_t *operator()() { return (wchar_t *)L"��������� ������"; }
};
template<>struct CbItem<BandStop<ChebI<Order>>>
{
	wchar_t *operator()() { return (wchar_t *)L"����������� ������"; }
};
template<>struct CbItem<CurrentFilter>
{
	wchar_t *operator()() { return (wchar_t *)L"���������"; }
};

#define TITLE(type, name)template<template<class>class X, template<class>class Y>\
struct ParamTitle<X<Y<type>>>{wchar_t* operator()() { return (wchar_t*)name; }};
TITLE(Order              , L"������� �������")
TITLE(CutoffFrequency    , L"������� �����")
TITLE(Ripple             , L"��������������� � ������ �����������")

TITLE(CenterFrequency, L"����� ������")
TITLE(WidthFrequency, L"������ ������")
#undef TITLE
PARAM_TITLE(CurrentFilter, L"��� �������")

template<>struct FillComboboxList<CurrentFilter>
{
	HWND h;
	template<class O, class P>struct loc
	{
		void operator()(P &p)
		{
			ComboBox_AddString(p.h, CbItem<O>()());
		}
	};
	void operator()(HWND h, CurrentFilter& t)
	{
		this->h = h;
		VL::foreach<__orders_list__, loc>()(*this);
	}
};	 
template<>struct CurrentValue<CurrentFilter>
{
	void operator()(HWND h, CurrentFilter &t)
	{
		ComboBox_SetCurSel(h, t.value);
	}
};
template<>struct DlgSubItems<CurrentFilter, int> : ComboBoxSubItem<CurrentFilter> {};

DO_NOT_CHECK(CurrentFilter)


template<class P>struct __command__<Dialog::NoButton<CurrentFilter>, P>
{
	typedef Dialog::NoButton<CurrentFilter> O;
	bool operator()(O &o, P &p)
	{
		if (1 == p.command.isAcselerator)
		{
			Dialog::DlgItem2<CurrentFilter, typename P::Owner>(&item) = p.owner.items.get<Dialog::DlgItem2<CurrentFilter, typename P::Owner>>();
			item.value.value = ComboBox_GetCurSel(item.hWnd);
			p.owner.additional->close = false;
			EndDialog(p.command.hwnd, FALSE);
			return false;
		}
		return true;
	}
};

template<class Dlg>struct __data_from_widget__<Dialog::DlgItem2<CurrentFilter, Dlg>, int>
{
	typedef int T;
	typedef Dialog::DlgItem2<CurrentFilter, Dlg> O;
	T operator()(O &o)
	{
		return ComboBox_GetCurSel(o.hWnd);
	}
};

template<class O, class P>struct __current_filtre_param__;

//template<class From, class To>struct __copy__
//{
//	void operator()(From &o, To &p)
//	{
//		p.get<From>().value = o.value;
//	}
//};

template<>struct Dialog::NoButton<CurrentFilter>{};

template<template<class>class X, template<class>class Y, class O, class P>struct __current_filtre_param__<X<Y<O>>, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__, X<Y<O>>>::value == p.obj.items.get<CurrentFilter>().value)
		{
			typedef typename VL::Append<typename __filtr__<X, Y, FiltersTable::items_list>::Result, CurrentFilter>::Result list;
			p.close = true;
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<OkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
			{
				VL::CopyFromTo(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<class P>struct __current_filtre_param__<CurrentFilter, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__, CurrentFilter>::value == p.obj.items.get<CurrentFilter>().value)
		{
			typedef Vlst<CurrentFilter> list;
			p.close = true;
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<OkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
			{
				VL::CopyFromTo(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

void DspFiltrDlg::Do(HWND h)
{
	__current_filtre_param_data__<FiltersTable> data = {
		Singleton<FiltersTable>::Instance()
		, h
		, false
		, &Singleton<FiltersTable>::Instance().items
	};
	while(!data.close)
		VL::find<__orders_list__, __current_filtre_param__>()(data);
}

//struct TstOkBtn
//{
//	static const int width = 120;
//	static const int height = 30;
//	static const int ID = IDOK;
//	wchar_t *Title() { return (wchar_t *)L"���������"; }
//	template<class Owner>void BtnHandler(Owner &owner, HWND h)
//	{
//		__ok_table_btn__<
//			Owner::Base, Owner::Table
//			, typename VL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
//		>x;
//		if (x(h, owner))
//		{
//			EndDialog(h, TRUE);
//		}
//	}
//};
//
//template<class O, class P>struct __Tst_ok_btn__
//{
//	void operator()(O &o)
//	{
//		o.value.value = __data_from_widget__<O, typename VL::Inner<O>::Result::type_value>()(o);
//	}
//};
//struct TstFiltersTable : FiltersTable {};
//template<class Base>struct __ok_table_btn__<Base, TstFiltersTable, Vlst<>>
//{
//	template<class T>bool operator()(HWND h, T &t)
//	{
//		if (!VL::find<typename T::list, __test__>()(t.items, h))return false;
//		VL::foreach<typename T::list, __Tst_ok_btn__>()(t.items);
//		return true;
//	}
//};
//
//template<class O, class P>struct __Tst_current_filtre_param__;
//
//template<template<class>class X, template<class>class Y, class O, class P>struct __Tst_current_filtre_param__<X<Y<O>>, P>
//{
//	bool operator()(P &p)
//	{
//		if (VL::IndexOf<__orders_list__, X<Y<O>>>::value == p.obj.items.get<CurrentFilter>().value)
//		{
//			typedef typename VL::Append<typename __filtr__<X, Y, TstFiltersTable::items_list>::Result, CurrentFilter>::Result list;
//			p.close = true;
//			if (Dialog::Templ<ParametersBase, TstFiltersTable
//				, list
//				, 550
//				, Vlst<TstOkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
//				, __current_filtre_param_data__<TstFiltersTable>
//			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
//			{
//			//	VL::foreach<list, __copy__>()(p.obj.items, p.w->locFltParams);
//			}
//			return false;
//		}
//		return true;
//	}
//};
//
//template<class P>struct __Tst_current_filtre_param__<CurrentFilter, P>
//{
//	bool operator()(P &p)
//	{
//		if (VL::IndexOf<__orders_list__, CurrentFilter>::value == p.obj.items.get<CurrentFilter>().value)
//		{
//			typedef Vlst<CurrentFilter> list;
//			p.close = true;
//			if (Dialog::Templ<ParametersBase, TstFiltersTable
//				, list
//				, 550
//				, Vlst<TstOkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
//				, __current_filtre_param_data__<TstFiltersTable>
//			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
//			{
//				//VL::foreach<list, __copy__>()(p.obj.items, p.w->locFltParams);
//			}
//			return false;
//		}
//		return true;
//	}
//};
//
void TstDspFiltrDlg::Do(HWND h)
{
	//HWND hh = FindWindow(WindowClass<ZonesWindow>()(), 0);
	//if (NULL == hh)	 return;
	//ZonesWindow *w = ((ZonesWindow *)GetWindowLongPtr(hh, GWLP_USERDATA));
	//
	//__current_filtre_param_data__<TstFiltersTable> data = {
	//	w->locFltParams
	//	, h
	//	, false
	//	//, w
	//};
	//while (!data.close)
	//	VL::find<__orders_list__, __Tst_current_filtre_param__>()(data);
	//
	//w->Update();
}
///////////////////////////////////////////////////////////
template<class O, class P>struct __Xok_btn__
{
	void operator()(O &o)
	{
		o.value.value = __data_from_widget__<O, typename VL::Inner<O>::Result::type_value>()(o);
	}
};
struct NoStoreOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title() { return (wchar_t *)L"���������"; }
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		//__ok_table_btn__<
		//	Owner::Base, Owner::Table
		//	, typename VL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
		//>x;
		//if (TestPassword<Owner::Base, Owner::Table>()(h))
		//{
		//	if (x(h, owner))
		//	{
		//		EndDialog(h, TRUE);
		//	}
		//}

		if (!VL::find<typename Owner::list, __test__>()(owner.items, h))return;
		VL::foreach<typename Owner::list, __Xok_btn__>()(owner.items);
		EndDialog(h, TRUE);
	}
};

template<class O, class P>struct __Xcurrent_filtre_param__;

template<template<class>class X, template<class>class Y, class O, class P>struct __Xcurrent_filtre_param__<X<Y<O>>, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__, X<Y<O>>>::value == p.obj.items.get<CurrentFilter>().value)
		{
			typedef typename VL::Append<typename __filtr__<X, Y, FiltersTable::items_list>::Result, CurrentFilter>::Result list;
			p.close = true;
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<NoStoreOkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
			{
				VL::CopyFromTo(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<class P>struct __Xcurrent_filtre_param__<CurrentFilter, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__, CurrentFilter>::value == p.obj.items.get<CurrentFilter>().value)
		{
			typedef Vlst<CurrentFilter> list;
			p.close = true;
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<NoStoreOkBtn, CancelBtn, Dialog::NoButton<CurrentFilter>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, (wchar_t *)L"������"))
			{
				VL::CopyFromTo(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

void AScanDspFiltrDlg::Do(HWND h)
{
	FiltersTable table;
	
	AScanWindow *w = (AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	VL::CopyFromTo(w->computeFrame.paramFlt, table.items);
	__current_filtre_param_data__<FiltersTable> data = {
		table
		, h
		, false
		, &w->computeFrame.paramFlt
	};
	while (!data.close)
		VL::find<__orders_list__, __Xcurrent_filtre_param__>()(data);
	w->computeFrame.UpdateFiltre();
	RepaintWindow(w->hWnd);
}


