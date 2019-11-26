#pragma once
#include "GridWindow.h"
#include <vector>
#include <strsafe.h>
#include "Grid/GridDetail.h"
#include "templates/templates.hpp"

struct Num {};
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
	struct Item
	{
		int id;
		typename Table::TItems items;
	};
	std::vector<Item> items;
	std::vector<Item> delItems;
	static const int proc_size = VL::Length<typename Table::items_list>::value;
	wchar_t *(*proc[proc_size])(typename Table::TItems &, wchar_t *);
public:
	HWND hWnd;
	TDataGrid() : DataGrid() 
	{
		VL::for_each<typename Table::items_list, __set_to_str__>()(proc);
	}
	void Create(HWND) override;
	virtual void RClick(LPNMITEMACTIVATE)override;
	virtual void SetDataToGrid(LV_DISPINFO *)override;
	virtual void SetColorToGrid(NMLVCUSTOMDRAW *)override;
	virtual void ButtonClick()override;
};

template<class T, class D>struct __insert__
{
	bool operator()(int id, T &t, D &d)
	{
		d.push_back({ id, t.items});
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
		_.ExecuteLoop<__insert__>(items);
	}
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::RClick(LPNMITEMACTIVATE d)
{
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::SetDataToGrid(LV_DISPINFO *d)
{
	//if (d->item.iItem < (int)items.size())
	//{
	//	Item &item = items[d->item.iItem];
	//	switch (d->item.iSubItem)
	//	{
	//	case 0:
	//	{
	//		wchar_t buf[32] = {};
	//		_itow_s(1 + d->item.iItem, buf, 10);
	//		StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
	//	}
	//	break;
	//	case 1:
	//		StringCchCopy(d->item.pszText, 1 + item.name.length(), item.name.c_str());
	//		break;
	//	case 2:
	//	{
	//		wchar_t buf[32];
	//		_itow_s(item.tabelNum, buf, 10);
	//		StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
	//	}
	//	break;
	//	}
	//}
	if (d->item.iItem < (int)items.size())
	{
		Item &item = items[d->item.iItem];
		const int col = d->item.iSubItem;
		if (col > 0 && col < 1 + VL::Length<Table::items_list>::value)
		{

		}
		else if(0 == col)
		{
			wchar_t buf[32] = {};
			_itow_s(1 + d->item.iItem, buf, 10);
			StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
		}
	}
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (d->nmcd.dwItemSpec < (int)items.size())
	{
		if (0 == ((d->nmcd.dwItemSpec) % 2))
		{
			d->clrTextBk = RGB(0xE7, 0xE7, 0xD1);
		}
	}
}

template<class Table, class OrderBy>
inline void TDataGrid<Table, OrderBy>::ButtonClick()
{
}
