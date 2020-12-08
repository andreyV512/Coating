#include "Dlg.h"
#include "DspFilters/DspFiltrParams.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "tools_debug/DebugMess.h"
#include "DlgTemplates/ParamDlg.hpp"
#include "Windows/ZonesWindow/ZonesWindow.h"
#include "window_tool/Emptywindow.h"
#include "Windows/AScanWindow/AScanWindow.h"
///*
template<class List, int N>struct __orders__;
template<int N, class Head, class ...Tail>struct __orders__<Vlst<Head, Tail...>, N>
{
	typedef typename __orders__<Vlst<Tail...>, N>::Result Result;
};
template<int N, template<class>class X, template<class>class Y, class ...Tail>struct __orders__<Vlst<Num<X<Y<Order>>, N>, Tail...>, N>
{
	typedef typename VL::Append< Num<X<Y<Order>>, N>, typename __orders__<Vlst<Tail...>, N>::Result>::Result Result;
};
template<int N>struct __orders__<Vlst<>, N>
{
	typedef Vlst<> Result;
};

template<template<class>class X, template<class>class Y, class List, int N>struct __filtr__;
template<int N, template<class>class X, template<class>class Y, class Head, class ...Tail>struct __filtr__<X, Y, Vlst<Head, Tail...>, N >
{
	typedef typename __filtr__<X, Y, Vlst<Tail...>, N>::Result Result;
};
template<int N, template<class>class X, template<class>class Y, class Head, class ...Tail>struct __filtr__<X, Y, Vlst<Num<X<Y<Head>>, N>, Tail...>, N >
{
	typedef typename VL::Append < Num<X<Y<Head>>, N>, typename __filtr__<X, Y, Vlst<Tail...>, N>::Result>::Result Result;
};
template<int N, template<class>class X, template<class>class Y>struct __filtr__<X, Y, Vlst<>, N >
{
	typedef Vlst<> Result;
};

template<int N>struct __orders_list__
{
	typedef typename VL::Append<typename __orders__<FiltersTable::items_list, N>::Result, Num<CurrentFilter, N>>::Result Result;
};

template<class T>struct __current_filtre_param_data__
{
	T obj;
	HWND h;
	bool close;
	typename T::TItems *pitems;
	int currentSensor;
};

#define XMIN_EQUAL_VALUE(sub_type, value)template<int N>struct LessEqual<Num<sub_type, N>>\
{typename Num<sub_type, N>::type_value operator()(){return value;}};


#define XMAX_EQUAL_VALUE(sub_type, value)template<int N>struct LargenEqual<Num<sub_type, N>>\
{typename Num<sub_type, N>::type_value operator()(){return value;}};

#define XDO_NOT_CHECK(n)template<int N>struct __compare_param__<Vlst<>, Vlst<>, Num<n, N>>\
{bool operator()(typename Num<n, N>::type_value &t){return true;}};

XMIN_EQUAL_VALUE(Low<ChebI<Order>>, 1)
XMAX_EQUAL_VALUE(Low<ChebI<Order>>, 10)
XDO_NOT_CHECK(Low<ChebI<CutoffFrequency>>)
XDO_NOT_CHECK(Low<ChebI<Ripple>>)

XMIN_EQUAL_VALUE(High<ChebI<Order>>, 1)
XMAX_EQUAL_VALUE(High<ChebI<Order>>, 10)
XDO_NOT_CHECK(High<ChebI<CutoffFrequency>>)
XDO_NOT_CHECK(High<ChebI<Ripple>>)

XMIN_EQUAL_VALUE(BandPass<ChebI<Order>>, 1)
XMAX_EQUAL_VALUE(BandPass<ChebI<Order>>, 10)
XDO_NOT_CHECK(BandPass<ChebI<CenterFrequency>>)
XDO_NOT_CHECK(BandPass<ChebI<WidthFrequency>>)
XDO_NOT_CHECK(BandPass<ChebI<Ripple>>)

