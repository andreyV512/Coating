#include "GridDetail.h"
namespace GridDetail
{
void SetRow(HWND hWnd, int count)
{
	LV_ITEM l;
	l.mask = LVIF_TEXT | LVIF_IMAGE;
	int c = ListView_GetItemCount(hWnd);
	for(int i = c; i < count; ++i)
	{
		l.iItem = i;
		l.iSubItem = 0;
		l.pszText = LPSTR_TEXTCALLBACK;
		l.cchTextMax = 0;
		ListView_InsertItem(hWnd, &l);
	}
}
void UpdateGridCells(HWND h)
{
	int s = ListView_GetTopIndex(h);
	int c = ListView_GetCountPerPage(h);
	for(int i = 0; i < c; ++i)
	{
		ListView_Update(h, s++);
	}
}

HWND CreateGrid(HWND hwnd, void *self)
{
	HWND h = CreateWindowEx(
		WS_EX_CLIENTEDGE
		, WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT 	
		, 0, 0, 0, 0,
		  hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
	SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)self);
	ListView_SetExtendedListViewStyleEx(h
		, 0
		, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP 
		);
	return h;
}
}