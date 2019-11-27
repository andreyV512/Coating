#pragma once
#include "window_tool/Emptywindow.h"
#include "window_tool/TEvent.h"
#include <CommCtrl.h>

class DataGrid : public TEvent
{
public:
	struct OkButton : TEvent
	{
		DataGrid &owner;
		OkButton(DataGrid &);
		void Do(TCommand &)override;
	} okButton;
	HWND hWnd;
	DataGrid();
	virtual void Create(HWND) = 0;
	void Size(int, int, int, int);
	LRESULT Do(TNotify &) override;
	virtual void RClick(LPNMITEMACTIVATE) = 0;
	virtual void SetDataToGrid(LV_DISPINFO *) = 0;
	virtual void SetColorToGrid(NMLVCUSTOMDRAW *) = 0;
	virtual void ButtonClick(HWND) = 0;
};

class GridWindowX
{
public:
	HWND hBtnOk, hBtnCancel;
public:
	struct CancelButton : TEvent
	{
		void Do(TCommand &);
	} cancelButton;
	HWND hWnd;
	DataGrid *grid;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	LRESULT operator()(TNotify &);
	void operator()(TCommand &);
};

template<class T>class GridOptions : public GridWindowX
{
	T _grid;
public:
	typedef GridWindowX Parent;
	LRESULT operator()(TCreate &l)
	{
		grid = &_grid;
		grid->Create(l.hwnd);
		LRESULT res = Parent::operator()(l);
		SetWindowLongPtr(hBtnOk, GWLP_USERDATA, (LONG_PTR)&_grid.okButton);
		return res;
	}
	static void Open(wchar_t *title, int width, int height)
	{
		typedef GridOptions<T> Owner;
		HWND hh = FindWindow(WindowClass<Owner>()(), 0);
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
