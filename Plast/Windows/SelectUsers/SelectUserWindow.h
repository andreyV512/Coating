#pragma once
#include "SelectUsers.h"
#include "window_tool/Emptywindow.h"

class GridWindow
{
public:
	HWND hBtnOk, hBtnCancel;
public:
	struct CancelButton: TEvent
	{
		void Do(TCommand &);
	} cancelButton;
	HWND hWnd;
	SelectUsers *grid;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	LRESULT operator()(TNotify &);
	void operator()(TCommand &);
};

template<class T, int width = 320, int height = 300>class GridOptions : public GridWindow
{
	T _grid;
public:
	typedef GridWindow Parent;
	LRESULT operator()(TCreate &l) 
	{
		grid = &_grid;
		grid->Create(l.hwnd);
		LRESULT res = Parent::operator()(l);
		SetWindowLongPtr(hBtnOk, GWLP_USERDATA, (LONG_PTR)&_grid.okButton);
		return res;
	}
	static void Open(wchar_t *title)
	{
		typedef GridOptions<T, width, height> Owner;
		WindowClass<Owner> x;
		HWND hh = FindWindow(x(), 0);
		if (NULL != hh)
		{
			SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hh);
		}
		else
		{
			RECT r;
			GetClientRect(GetDesktopWindow(), &r);
			hh = WindowTemplate(new Owner, title
				, (r.right - width) / 2
				, (r.bottom - height) / 2
				, width
				, height
				, IDI_settings
				, (HBRUSH)COLOR_WINDOW
			);
			LONG_PTR s = GetWindowLongPtr(hh, GWL_STYLE);
			s &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
			SetWindowLongPtr(hh, GWL_STYLE, s);
			SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(hh, SW_SHOWNORMAL);
		}
	}
};