XMIN_EQUAL_VALUE(BandStop<ChebI<Order>>, 1)
XMAX_EQUAL_VALUE(BandStop<ChebI<Order>>, 10)
XDO_NOT_CHECK(BandStop<ChebI<CenterFrequency>>)
XDO_NOT_CHECK(BandStop<ChebI<WidthFrequency>>)
XDO_NOT_CHECK(BandStop<ChebI<Ripple>>)
template<class T>struct CbItem;
template<int N>struct CbItem<Num<Low<ChebI<Order>>, N>>
{
	wchar_t* operator()() { return (wchar_t *)L"Низкочастотный фильтр"; }
};
template<int N>struct CbItem<Num<High<ChebI<Order>>, N>>
{
	wchar_t* operator()() { return (wchar_t*)L"Высокочастотный фильтр"; }
};
template<int N>struct CbItem<Num<BandPass<ChebI<Order>>, N>>
{
	wchar_t *operator()() { return (wchar_t *)L"Полосовой фильтр"; }
};
template<int N>struct CbItem<Num<BandStop<ChebI<Order>>, N>>
{
	wchar_t *operator()() { return (wchar_t *)L"Режекторный фильтр"; }
};
template<int N>struct CbItem<Num<CurrentFilter, N>>
{
	wchar_t *operator()() { return (wchar_t *)L"Отключено"; }
};

#define TITLE(type, name)template<template<class>class X, template<class>class Y, int N>\
struct ParamTitle<Num<X<Y<type>>, N>>{wchar_t* operator()() { return (wchar_t*)name; }};
TITLE(Order              , L"Порядок фильтра")
TITLE(CutoffFrequency    , L"Частота среза")
TITLE(Ripple             , L"Неравномерность в полосе пропускания")

TITLE(CenterFrequency, L"Центр полосы")
TITLE(WidthFrequency, L"Ширина полосы")
#undef TITLE

template<int N>struct ParamTitle<Num<CurrentFilter, N>> { wchar_t *operator()() { return (wchar_t *)L"Тип фильтра"; } };

template<int N>struct FillComboboxList<Num<CurrentFilter, N>>
{
	HWND h;
	template<class O, class P>struct loc
	{
		void operator()(P &p)
		{
			ComboBox_AddString(p.h, CbItem<O>()());
		}
	};
	void operator()(HWND h, Num<CurrentFilter, N> &t)
	{
		this->h = h;
		VL::foreach<__orders_list__<N>::Result, loc>()(*this);
	}
};	 
template<int N>struct CurrentValue<Num<CurrentFilter, N>>
{
	void operator()(HWND h, Num<CurrentFilter, N> &t)
	{
		ComboBox_SetCurSel(h, t.value);
	}
};
template<int N>struct DlgSubItems<Num<CurrentFilter, N>, int> : ComboBoxSubItem<Num<CurrentFilter, N>> {};

XDO_NOT_CHECK(CurrentFilter)

template<class P, int N>struct __command__<Dialog::NoButton<Num<CurrentFilter, N>>, P>
{
	typedef Dialog::NoButton<Num<CurrentFilter, N>> O;
	bool operator()(O &o, P &p)
	{
		if (1 == p.command.isAcselerator)
		{
			Dialog::DlgItem2<Num<CurrentFilter, N>, typename P::Owner>(&item) = p.owner.items.get<Dialog::DlgItem2<Num<CurrentFilter, N>, typename P::Owner>>();
			if (p.command.hControl == item.hWnd)
			{
				item.value.value = ComboBox_GetCurSel(item.hWnd);
				p.owner.additional->close = false;
				EndDialog(p.command.hwnd, FALSE);
				return false;
			}
		}
		return true;
	}
};

template<int N, class Dlg>struct __data_from_widget__<Dialog::DlgItem2<Num<CurrentFilter, N>, Dlg>, int>
{
	typedef int T;
	typedef Dialog::DlgItem2<Num<CurrentFilter, N>, Dlg> O;
	T operator()(O &o)
	{
		return ComboBox_GetCurSel(o.hWnd);
	}
};
////////////////////////////////////////////////////////////////////////////
DEFINE_PARAM(CurrentSensor, int, 0)
PARAM_TITLE(CurrentSensor, L"Датчик")
template<>struct FillComboboxList<CurrentSensor>
{
	void operator()(HWND h, CurrentSensor &t)
	{
		wchar_t buf[32];
		for (int i = 0; i < App::count_sensors; ++i)
		{
			ComboBox_AddString(h, _itow(1 + i, buf, 10));
		}
	}
};
template<>struct CurrentValue<CurrentSensor>
{
	void operator()(HWND h, CurrentSensor &t)
	{
		ComboBox_SetCurSel(h, t.value);
	}
};

