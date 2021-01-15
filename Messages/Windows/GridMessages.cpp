#include "GridMessages.h"
#include "templates/typelist.hpp"
#include "Grid/GridDetail.h"
#include "Grid/DataGrid.hpp"
#include "Log/LogBuffer.h"
#include "window_tool/ItemIni.h"

struct Group {};
struct Color {};
HEADER_TABLE(Group, 70, L"Время")
HEADER_TABLE(Color, 380, L"Сообщение")

void GridMessages::Create(HWND h)
{
	tail = 0;
	FilterInit();
	hWnd = GridDetail::CreateGrid(h, this);
    GridDetail::SetGridHeader<Vlst<Group, Color>> x(hWnd);
	GridDetail::SetRow(hWnd, 1024);
}

void GridMessages::SetDataToGrid(LV_DISPINFO *l)
{
	unsigned col = l->item.iSubItem;
	unsigned row = l->item.iItem;
	Log::TData *d = NULL;
	if(row < tail)
	{
		Log::TData *d = &buf[row];
		switch (col)
		{
		case 0:
		{
			if(1 + row < tail)
			{
				wchar_t delay[32];
				unsigned time = buf[1 + row].time;
				_itow(d->time - time, delay, 10);
				StringCchCopy(l->item.pszText, wcslen(delay) + 1, delay);
			}
			else
			{
				StringCchCopy(l->item.pszText, 2, L"0");
			}
		}
		break;
		case 1:
		{
			wchar_t wbuf[1024];
			bool b = LogMess::FactoryMessages::Instance().Text(d->id, wbuf, d->value);
			if(b)StringCchCopy(l->item.pszText, wcslen(wbuf) + 1, wbuf);
		}
		break;
		}
	}
}

void GridMessages::SetColorToGrid(NMLVCUSTOMDRAW *l)
{
	unsigned row = (int)l->nmcd.dwItemSpec;
	if (row < tail)
	{
		Log::TData *d = &buf[row];
		LogMess::FactoryMessages::Instance().Color(d->id, (unsigned &)l->clrTextBk, (unsigned &)l->clrText);
	}
}

LRESULT GridMessages::Do(TNotify &l)
{
	return  GridDetail::Do(this, l);
}

struct DisableMess{};
struct EnableAllMess {};

template<>struct NameMenu<TopMenu<DisableMess>>
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Отключить сообщение"; }
};
template<>struct NameMenu<TopMenu<EnableAllMess>>
{
	wchar_t *operator()(HWND) { return (wchar_t *)L"Включить все сообщения"; }
};
template<>struct Event<TopMenu<DisableMess>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		GridMessages *x = (GridMessages *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		x->SetSkip(d->iItem);
	}
};
template<>struct Event<TopMenu<EnableAllMess>>
{
	static void Do(LPNMITEMACTIVATE d)
	{
		GridMessages *x = (GridMessages *)GetWindowLongPtr(d->hdr.hwndFrom, GWLP_USERDATA);
		x->Clean();
	}
};

void GridMessages::RClick(LPNMITEMACTIVATE d)
{
	PopupMenu<Vlst<TopMenu<DisableMess>, Separator<0>, TopMenu<EnableAllMess>>>::Do(d->hdr.hwndFrom, d);
}

void GridMessages::Update()
{
	unsigned thead;
	unsigned ttail;

	Log::TData *d = Log::HeadTail(thead, ttail);
	if (thead != head)
	{
		head = thead;
		unsigned start = 0;
		for (unsigned i = thead; i > ttail; --i)
		{
			unsigned k = i % dimention_of(buf);
			
			if (skip.end() == std::find(skip.begin(), skip.end(), d[k].id))
			{
				buf[start] = d[k];
				++start;
			}
		}
		tail = start;
	}
}

void GridMessages::UpdateGrid()
{
	head = 0;
	Update();
	GridDetail::UpdateGridCells(hWnd);
}

void GridMessages::Clean()
{
	FilterClean();
	UpdateGrid();
}

void GridMessages::SetSkip(unsigned id)
{
	FilterSkip(buf[id].id);
	UpdateGrid();
}

static const wchar_t *__section__ = L"FILTER";

void GridMessages::FilterInit()
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	wchar_t buf[32];
	for (unsigned i = 0; i < LogMess::MAX_MESS_ID; ++i)
	{
		bool b = 0 != ItemIni::Get((wchar_t *)__section__, _itow(i, buf, 10), 0, (wchar_t *)path);
		if(b) skip.push_back(i);
	}
}
void GridMessages::FilterSkip(unsigned id)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	skip.push_back(id);
	wchar_t buf[32];
	ItemIni::Set((wchar_t *)__section__, _itow(id, buf, 10), 1, (wchar_t *)path);
}
void GridMessages::FilterClean()
{
	skip.clear();
	wchar_t path[1024];
	ItemIni::GetPath(path);
	wchar_t buf[32];
	for (unsigned i = 0; i < LogMess::MAX_MESS_ID; ++i)
	{
		ItemIni::Set((wchar_t *)__section__, _itow(i, buf, 10), 0, (wchar_t *)path);
	}
}
