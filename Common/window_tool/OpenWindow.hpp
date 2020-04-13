#pragma once
#include "window_tool/WindowsPosition.h"
#include "window_tool/Emptywindow.h"

template<class T>struct OpenWindow
{
	static HWND Do(HWND)
	{
		HWND hh = FindWindow(WindowClass<T>()(), 0);
		if (NULL != hh)
		{
			SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hh);
		}
		else
		{
			RECT r;
			WindowPosition::Get<T>(r);
			hh = WindowTemplate(new T, T::Title(), r.left, r.top, r.right, r.bottom);
			ShowWindow(hh, SW_SHOWNORMAL);
		}
		return hh;
	}
};

template<class T>struct OpenTopWindow
{
	static HWND Do(HWND)
	{
		HWND hh = FindWindow(WindowClass<T>()(), 0);
		if (NULL != hh)
		{
			SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(hh);
		}
		else
		{
			RECT r;
			WindowPosition::Get<T>(r);
			hh = WindowTemplate(new T, T::Title(), r.left, r.top, r.right, r.bottom);

			//LONG_PTR s = GetWindowLongPtr(hh, GWL_STYLE);
			//s &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
			//SetWindowLongPtr(hh, GWL_STYLE, s);
			SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(hh, SW_SHOWNORMAL);
		}
		return hh;
	}
};