#include "ZonesWindow.h"
#include "ZonesWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include <CommCtrl.h>
#include "window_tool/ItemIni.h"

struct __init_radio_btn_data__
{
	int offs;
	int id;
	ZonesWindow *owner;
};
template<class O, class P>struct __init_radio_btn__
{
	void operator()(O &o, P &p)
	{
		wchar_t buf[32];
		_itow_s(O::ID + 1, buf, 10);
		HWND h = CreateWindow(L"button", buf
			, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
			, p.offs, 1, 50, 25, p.owner->toolBar.hWnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
		p.offs += 50;
		SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)&o);
		if (O::ID == p.id)
		{
			Button_SetCheck(h, BST_CHECKED);
			p.owner->sensor.data = &Singleton<Data::Sensor<O::ID>>::Instance().data;
		}
		o.owner = p.owner;
	}
};

ZonesWindow::ZonesWindow()
	: sensor(NULL)
{}

LRESULT ZonesWindow::operator()(TCreate &l)
{
	Menu<ZonesWindowMenu::Menu>().Init(l.hwnd);
	toolBar.Init(l.hwnd);

	hStatuisBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int parts[]{ 300, 650, 900, 1200 };
	SendMessage(hStatuisBar, SB_SETPARTS, dimention_of(parts), (LPARAM)parts);
	HFONT font = CreateFont(30, 0, 0, 90, 900, TRUE, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial");
	SendMessage(hStatuisBar, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatuisBar, SB_SETMINHEIGHT, 30, (LPARAM)0);

	wchar_t path[1024];
	ItemIni::GetPath(path);
	int sensID = ItemIni::Get((wchar_t *)L"ZonesWindow", (wchar_t *)L"SelectedSensor", 1, (wchar_t *)path);

	__init_radio_btn_data__ data = { toolBar.Width() - 70 , sensID, this};
	VL::foreach<radio_btn_list, __init_radio_btn__>()(radio_btns, data);

	sensor.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<SensorViewer>::Proc, (wchar_t *)L"SensorViewer", &sensor);
	sensor.tchart.maxAxesX = App::count_zones;
	zone.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<ZoneViewer>::Proc, (wchar_t *)L"ZoneViewer", &zone);
	aScan.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<AScanZoneViewer>::Proc, (wchar_t *)L"ZoneViewer", &aScan);

	return 0;
}

void ZonesWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	//PostQuitMessage(0);
	delete this;
}

void ZonesWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	MoveWindow(hStatuisBar, 0, 0, 0, 0, FALSE);
	int dy = rt.bottom;
	MoveWindow(sensor.hWnd, 0, dy, rt.right, 100, TRUE);
	dy += 100;
	MoveWindow(zone.hWnd, 0, dy, rt.right, 100, TRUE);
	dy += 100;
	RECT r;
	GetClientRect(l.hwnd, &r);
	RECT st;
	GetClientRect(hStatuisBar, &st);
	MoveWindow(aScan.hWnd, 0, dy, rt.right, r.bottom - dy - st.bottom, TRUE);
}

void ZonesWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void ZonesWindow::operator()(TGetMinMaxInfo &l)
{
	if (NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 400;
		l.pMinMaxInfo->ptMinTrackSize.y = 400;
	}
}

void ZonesWindow::operator()(TClose &l)
{
	//if (App::IsRun())
	//{
	//	MessageBox(l.hwnd, L"Программа находится в режиме сбора данных!", L"Cообщение", MB_ICONEXCLAMATION | MB_OK);
	//}
	//else
	//{
	//	if (IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	//	{
	//		DestroyWindow(l.hwnd);
	//	}
	//}
	DestroyWindow(l.hwnd);
}

struct __set_sensor_data_data__
{
	int id;
	Data::SensorData *data;
};
template<class O, class P>struct __set_sensor_data__
{
	bool operator()(P &p)
	{
		if (p.id == O::ID)
		{
			p.data = &Singleton<Data::Sensor<O::ID>>::Instance().data;
			return false;
		}
		return true;
	}
};

void ZonesWindow::ChangeSensor(int id)
{
	__set_sensor_data_data__ data = {id, NULL};
	VL::find<radio_btn_list, __set_sensor_data__>()(data);
	sensor.data = data.data;
	RepaintWindow(sensor.hWnd);
	{
		wchar_t path[1024];
		ItemIni::GetPath(path);
		ItemIni::Set((wchar_t *)L"ZonesWindow", (wchar_t *)L"SelectedSensor", id, path);
	}
}

void ZonesWindow::LeftCursor(HWND h)
{
	TMouseWell w{sensor.hWnd, WM_MOUSEWHEEL};
	w.delta = 120;
	SendMessage(MESSAGE(w));
}
void ZonesWindow::RightCursor(HWND h)
{
	TMouseWell w{ sensor.hWnd, WM_MOUSEWHEEL};
	w.delta = -120;
	SendMessage(MESSAGE(w));
}
