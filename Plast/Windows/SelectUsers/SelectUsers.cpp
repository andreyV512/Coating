#include "SelectUsers.h"
#include "templates/typelist.hpp"
#include "Grid/GridDetail.h"
#include "tools_debug/DebugMess.h"
#include "window_tool/MenuAPI.h"
#include <strsafe.h>
#include "SelectUserDlg.h"
#include "App/AppBase.h"
#include "window_tool/Pass.h"

#define Z(T, w, txt)template<>struct GridDetail::header_table<T>\
	{\
	LPWSTR name(){return (LPWSTR)txt;}\
	static const int width = w;\
	};

struct Num {};
struct Name {};
struct TabelNum {};

Z(Num, 60, L"Номер")
Z(Name, 90, L"Имя")
Z(TabelNum, 120, L"Табельный номер")
#undef Z

typedef Vlst<
	Num
	, Name
	, TabelNum
> ParameterNameList;

SelectUsers::SelectUsers()
	: hWnd(0)
	, okButton(*this)
{}

template<class T, class D>struct __insert__
{
	bool operator()(int id, T &t, D &d)
	{
		d.push_back({id, (wchar_t *)t.items.get<UserName>().value, t.items.get<UserPersonnelNumber>().value});
		return false;
	}
};

void SelectUsers::Create(HWND hwnd)
{
	hWnd = GridDetail::CreateGrid(hwnd, this);
	GridDetail::SetGridHeader<ParameterNameList>_(hWnd);
	GridDetail::SetRow(hWnd, 128);

	CBase base((wchar_t *)ParametersBase().name());
	if (base.IsOpen())
	{
		Select<UserTable>_(base);
		wcscpy(&_.head[wcslen(_.head) - 7], L" ORDER BY UserName ASC       ");
		_.ExecuteLoop<__insert__>(items);
	}
}

void SelectUsers::Size(int x, int y, int width, int heigth)
{
	MoveWindow(hWnd, x, y, width, heigth, TRUE);
}

LRESULT SelectUsers::Do(TNotify &l)
{
	return  GridDetail::Do(this, l);
}
//---------------------------------------------------------------------------
namespace __pupup_menu_su__
{
	struct Add {};
	struct Del {};
	template<>struct TopMenu<Add> { typedef Vlst<> list; };
	template<>struct TopMenu<Del> { typedef Vlst<> list; };
	typedef Vlst<TopMenu<Add>, TopMenu<Del> > __menu_list__;
	MENU_TEXT(L"Добавить", TopMenu<Add>)
		MENU_TEXT(L"Удалить", TopMenu<Del>)

		template<>struct Event<TopMenu<Add>>
	{
		static void Do(LPNMITEMACTIVATE d)
		{
			UserTable t;
			if (SelectUserDlg::Do(d->hdr.hwndFrom, t))
			{
				SelectUsers *o = (SelectUsers *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
				o->items.push_back({ 
					-1
					, (wchar_t *)t.items.get<UserName>().value
					, t.items.get<UserPersonnelNumber>().value
					});
				GridDetail::UpdateGridCells(d->hdr.hwndFrom);
			}
		}
	};

	template<>struct Event<TopMenu<Del>>
	{
		static void Do(LPNMITEMACTIVATE d)
		{
			SelectUsers *o = (SelectUsers *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
			if ((int )o->items.size() > d->iItem)
			{
				int i = d->iItem;
				o->delItems.push_back(o->items[i]);
				o->items.erase(o->items.begin() + i);
				GridDetail::UpdateGridCells(d->hdr.hwndFrom);
			}
		}
	};
}
//---------------------------------------------------------------------------
void SelectUsers::RClick(LPNMITEMACTIVATE d)
{
	PopupMenu<__pupup_menu_su__::__menu_list__>::Do(d->hdr.hwndFrom, d);
}
#pragma warning(disable: 6054)
void SelectUsers::SetDataToGrid(LV_DISPINFO *d)
{
	if (d->item.iItem < (int)items.size())
	{
		Item &item = items[d->item.iItem];
		switch (d->item.iSubItem)
		{
		case 0:
		{
			wchar_t buf[32] = {};
			_itow_s(1 + d->item.iItem, buf, 10);
			StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
		}
		break;
		case 1:
			StringCchCopy(d->item.pszText, 1 + item.name.length(), item.name.c_str());
			break;
		case 2:
		{
			wchar_t buf[32];
			_itow_s(item.tabelNum, buf, 10);
			StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
		}
		break;
		}
	}
}

void SelectUsers::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (d->nmcd.dwItemSpec < (int)items.size())
	{
		if (0 == ((d->nmcd.dwItemSpec) % 2))
		{
			d->clrTextBk = RGB(0xE7, 0xE7, 0xD1);
		}
	}
}

SelectUsers::OkButton::OkButton(SelectUsers &o) : owner(o) {}

void SelectUsers::OkButton::Do(TCommand &l)
{
	if (TypesizePasswordDlg().Do(l.hwnd))
	{
		CBase base((wchar_t *)ParametersBase().name());
		if (base.IsOpen())
		{
			for (auto i : owner.delItems)
			{
				if (-1 != i.id)
				{
					try
					{
						Delete<UserTable>(base).ID(i.id).Execute();
					}
					catch (...) {}
				}
			}
			
			for (auto i : owner.items)
			{
				if (-1 == i.id)
				{
					UserTable t;
					t.items.get<UserName>().value = (wchar_t *)i.name.c_str();
					t.items.get<UserPersonnelNumber>().value = i.tabelNum;
					try
					{
						Insert_Into<UserTable>(t, base).Execute();
					}
					catch (...) 					
					{
					}
				}
			}
		}
		DestroyWindow(l.hwnd);
	}
}
