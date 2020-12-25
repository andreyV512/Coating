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
	hWnd = GridDetail::CreateGrid(h, this);
	typename GridDetail::SetGridHeader<Vlst<Group, Color>> x(hWnd);
	GridDetail::SetRow(hWnd, 1024);
}

void GridMessages::SetDataToGrid(LV_DISPINFO *l)
{
	int col = l->item.iSubItem;
	int row = l->item.iItem;
	Log::TData *d = NULL;
	if (Log::IsRow(row, d))
	{
		switch (col)
		{
		case 0:
		{
			Log::TData *d0 = NULL;
			if (Log::IsRow(row + 1, d0))
			{
				wchar_t buf[32];
				_itow(d->time - d0->time, buf, 10);
				StringCchCopy(l->item.pszText, wcslen(buf) + 1, buf);
			}
			else
			{
				StringCchCopy(l->item.pszText, 2, L"0");
			}
		}
		break;
		case 1:
		{
			//char buf[1024];
			wchar_t wbuf[1024];
			bool b = LogMess::FactoryMessages::Instance().Text(d->id, wbuf, d->value);

			//int len = 1 + strlen(buf);
			//MultiByteToWideChar(1251, 0, buf, len, wbuf, len);
			if(b)StringCchCopy(l->item.pszText, wcslen(wbuf) + 1, wbuf);
		}
		break;
		}
	}
}

void GridMessages::SetColorToGrid(NMLVCUSTOMDRAW *l)
{
	int row = (int)l->nmcd.dwItemSpec;
	Log::TData *d = NULL;
	if (Log::IsRow(row, d))
	{
		LogMess::FactoryMessages::Instance().Color(d->id, (unsigned &)l->clrTextBk, (unsigned &)l->clrText);
	}
}

LRESULT GridMessages::Do(TNotify &l)
{
	return  GridDetail::Do(this, l);
}