template<>struct DlgSubItems<CurrentSensor, int> : ComboBoxSubItem<CurrentSensor> {};
DO_NOT_CHECK(CurrentSensor)
template<class Owner>struct Dialog::DlgItem2<CurrentSensor, Owner>
{
	typedef CurrentSensor T;
	static const int DY = DlgSubItems<T, typename T::type_value>::DY;
	HWND hWnd;
	T value;
	DlgItem2(Owner &o) 
	{
		value.value = o.additional->currentSensor;
	}
	void Init(HWND h, int &x, int &width, int &dy)
	{
		hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
	}
};

template<class P>struct __command__<Dialog::NoButton<CurrentSensor>, P>
{
	typedef Dialog::NoButton<CurrentSensor> O;
	bool operator()(O &o, P &p)
	{
		if (1 == p.command.isAcselerator)
		{
			Dialog::DlgItem2<CurrentSensor, typename P::Owner>(&item) = p.owner.items.get<Dialog::DlgItem2<CurrentSensor, typename P::Owner>>();
			if (p.command.hControl == item.hWnd)
			{
				item.value.value = p.owner.additional->currentSensor = ComboBox_GetCurSel(item.hWnd);
				p.owner.additional->close = false;
				EndDialog(p.command.hwnd, FALSE);
				return false;
			}
		}
		return true;
	}
};

template<class List, class Item>struct EraseItem;
template<class Item, class Head, class ...Tail>struct EraseItem<Vlst<Head, Tail...>, Item>
{
	typedef typename VL::Append<Head, typename EraseItem<Vlst<Tail...>, Item>::Result>::Result Result;
};
template<class Item, class Param, template<class, class>class Wrap, class ...Tail>struct EraseItem<Vlst<Wrap<Item, Param>, Tail...>, Item>
{
	typedef typename EraseItem<Vlst<Tail...>, Item>::Result Result;
};
template<class Item>struct EraseItem<Vlst<>, Item>
{
	typedef Vlst<> Result;
};

template<class Base>struct __ok_table_btn__<Base, FiltersTable, typename Base::multy_row_table_list>
{
	template<class T>bool operator()(HWND h, T &t)
	{
		typedef FiltersTable Table;
		typedef typename EraseItem<typename T::list, CurrentSensor>::Result list;
		if (!VL::find<list, __test__>()(t.items, h))return false;
		CBase base(Base().name());
		if (base.IsOpen())
		{
			int id = CurrentId<ID<Table> >();
			__update_data__<Table> _data(base);
			VL::foreach<list, __ok_btn__>()(t.items, _data);
			if (1 == CountId<ID<Table> >(base, id))
			{
				_data.update.Where().ID(id).Execute();
			}
			else
			{
				Insert<Base, Table, T>()(t, base);
			}
		}
		return true;
	}
};
////////////////////////////////////////////////////////////////////////////
template<class List, int N, template<class>class F>struct EraseNum;
template<int N, template<class>class F, class Head, class ...Tail>struct EraseNum<Vlst<Num<F<Head>, N>, Tail...>, N, F>
{
	typedef typename VL::Append<Num<F<Head>, N>, typename EraseNum<Vlst<Tail...>, N, F>::Result>::Result Result;
};
template<int N, template<class>class F, class Head, class ...Tail>struct EraseNum<Vlst<Head, Tail...>, N, F>
{
	typedef typename EraseNum<Vlst<Tail...>, N, F>::Result Result;
};
template<int N, template<class>class F>struct EraseNum<Vlst<>, N, F>
{
	typedef Vlst<> Result;
};

template<class O, class P>struct __current_filtre_param__;

