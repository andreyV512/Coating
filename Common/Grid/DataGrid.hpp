#pragma once
#include "GridWindow.h"
#include <vector>
#include <strsafe.h>
#include "Grid/GridDetail.h"
#include "window_tool/MenuAPI.h"
#include "DlgTemplates/ParamDlgNew.h"

struct Num {};

#define HEADER_TABLE(T, w, txt)template<>struct GridDetail::header_table<T>\
	{\
	LPWSTR name(){return (LPWSTR)txt;}\
	static const int width = w;\
	};

#define PARAM_TABLE(T, w, txt)\
PARAM_TITLE(T, txt) \
HEADER_TABLE(T, w, txt)	 

template<class Table, class OrderBy>class TDataGrid : public DataGrid
{
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
			p[VL::IndexOf<typename Table::items_list, O>::value] = __to_str__<typename Table::TItems, O, typename O::type_value>::Do;
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
		typename VL::Factory<typename Table::items_list> items;
		Item &operator=(const Item &t)
		{
			id = t.id;
			VL::for_each<typename Table::items_list, __copy__>()(items, t.items);
			return *this;
		}
	};
	int countItems;
	
	static const int proc_size = VL::Length<typename Table::items_list>::value;
	wchar_t *(*proc_proc[proc_size])(typename Table::TItems &, wchar_t *);
	std::vector<Item> dataItems;
	std::vector<int> delItems;
public:
	TDataGrid() : DataGrid(), countItems(0)
	{
		VL::for_each<typename Table::items_list, __set_to_str__>()(proc_proc);
	}
	void Create(HWND) override;
	void RClick(LPNMITEMACTIVATE)override;
	void SetDataToGrid(LV_DISPINFO *)override;
	void SetColorToGrid(NMLVCUSTOMDRAW *)override;
	void ButtonClick(HWND)override;
	void DeleteItem(int i, HWND h);
	void AddItem(HWND h);
};

template<class T, class D>struct __insert__
{
	bool operator()(int id, T &t, D &d)
	{
		d.push_back({ id, t.items });
		return false;
	}
};

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::Create(HWND hwnd)
{
	hWnd = GridDetail::CreateGrid(hwnd, this);
	typedef typename VL::Append<
		Num
		, typename Table::items_list
	>::Result  list;
	typename GridDetail::SetGridHeader<list> _(hWnd);
	GridDetail::SetRow(hWnd, 128);

	CBase base((wchar_t *)ParametersBase().name());
	if (base.IsOpen())
	{
		Select<Table>_(base);
		wsprintf(&_.head[wcslen(_.head) - 7], L" ORDER BY %s ASC       ", OrderBy().name());
		_.ExecuteLoop<__insert__>(dataItems);
		countItems = (int)dataItems.size();
	}
}
template<class, class, class>struct __ok_table_btn__;

struct sel_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title() { return (wchar_t *)L"Применить"; }
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if (__ok_table_btn__<
			Owner::Base, Owner::Table
			, typename VL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
		>x; x(h, owner))
		{
			EndDialog(h, TRUE);
		}
	}
};

struct Add {};
struct Del {};
template<class, class >class Wapper {};
template<class T>struct TopMenu<Wapper<Add, T>> { typedef Vlst<> list; };
template<class T>struct TopMenu<Wapper<Del, T>> { typedef Vlst<> list; };

template<class T>struct NameMenu<TopMenu<Wapper<Add, T> > >
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Добавить"; }
};
template<class T>struct NameMenu<TopMenu<Wapper<Del, T> > >
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Удалить"; }
};

template<class T>struct Event<TopMenu<Wapper<Add, T>>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		T *o = (T *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		o->AddItem(d->hdr.hwndFrom);
	}
};

template<class T>struct Event<TopMenu<Wapper<Del, T>>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		T *o = (T *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		o->DeleteItem(d->iItem, d->hdr.hwndFrom);
	}
};

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::RClick(LPNMITEMACTIVATE d)
{
	PopupMenu<Vlst<TopMenu<Wapper<Add, TDataGrid>>, TopMenu<Wapper<Del, TDataGrid>>>>::Do(d->hdr.hwndFrom, d);
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::SetDataToGrid(LV_DISPINFO *d)
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

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (d->nmcd.dwItemSpec < (int)dataItems.size())
	{
		if (0 == ((d->nmcd.dwItemSpec) % 2))
		{
			d->clrTextBk = RGB(0xE7, 0xE7, 0xD1);
		}
	}
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::ButtonClick(HWND h)
{
	if ((0 != delItems.size() || countItems != dataItems.size()) && TypesizePasswordDlg().Do(h))
	{
		CBase base((wchar_t *)ParametersBase().name());
		if (base.IsOpen())
		{
			for (auto i : delItems)
			{
				if (-1 != i)
				{
					try
					{
						Delete<Table>(base).ID(i).Execute();
					}
					catch (...) {}
				}
			}

			for (auto i : dataItems)
			{
				if (-1 == i.id)
				{
					try
					{
						Insert_Into<Table>(*(Table *)&i.items, base).Execute();
					}
					catch (...) {}
				}
			}
		}
	}
	DestroyWindow(h);
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::DeleteItem(int i, HWND h)
{
	if ((int)dataItems.size() > i)
	{
		delItems.push_back(dataItems[i].id);
		dataItems.erase(dataItems.begin() + i);
		GridDetail::UpdateGridCells(h);
	}
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::AddItem(HWND h)
{
	Table t;
	wchar_t buf[128];
	GetWindowText(GetParent(h), buf, dimention_of(buf));
	if (Dialog::Templ<
		ParametersBase
		, Table
		, typename Table::items_list
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
//HEADER_TABLE(Num, 60, L"Номер")  //заголовок столбца, ширина столбца
//PARAM_TABLE(UserName, 90, L"Оператор") //заголовок столбца, ширина столбца(столбец из базы данных)
//PARAM_TABLE(UserPersonnelNumber, 120, L"Табельный номер")
//
//CHECK_EMPTY_STRING(UserName)
//MIN_VALUE(UserPersonnelNumber, 0)
//MAX_VALUE(UserPersonnelNumber, 9999999)
//
//void TestTest()
//{
//	GridOptions<TDataGrid<UserTable, UserName> >::Open((wchar_t *)L"Настройки оператора", 320, 300);
//}
