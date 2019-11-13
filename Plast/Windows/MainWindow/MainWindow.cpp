#include "MainWindow.h"
#include "MainWindowMenu.hpp"
#include "window_tool/MenuAPI.h"

LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	return 0;
}

void MainWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
}

void MainWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
}

void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void MainWindow::operator()(TGetMinMaxInfo &)
{
}
