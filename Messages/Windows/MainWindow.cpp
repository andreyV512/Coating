#include "MainWindow.h"
#include <CommCtrl.h>
#include "window_tool/MenuApi.h"
#include "Log/LogBuffer.h"
#include "MainWindowMenu.hpp"
//------------------------------------------------------------------------
LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::Menu>().Init(l.hwnd);
	grid.Create(l.hwnd);
	SetTimer(l.hwnd,             // хэндл главного окна
		IDT_TIMER1,            // идентификатор таймера
		300,                 // интервал - 1 секунд
		(TIMERPROC)NULL);     // процедуры таймера нет
	return 0;
}

void MainWindow::operator()(TDestroy &l)
{
	KillTimer(l.hwnd, IDT_TIMER1);
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
}

void MainWindow::operator()(TSize &l)
{
	RECT r;
	GetClientRect(l.hwnd, &r);
	MoveWindow(grid.hWnd, 0, 0, r.right, r.bottom, TRUE);
}

LRESULT MainWindow::operator()(TNotify &l)
{
	return EventDo(l);
}

void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void MainWindow::operator()(TTimer &l)
{
	switch (l.ID)
	{
	case IDT_TIMER1:
	{
		unsigned last = Log::LastMessageIndex();
		if (lastIndex == last) break;
		lastIndex = last;
		int i = ListView_GetTopIndex(grid.hWnd);
		int count = i + ListView_GetCountPerPage(grid.hWnd);
		for (int z = i; z < count; ++z)
		{
			ListView_Update(grid.hWnd, z);
		}
	}
	break;
	}
}

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")