template<template<class>class X, template<class>class Y, class O, int N, class P>struct __current_filtre_param__<Num<X<Y<O>>, N>, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<typename __orders_list__<N>::Result, Num<X<Y<O>>, N>>::value == p.obj.items.get<Num<CurrentFilter, N>>().value)
		{
			typedef typename VL::Append<typename __filtr__<X, Y, FiltersTable::items_list, N>::Result, Num<CurrentFilter, N>>::Result _list;
			typedef VL::Append<CurrentSensor, _list>::Result list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Настройки аналогового фильтра. Датчик %d", 1 + N);
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<OkBtn, CancelBtn, Dialog::NoButton<Num<CurrentFilter, N>>, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, buf))
			{
				typedef typename EraseNum<FiltersTable::items_list, N, X >::Result _list;
				typedef VL::Append< Num<CurrentFilter, N>, _list>::Result list;
				VL::CopyFromTo<list>(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<int N, class P>struct __current_filtre_param__<Num<CurrentFilter, N>, P>
{
	typedef Num<CurrentFilter, N> O;
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__<N>::Result, O>::value == p.obj.items.get<O>().value)
		{
			typedef VL::Append<CurrentSensor, O>::Result list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Настройки аналогового фильтра. Датчик %d", 1 + N);
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<OkBtn, CancelBtn, Dialog::NoButton<O>, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, buf))
			{
				VL::CopyFromTo<Vlst<Num<CurrentFilter, N>>>(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<class O, class P>struct __curr_sens__
{
	bool operator()(P &p)
	{
		if (O::value == p.currentSensor)
		{
			VL::find<__orders_list__<O::value>::Result, __current_filtre_param__>()(p);
			return false;
		}
		return true;
	}
};

void DspFiltrDlg::Do(HWND h)
{
	static int currentSensor = 0;
	__current_filtre_param_data__<FiltersTable> data = {
		Singleton<FiltersTable>::Instance()
		, h
		, false
		, &Singleton<FiltersTable>::Instance().items
		, currentSensor
	};
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens__>()(data);
	currentSensor = data.currentSensor;
}
///////////////////////////////////////////////////////////
template<class O, class P>struct __Xcurrent_filtre_param__;

template<template<class>class X, template<class>class Y, class O, int N, class P>struct __Xcurrent_filtre_param__<Num<X<Y<O>>, N>, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<typename __orders_list__<N>::Result, Num<X<Y<O>>, N>>::value == p.obj.items.get<Num<CurrentFilter, N>>().value)
		{
			typedef typename VL::Append<typename __filtr__<X, Y, FiltersTable::items_list, N>::Result, Num<CurrentFilter, N>>::Result _list;
			typedef VL::Append<CurrentSensor, _list>::Result list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Настройки аналогового фильтра. Датчик %d", 1 + N);
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<NoStoreOkBtn, CancelBtn, Dialog::NoButton<Num<CurrentFilter, N>>, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, buf))
			{
				typedef typename EraseNum<FiltersTable::items_list, N, X >::Result _list;
				typedef VL::Append< Num<CurrentFilter, N>, _list>::Result list;
				VL::CopyFromTo<list>(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<int N, class P>struct __Xcurrent_filtre_param__<Num<CurrentFilter, N>, P>
{
	typedef Num<CurrentFilter, N> O;
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__<N>::Result, O>::value == p.obj.items.get<O>().value)
		{
			typedef VL::Append<CurrentSensor, O>::Result list;
			p.close = true;
			wchar_t buf[128];
			wsprintf(buf, L"Настройки аналогового фильтра. Датчик %d", 1 + N);
			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<NoStoreOkBtn, CancelBtn, Dialog::NoButton<O>, Dialog::NoButton<CurrentSensor>>
				, __current_filtre_param_data__<FiltersTable>
			>(p.obj, &p).Do(p.h, buf))
			{
				VL::CopyFromTo<Vlst<Num<CurrentFilter, N>>>(p.obj.items, *p.pitems);
			}
			return false;
		}
		return true;
	}
};

template<class O, class P>struct __curr_sens_XX__
{
	bool operator()(P &p)
	{
		if (O::value == p.currentSensor)
		{
			VL::find<__orders_list__<O::value>::Result, __Xcurrent_filtre_param__>()(p);
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
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens_XX__>()(data);
	w->computeFrame.UpdateFiltre();
	RepaintWindow(w->hWnd);
}

void TstDspFiltrDlg::Do(HWND h)
{
	FiltersTable table;
	
	ZonesWindow *w = (ZonesWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	VL::CopyFromTo(w->computeFrame.paramFlt, table.items);
	__current_filtre_param_data__<FiltersTable> data = {
		table
		, h
		, false
		, &w->computeFrame.paramFlt
	};
	while (!data.close) VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __curr_sens_XX__>()(data);
	w->computeFrame.UpdateFiltre();
	RepaintWindow(w->hWnd);
}

#undef XMIN_EQUAL_VALUE
#undef XMAX_EQUAL_VALUE
#undef XDO_NOT_CHECK

