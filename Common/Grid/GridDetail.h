#pragma once
#include <windows.h>
#include <commctrl.h>
#include "window_tool/message.h"
#include "templates/typelist.hpp"

namespace GridDetail
{
	template<class T>struct header_table;
	template<class T>class SetGridHeader
	{
		HWND hWnd;
		LV_COLUMN lvc;
		template<class O, class P>struct loc
		{
			void operator()(P &p)
			{
				p.lvc.iSubItem = VL::IndexOf<T, O>::value;
				p.lvc.pszText = header_table<O>().name();
				p.lvc.cx = header_table<O>::width;
				ListView_InsertColumn(p.hWnd, p.lvc.iSubItem, &p.lvc);
			}
		};
	public:
		explicit SetGridHeader(HWND hWnd) : hWnd(hWnd)
		{
			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.fmt = LVCFMT_LEFT;
			VL::foreach<T, loc>()(*this);
		}
	};

	void SetRow(HWND hWnd, int count);
	void UpdateGridCells(HWND h);

	HWND CreateGrid(HWND, void *);

#define NANDLER_PROC(name)class name\
{\
	template<class T, T>struct Helper{};\
	template<class T, class P>void Xz(T *t, P *p, Helper<void(T::*)(P *), &T::name> * = NULL){t->name(p);}\
	template<class T, class P>void Xz(...){};\
public:\
	template<class T, class P>void operator()(T *t, P *p){ Xz<T, P>(t, p);};\
};

	NANDLER_PROC(SetDataToGrid)
		NANDLER_PROC(SetColorToGrid)
		NANDLER_PROC(LClick)
		NANDLER_PROC(RClick)
#undef NANDLER_PROC

		template<class T>LRESULT Do(T *t, TNotify &l)
	{
		switch (l.pnmh->code)
		{
		case LVN_GETDISPINFO:
		{
			SetDataToGrid()(t, (LV_DISPINFO *)l.pnmh);
		}
		break;
		case NM_CUSTOMDRAW:
		{
			NMLVCUSTOMDRAW *d = (NMLVCUSTOMDRAW *)l.pnmh;
			switch (d->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT: return CDRF_NOTIFYITEMDRAW;
			case CDDS_ITEMPREPAINT: return CDRF_NOTIFYSUBITEMDRAW;
			case CDDS_ITEM | CDDS_SUBITEM | CDDS_PREPAINT:
			{
				SetColorToGrid()(t, d);
			}
			return CDRF_NEWFONT;
			}
		}
		break;
		case NM_CLICK:
		{
			LClick()(t, (LPNMITEMACTIVATE)l.pnmh);
		}
		break;
		case NM_RCLICK:
		{
			RClick()(t, (LPNMITEMACTIVATE)l.pnmh);
		}
		break;
		}
		return 0;
	}

}
/*
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//samples GridItem.h
#pragma once
#include <windows.h>
#include <commctrl.h>
#include "window_tool/MenuAPI.h"

class GridItem : public TEvent
{
public:
	HWND hWnd;
	void Create(HWND);
	void Size(int, int, int, int);
	LRESULT Do(TNotify &);
	void LClick(LPNMITEMACTIVATE);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);
};
////////////////////////////////////////////
//////////////////////////////////////////
///////////////////////////////////////////////
//samples GridItem.cpp
#include "GridItem.h"
#include <strsafe.h>
#include "tools_debug\DebugMess.h"
#include "templates\typelist.hpp"
#include "Grid\GridDetail.h"

namespace
{
	template<class T>struct header_table;
#define Z(T, w, txt)template<>struct header_table<T>\
	{\
	LPWSTR name(){return L###txt;}\
	static const int width = w;\
	};

	struct Num {};
	struct Name {};

	Z(Num, 60, Номер)
		Z(Name, 90, Имя)

#undef Z

		typedef TL::MkTlst<
		Num
		, Name
		>::Result ParameterNameList;
}

void GridItem::Create(HWND hOwner)
{
	hWnd = GridDetail::CreateGrid(hOwner, this);
	GridDetail::SetGridHeader<ParameterNameList> t(hWnd);
	GridDetail::SetRow(hWnd, 10);
}

void GridItem::Size(int x, int y, int width, int heigth)
{
	MoveWindow(hWnd, x, y, width, heigth, TRUE);
}

void GridItem::LClick(LPNMITEMACTIVATE)
{
	dprint(__FUNCTION__" LCLICK \n");
}

void GridItem::SetDataToGrid(LV_DISPINFO *d)
{
	if (0 == d->item.iSubItem)
	{
		wchar_t buf[64];
		wsprintf(buf, L"col %d", d->item.iItem);
		StringCchCopy(d->item.pszText, wcslen(buf) + 1, buf);
	}
}

void GridItem::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	if (0 == ((d->nmcd.dwItemSpec + d->iSubItem) % 2))
	{
		d->clrText = RGB(0, 0, 255);
		d->clrTextBk = RGB(255, 0, 0);
	}
	else
	{
		d->clrText = RGB(255, 0, 255);
		d->clrTextBk = RGB(0, 255, 0);
	}
}

LRESULT GridItem::Do(TNotify &l)
{
	return GridDetail::Do(this, l);
}
*/

