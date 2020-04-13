#include "AScanWindow.h"
#include <CommCtrl.h>
#include "AScanWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "AScanKeyHandler.h"

LRESULT AScanWindow::operator()(TCreate &l)
{
	Menu<AScanWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	CreateChildWindow(l.hwnd, &topLabelViewer);
	topLabelViewer.label = (wchar_t *)L"<ff00>Test";
	topLabelViewer.label.fontHeight = 25;

	hStatuisBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int parts[]{ 300, 650, 900, 1200 };
	SendMessage(hStatuisBar, SB_SETPARTS, dimention_of(parts), (LPARAM)parts);
	HFONT font = CreateFont(30, 0, 0, 90, 900, TRUE, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");
	SendMessage(hStatuisBar, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatuisBar, SB_SETMINHEIGHT, 30, (LPARAM)0);

	AScanKeyHandler::Stop();

	//VL::foreach<viewers_list, Common::__create_window__>()(viewers, l.hwnd);
	return 0;
}

void AScanWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	AppKeyHandler::Stop();
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
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, (WORD)p.height };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.maxYHeight - p.y, TRUE);
	}
};

void AScanWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	MoveWindow(hStatuisBar, 0, 0, 0, 0, FALSE);
	RECT rs;
	GetClientRect(hStatuisBar, &rs);
	static const int topLabelHeight = 40;
	int y = rt.bottom - 1;
	MoveWindow(topLabelViewer.hWnd, 0, y, l.Width, topLabelHeight, true);

	//int t = (l.Height - rs.bottom - rt.bottom - topLabelHeight) / (1 + VL::Length<viewers_list>::value);
	//y += topLabelHeight;
	//
	//__move_window_data__ data{ y, l.Width, t, l.Height - rs.bottom, 100 };
	//VL::foreach<viewers_list, __move_window__>()(viewers, data);

}

void AScanWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void AScanWindow::operator()(TGetMinMaxInfo &l)
{
	if (NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 400;
		l.pMinMaxInfo->ptMinTrackSize.y = 400;
	}
}
