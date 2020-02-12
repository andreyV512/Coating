#include "Dlg.h"
#include "DspFiltrParams.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "tools_debug/DebugMess.h"

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

typedef __orders__<FiltersTable::items_list>::Result __orders_list__;

template<class T>struct __current_filtre_param_data__
{
	T obj;
	HWND h;
};

MIN_EQUAL_VALUE(Low<ChebyshevI<Order>>, 1)
MAX_EQUAL_VALUE(Low<ChebyshevI<Order>>, 10)
DO_NOT_CHECK(Low<ChebyshevI<CutoffFrequency>>)
DO_NOT_CHECK(Low<ChebyshevI<Ripple>>)

MIN_EQUAL_VALUE(High<ChebyshevI<Order>>, 1)
MAX_EQUAL_VALUE(High<ChebyshevI<Order>>, 10)
DO_NOT_CHECK(High<ChebyshevI<CutoffFrequency>>)
DO_NOT_CHECK(High<ChebyshevI<Ripple>>)

template<class T>struct CbItem;
template<>struct CbItem<Low<ChebyshevI<Order>>>
{
	wchar_t* operator()() { return (wchar_t *)L"Низкочастотный фильтр"; }
};
template<>struct CbItem<High<ChebyshevI<Order>>>
{
	wchar_t* operator()() { return (wchar_t*)L"Высокочастотный фильтр"; }
};

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
		VL::for_each<__orders_list__, loc>()(*this);
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

template<class T>struct Cb {};

template<class P>struct __command__<Cb<CurrentFilter>, P>
{
	typedef Cb<CurrentFilter> O;
	bool operator()(O *o, P *p)
	{
		if (1 == p->e.isAcselerator)
		{
			//HWND h = p->owner.items.get<Dialog::DlgItem2<DefectSig<TypeFiltre>, P::Owner>>().hWnd;
			////
			//if (p->e.hControl == h)
			//{
			//	int t = ComboBox_GetCurSel(h);
			//	dprint("%s %d\n", typeid(O).name(), t);
			//	if (p->owner.additionalData->id != t)
			//	{
			//		p->owner.additionalData->current = t;
			//		p->owner.additionalData->changed = true;
			//		EndDialog(p->e.hwnd, FALSE);
			//	}
			//	return false;
			//}
		}
		return true;
	}
};



template<class O, class P>struct __current_filtre_param__;

template<class From, class To>struct __copy__
{
	void operator()(From &o, To &p)
	{
		p.get<From>().value = o.value;
	}
};

template<template<class>class X, template<class>class Y, class O, class P>struct __current_filtre_param__<X<Y<O>>, P>
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<__orders_list__, X<Y<O>>>::value == p.obj.items.get<CurrentFilter>().value)
		{
			typedef typename VL::Append<typename __filtr__<X, Y, FiltersTable::items_list>::Result, CurrentFilter>::Result list;

			if (Dialog::Templ<ParametersBase, FiltersTable
				, list
				, 550
				, Vlst<OkBtn, CancelBtn, Cb<CurrentFilter>>
			>(p.obj).Do(p.h, (wchar_t *)L"Фильтр"))
			{
				VL::for_each<list, __copy__>()(p.obj.items, Singleton<FiltersTable>::Instance().items);
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
	};
	VL::find<__orders_list__, __current_filtre_param__>()(data);
}