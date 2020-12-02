#include "MainWindow.h"
#include "MainWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include <CommCtrl.h>
#include "window_tool/ItemIni.h"

LRESULT MainWindow::operator()(TCreate &l)
{
	Menu<MainWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	select.Create(toolBar.hWnd);
	CreateChildWindow(l.hwnd, &topLabelViewer);
	//topLabelViewer.label = (wchar_t *)L"<ff00>Test";
	topLabelViewer.label.fontHeight = 25;

	hStatuisBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int parts[]{ 300, 650, 900, 1200 };
	SendMessage(hStatuisBar, SB_SETPARTS, dimention_of(parts), (LPARAM)parts);
	HFONT font = CreateFont(30,0,0,90,900,TRUE,0,0,ANSI_CHARSET, 0,0,0,0,L"Arial");
	SendMessage(hStatuisBar, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatuisBar, SB_SETMINHEIGHT, 30, (LPARAM)0);

	cbStopControl.Init(toolBar.hWnd, this
		, &MainWindow::StoreStopControl, LoadStopControl()
		, (wchar_t *)L"Прерывание на просмотр"
		);

	VL::foreach<viewers_list, Common::__create_window__>()(viewers, l.hwnd);
	return 0;
}

void MainWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
}

struct __move_window_data__
{
	int y, width, height, maxYHeight, lengthTube;
};

template<class O, class P>struct __move_window__	
{
	void operator()(O &o, P &p)
	{
		o.tchart.maxAxesX = App::count_zones;
		o.tchart.count = App::count_zones;
		
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, (WORD)p.height };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.height, TRUE);
		p.y += p.height;
	}
};
template<class P>struct __move_window__<ResultViewer, P>
{
	typedef ResultViewer O;
	void operator()(O &o, P &p)
	{
		o.tchart.maxAxesX = App::count_zones;
		o.tchart.count = App::count_zones;
		
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, WORD(p.maxYHeight - p.y) };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.maxYHeight - p.y, TRUE);
	}
};

void MainWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	static const int width = toolBar.Width();
	select.Size(width, 5, 400);
	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	MoveWindow(hStatuisBar, 0, 0, 0, 0, FALSE);
	RECT rs;
	GetClientRect(hStatuisBar, &rs);
	static const int topLabelHeight = 40;
	int y = rt.bottom - 1;
	MoveWindow(topLabelViewer.hWnd, 0, y, l.Width, topLabelHeight, true);

	int t = (l.Height - rs.bottom - rt.bottom - topLabelHeight) / ( 1 + VL::Length<viewers_list>::value);
	y += topLabelHeight;

	__move_window_data__ data{y, l.Width, t, l.Height - rs.bottom, 100};
	VL::foreach<viewers_list, __move_window__>()(viewers, data);

	cbStopControl.Size(width, 60, 200, 20);
}

void MainWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void MainWindow::operator()(TGetMinMaxInfo &l)
{
	if (NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 400;
		l.pMinMaxInfo->ptMinTrackSize.y = 400;
	}
}

void MainWindow::operator()(TClose &l)
{
	if (App::IsRun())
	{
		MessageBox(l.hwnd, L"Программа находится в режиме сбора данных!", L"Cообщение", MB_ICONEXCLAMATION | MB_OK);
	}
	else
	{
		if (IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
		{
			DestroyWindow(l.hwnd);
		}
	}
}

namespace {
	struct __enable_data__
	{
		bool en;
		HWND h;
	};

	template<class O, class P>struct __enable__
	{
		void operator()(P &p)
		{
			UINT t = p.en ? MF_ENABLED : MF_DISABLED;
			EnableMenuItem(GetMenu(p.h), (unsigned short)(ULONG_PTR)Event<O>::Do, t | MF_BYCOMMAND);
			DrawMenuBar(p.h);
		}
	};

	typedef Vlst<
		TopMenu<MainWindowMenu::MainFile>
		, MenuItem<MainWindowMenu::AScanWindow>
		, TopMenu<MainWindowMenu::OptionsFile>
		, MenuItem<MainWindowMenu::__Zones>
	> __menu_enable__;
}

void MainWindow::EnableMenu(bool b)
{
	__enable_data__ data = { b, Singleton<MainWindow>::Instance().hWnd };
	VL::foreach<__menu_enable__, __enable__>()(data);
}

void MainWindow::StoreStopControl(bool b)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	ItemIni::Set((wchar_t *)L"App", (wchar_t *)L"stop_control", b, path);
}

bool MainWindow::LoadStopControl()
{
	wchar_t path[1024];
	ItemIni::GetPath(path);
	return 0 != ItemIni::Get((wchar_t *)L"App", (wchar_t *)L"stop_control", 1, path);
}
