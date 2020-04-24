#pragma once
#include "GridWindow.h"
#include <vector>
#include <strsafe.h>
#include "Grid/GridDetail.h"
#include "window_tool/MenuAPI.h"
#include "DlgTemplates/ParamDlgNew.h"

#define HEADER_TABLE(T, w, txt)template<>struct GridDetail::header_table<T>\
	{\
	LPWSTR name(){return (LPWSTR)txt;}\
	static const int width = w;\
	};

#define PARAM_TABLE(T, w, txt)\
PARAM_TITLE(T, txt) \
HEADER_TABLE(T, w, txt)	 

template<class Table, class OrderBy, class ViewCols = Vlst<>>class TDataGrid : public DataGrid
{
	typedef typename VL::_if<VL::Length<ViewCols>::value, ViewCols, typename Table::items_list>::Result cols_list;
	template<class Items, class Type, class TypeValue>struct __to_str__;
	template<class Items, class Type>struct __to_str__<Items, Type, int>
	{
		static wchar_t *Do(Items &item, wchar_t *buf)
		{
			typename Type::type_value &val = item.get<Type>().value;
			_itow(val, buf, 10);
			return buf;
		}
	};
	template<class Items, class Type, int N>struct __to_str__<Items, Type, Holder<N> >
	{
		static wchar_t *Do(Items &item, wchar_t *buf)
		{
			return item.get<Type>().value.buffer;
		}
	};
	template<class O, class P>struct __set_to_str__
	{
		void operator()(P &p)
		{
			p[VL::IndexOf<cols_list, O>::value] = __to_str__<typename Item::TItems, O, typename O::type_value>::Do;
		}
	};
	template<class O, class P>struct __copy__
	{
		void operator()(O &o, P &p)
		{
			o.value = ((O *)&p)->value;
		}
	};
	struct Item
	{
		int id;
		typedef typename VL::Factory<cols_list> TItems;
		TItems items;
		Item &operator=(const Item &t)
		{
			id = t.id;
			VL::foreach<cols_list, __copy__>()(items, t.items);
			return *this;
		}
	};
	int countItems;
	
	static const int proc_size = VL::Length<cols_list>::value;
	wchar_t *(*proc_proc[proc_size])(typename Item::TItems &, wchar_t *);
	Item dataItems[1024];
public:
	TDataGrid() : DataGrid(), countItems(0)
	{
		//VL::foreach<cols_list, __set_to_str__>()(proc_proc);
	}
	void Create(HWND) override;
	void RClick(LPNMITEMACTIVATE)override;
	void SetDataToGrid(LV_DISPINFO *)override;
	void SetColorToGrid(NMLVCUSTOMDRAW *)override;
	void ButtonClick(HWND)override;
	void DeleteItem(int i, HWND h);
	void AddItem(HWND h);
	void SelectItem(int i, HWND h);
};

template<class T, class D>struct __insert__
{
	bool operator()(int id, T &t, D &d)
	{
		d.push_back({ id, t.items });
		return false;
	}
};

template<class List>class __order_by__
{
	template<class O, class P>struct loc
	{
		void operator()(P &p)
		{
			wsprintf(p, L" %s,", O().name());
			p += wcslen(p);
		}
	};
public: 
	void operator()(wchar_t *s)
	{
		wcscat(s, L" ORDER BY ");
		s += wcslen(s);
		VL::foreach<List, loc>()(s);
		wcscat(&s[wcslen(s)], L"      ");
	}
};
template<>class __order_by__<Vlst<>>
{
public:
	void operator()(wchar_t *s) {}
};

template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::Create(HWND hwnd)
{
	hWnd = GridDetail::CreateGrid(hwnd, this);
	typedef typename VL::Append<
		Num
		, typename VL::_if<VL::Length<ViewCols>::value, ViewCols, typename Table::items_list>::Result
	>::Result  list;
	typename GridDetail::SetGridHeader<list> _(hWnd);
	GridDetail::SetRow(hWnd, 128);

	CBase base((wchar_t *)ParametersBase().name());
	if (base.IsOpen())
	{
		Select<Table>_(base);
		wchar_t *s = &_.head[wcslen(_.head) - 7];
		*s = L'\0';
		__order_by__<OrderBy>()(s);
		_.ExecuteLoop<__insert__, cols_list>(dataItems);
		countItems = (int)dataItems.size();
	}
}

template<class O, class P>struct __ok_btn_data_grid__
{
	void operator()(O &o)
	{
        o.value.value =  __data_from_widget__<O, typename VL::Inner<O>::Result::type_value>()(o);
	}
};

