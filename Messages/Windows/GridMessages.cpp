#include "GridMessages.h"
#include "templates/typelist.hpp"
#include "Grid/GridDetail.h"
#include "Grid/DataGrid.hpp"
#include "Log/LogBuffer.h"

struct Group {};
struct Color {};
HEADER_TABLE(Group, 70, L"Время")
HEADER_TABLE(Color, 380, L"Сообщение")

void GridMessages::Create(HWND h)
{
	tail = 0;
	hWnd = GridDetail::CreateGrid(h, this);
	typename GridDetail::SetGridHeader<Vlst<Group, Color>> x(hWnd);
	GridDetail::SetRow(hWnd, 1024);
}

void GridMessages::SetDataToGrid(LV_DISPINFO *l)
{
	int col = l->item.iSubItem;
	int row = l->item.iItem;
	Log::TData *d = NULL;
	//if (Log::IsRow(row, d))
	if(row < tail)
	{
		Log::TData *d = &buf[row];
		switch (col)
		{
		case 0:
		{
			//if (Log::IsRow(row + 1, d0))
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
	int row = (int)l->nmcd.dwItemSpec;
	//Log::TData *d = NULL;
	//if (Log::IsRow(row, d))
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

void GridMessages::Update()
{
	unsigned thead;
	unsigned ttail;

	Log::TData *d = Log::HeadTail(thead, ttail);
	if (thead != head)
	{
		head = thead;
		unsigned start = 0;
		for (int i = thead; i > ttail; --i)
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

void GridMessages::Clean()
{
	skip.clear();
}

void GridMessages::SetSkip(unsigned id)
{
	skip.push_back(id);
}
