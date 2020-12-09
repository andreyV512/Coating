#pragma once

template<class T>struct __current_filtre_param_data__
{
	T table;
	HWND h;
	bool close;
	typename T::TItems &items;
	int &currentSensor;
	__current_filtre_param_data__(HWND h, typename T::TItems &items, int &currentSensor)
		: h(h)
		, close(false)
		, items(items)
		, currentSensor(currentSensor)
	{
		VL::CopyFromTo(items, table.items);
	}
	template<class List>void Restore()
	{
		VL::CopyFromTo(table.items, items);
	}
};

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

template<class Base, class Table, class Sub>struct _tpl_ok_table_btn__
{
	template<class T>bool operator()(HWND h, T &t)
	{
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

#define XMIN_EQUAL_VALUE(sub_type, value)\
template<>struct LessEqual<Num<sub_type, 0>>{ Num<sub_type, 0>::type_value operator()(){return value;}};\
template<>struct LessEqual<Num<sub_type, 1>>{ Num<sub_type, 1>::type_value operator()(){ return value;}};\
template<>struct LessEqual<Num<sub_type, 2>>{ Num<sub_type, 2>::type_value operator()(){ return value;}};

#define XMAX_EQUAL_VALUE(sub_type, value)\
template<>struct LargenEqual<Num<sub_type, 0>>{Num<sub_type, 0>::type_value operator()(){return value;}};\
template<>struct LargenEqual<Num<sub_type, 1>>{Num<sub_type, 1>::type_value operator()(){return value;}};\
template<>struct LargenEqual<Num<sub_type, 2>>{Num<sub_type, 2>::type_value operator()(){return value;}};

#define XMIN_VALUE(sub_type, value)\
template<>struct Less<Num<sub_type, 0>>{Num<sub_type, 0>::type_value operator()(){return value;}};\
template<>struct Less<Num<sub_type, 1>>{Num<sub_type, 1>::type_value operator()(){return value;}};\
template<>struct Less<Num<sub_type, 2>>{Num<sub_type, 2>::type_value operator()(){return value;}};

#define XPARAM_TITLE(type, name)\
template<>struct ParamTitle<Num<type, 0>>{wchar_t *operator()(){return (wchar_t *)name;}};\
template<>struct ParamTitle<Num<type, 1>> { wchar_t *operator()() { return (wchar_t *)name; } };\
template<>struct ParamTitle<Num<type, 2>> { wchar_t *operator()() { return (wchar_t *)name; } };

#define XDO_NOT_CHECK(n)template<int N>struct __compare_param__<Vlst<>, Vlst<>, Num<n, N>>\
{bool operator()(typename Num<n, N>::type_value &t){return true;}};

template<class List, int N>struct GB;
template<int N, class Head, class ...Tail>struct GB<Vlst<Head, Tail...>, N>
{
	typedef typename VL::Append<Num<Head, N>, typename GB<Vlst<Tail...>, N>::Result>::Result Result;
};
template<int N>struct GB<Vlst<>, N>
{
	typedef Vlst<> Result;
};