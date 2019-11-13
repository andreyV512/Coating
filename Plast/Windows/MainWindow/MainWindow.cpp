#include "MainWindow.h"

LRESULT MainWindow::operator()(TCreate &l)
{
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

void MainWindow::operator()(TCommand&)
{

}

void MainWindow::operator()(TGetMinMaxInfo&)
{
}
