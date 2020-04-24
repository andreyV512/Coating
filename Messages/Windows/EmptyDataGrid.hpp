#pragma once
#include "Grid/GridWindow.h"
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

template<class List>class EmptyDataGrid: public TEvent
{
	typedef List cols_list;
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
		//	p[VL::IndexOf<cols_list, O>::value] = __to_str__<typename Item::TItems, O, typename O::type_value>::Do;
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
	HWND hWnd;
	EmptyDataGrid(): countItems(0)
	{
		//VL::foreach<cols_list, __set_to_str__>()(proc_proc);
	}
	void Create(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
};

//template<class T, class D>struct __insert__
//{
//	bool operator()(int id, T &t, D &d)
//	{
//		d.push_back({ id, t.items });
//		return false;
//	}
//};

template<class List>
inline void EmptyDataGrid<List>::Create(HWND hwnd)
{
	hWnd = GridDetail::CreateGrid(hwnd, this);
	typename GridDetail::SetGridHeader<cols_list> _(hWnd);
	GridDetail::SetRow(hWnd, dimention_of(dataItems));
}

/*
struct TCellColor
{
	static const unsigned mess = NM_CUSTOMDRAW;
	DWORD_PTR col, row;
	COLORREF &textColor, &bkColor;
	TCellColor(TNotify &m)
		: col(((NMLVCUSTOMDRAW *)m.pnmh)->iSubItem)
		, row(((NMLVCUSTOMDRAW *)m.pnmh)->nmcd.dwItemSpec)
		, textColor(((NMLVCUSTOMDRAW *)m.pnmh)->clrText)
		, bkColor(((NMLVCUSTOMDRAW *)m.pnmh)->clrTextBk)
	{}
};

struct TCellData
{
	static const unsigned mess = LVN_GETDISPINFO;
	int col, row;
	HWND h;
	wchar_t data[128];
	TCellData(TNotify &m)
		: col(((LV_DISPINFO *)m.pnmh)->item.iSubItem)
		, row(((LV_DISPINFO *)m.pnmh)->item.iItem)
		, h(m.hwnd)
	{
		data[0] = 0;
	}
};
*/

template<class List>
inline void EmptyDataGrid<List>::SetDataToGrid(LV_DISPINFO *d)
{
	//int row = d->item.iItem;
	//if (row < (int)dataItems.size())
	//{
	//	Item &item = dataItems[d->item.iItem];
	//	const int col = d->item.iSubItem;
	//	if (col > 0 && col < 1 + VL::Length<Table::items_list>::value)
	//	{
	//		wchar_t buf[32] = {};
	//		wchar_t *s = (*proc_proc[col - 1])(dataItems[row].items, buf);
	//		StringCchCopy(d->item.pszText, 1 + wcslen(s), s);
	//	}
	//	else if (0 == col)
	//	{
	//		wchar_t buf[32] = {};
	//		_itow_s(1 + row, buf, 10);
	//		StringCchCopy(d->item.pszText, 1 + wcslen(buf), buf);
	//	}
	//}
}

template<class List>
inline void EmptyDataGrid<List>::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (d->nmcd.dwItemSpec < (int)dataItems.size())
	{
		if (0 == ((d->nmcd.dwItemSpec) % 2))
		{
			d->clrTextBk = RGB(0xE7, 0xE7, 0xD1);
		}
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
