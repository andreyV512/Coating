#include "SelectUserWindow.h"
#include "window_tool/MenuAPI.h"
#include "window_tool/Common.h"

LRESULT GridWindow::operator()(TCreate &l)
{
	hWnd = l.hwnd;
	hBtnOk = CreateWindow(L"button", L"Применить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
		, 0, 0, 0, 0
		, l.hwnd
		, NULL
		, GetModuleHandle(NULL)
		, NULL
		);
	SetWindowLongPtr(hBtnOk, GWLP_USERDATA, NULL);
	hBtnCancel = CreateWindow(L"button", L"Отмена"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		, 0, 0, 0, 0
		, l.hwnd
		, NULL
		, GetModuleHandle(NULL)
		, NULL
	);
	SetWindowLongPtr(hBtnCancel, GWLP_USERDATA, (LONG_PTR)&cancelButton);
	return 0;
}

void GridWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	delete this;
}

void GridWindow::operator()(TSize &l)
{
	RECT r;
	GetClientRect(l.hwnd, &r);
	grid->Size(0, 0, r.right, r.bottom - 50);
	const int top = r.bottom - 40;
	const int height = 30;
	const int width = r.right / 2 - 40;

	MoveWindow(hBtnOk, 20, top, width, height, TRUE);
	MoveWindow(hBtnCancel, 60 + width, top, width, height, TRUE);
}

LRESULT GridWindow::operator()(TNotify &l)
{
	return EventDo(l);
}

void GridWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void GridWindow::CancelButton::Do(TCommand &l)
{
	DestroyWindow(l.hwnd);
}