struct sel_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title() { return (wchar_t *)L"Применить"; }
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if (VL::find<Owner::list, __test__>()(owner.items, h))
		{
			VL::foreach<Owner::list, __ok_btn_data_grid__>()(owner.items);
			EndDialog(h, TRUE);
		}
	}
};

struct Add {};
struct Del {};
struct Sel {};
template<class, class >class Wrap {};
template<class T>struct TopMenu<Wrap<Add, T>> { typedef Vlst<> list; };
template<class T>struct TopMenu<Wrap<Del, T>> { typedef Vlst<> list; };
template<class T>struct TopMenu<Wrap<Sel, T>> { typedef Vlst<> list; };

template<class T>struct NameMenu<TopMenu<Wrap<Add, T> > >
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Добавить"; }
};
template<class T>struct NameMenu<TopMenu<Wrap<Del, T> > >
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Удалить"; }
};
template<class T>struct NameMenu<TopMenu<Wrap<Sel, T> > >
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Выбрать"; }
};

template<class T>struct Event<TopMenu<Wrap<Add, T>>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		T *o = (T *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		o->AddItem(d->hdr.hwndFrom);
	}
};

template<class T>struct Event<TopMenu<Wrap<Del, T>>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		T *o = (T *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		o->DeleteItem(d->iItem, d->hdr.hwndFrom);
	}
};

template<class T>struct Event<TopMenu<Wrap<Sel, T>>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		T *o = (T *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		o->SelectItem(d->iItem, d->hdr.hwndFrom);
	}
};

template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::RClick(LPNMITEMACTIVATE d)
{
	PopupMenu<Vlst<TopMenu<Wrap<Sel, TDataGrid>>, Separator<0>, TopMenu<Wrap<Add, TDataGrid>>, TopMenu<Wrap<Del, TDataGrid>>>>::Do(d->hdr.hwndFrom, d);
}

template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::SetDataToGrid(LV_DISPINFO *d)
{
	int row = d->item.iItem;
	if (row < (int)dataItems.size())
	{
		Item &item = dataItems[d->item.iItem];
		const int col = d->item.iSubItem;
		if (col > 0 && col < 1 + VL::Length<Table::items_list>::value)
		{
			wchar_t buf[32] = {};
			wchar_t *s = (*proc_proc[col - 1])(dataItems[row].items, buf);
			StringCchCopy(d->item.pszText, 1 + wcslen(s), s);
		}
		else if (0 == col)
		{
			wchar_t buf[32] = {};
			_itow_s(1 + row, buf, 10);
			StringCchCopy(d->item.pszText, 1 + wcslen(buf), buf);
		}
	}
}

template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (d->nmcd.dwItemSpec < (int)dataItems.size())
	{
		if (0 == ((d->nmcd.dwItemSpec) % 2))
		{
			d->clrTextBk = RGB(0xE7, 0xE7, 0xD1);
		}
	}
}



template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::DeleteItem(int i, HWND h)
{
	if ((int)dataItems.size() > i)
	{
		delItems.push_back(dataItems[i].id);
		dataItems.erase(dataItems.begin() + i);
		GridDetail::UpdateGridCells(h);
	}
}

template<class Table, class OrderBy, class ViewCols>
inline void TDataGrid<Table, OrderBy, ViewCols>::AddItem(HWND h)
{
	Table t;
	wchar_t buf[128];
	GetWindowText(GetParent(h), buf, dimention_of(buf));
	if (Dialog::Templ<
		ParametersBase
		, Table
		, cols_list
		, 350
		, Vlst<sel_OkBtn, CancelBtn>
	>(t).Do(h, buf))
	{
		dataItems.push_back({
			-1
			, t.items
			});
		GridDetail::UpdateGridCells(h);
	}
}



//Пример использования сетка с окном добавления и удаления и сохранения базы данных
//HEADER_TABLE(Num, 60, L"Номер")  //заголовок столбца, ширина столбца (отрисовывает номер строки)
//PARAM_TABLE(UserName, 90, L"Оператор") //заголовок столбца, ширина столбца(столбец из базы данных)
//PARAM_TABLE(UserPersonnelNumber, 120, L"Табельный номер")	 //заголовок столбца, ширина столбца(столбец из базы данных)
//
//CHECK_EMPTY_STRING(UserName)
//MIN_VALUE(UserPersonnelNumber, 0)
//MAX_VALUE(UserPersonnelNumber, 9999999)
//Сохраняет в таблицу UserTable
//Сортирует список по полю	   UserName
//
//void TestTest()
//{
//	GridOptions<TDataGrid<UserTable, UserName> >::Open((wchar_t *)L"Настройки оператора", 320, 300);
//}
