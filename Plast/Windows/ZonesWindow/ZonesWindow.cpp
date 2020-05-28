#include "ZonesWindow.h"
#include "ZonesWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include <CommCtrl.h>
#include "window_tool/ItemIni.h"
#include "DspFilters/DspFiltrParams.h"
#include "DspFilters/Filters.hpp"
#include "DspFilters/ChebyshevFiltre.hpp"

double data[32][100000];
#define READ(type, val)	type val; fread(&val, sizeof(type), 1, f);
int Load()
{
	int offs = 0;
	FILE *f = NULL;
	if (!fopen_s(&f, "..\\..\\xxx.bindkb2", "rb"))
	{
		READ(short, capasity)
		READ(short, countSensors)
			
		int size = 0;

		for (short k = 0; k < capasity; ++k)
		{

			READ(double, dBClass)
				READ(int, zoneLength)

				for (short i = 0; i < countSensors; ++i)
				{
					READ(double, level)
						fread(&size, sizeof(int), 1, f);
					for (int j = 0; j < size; ++j)
					{
						READ(double, val)
							data[i][j + offs] = val;
					}
				}
			offs += size;
		}
		fclose(f);
	}
	return offs;
}
#undef READ

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
	//: sensor(NULL)
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

	//__init_radio_btn_data__ data = { toolBar.Width() - 70 , sensID, this};
	//VL::foreach<radio_btn_list, __init_radio_btn__>()(radio_btns, data);

	//sensor.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<SensorViewer>::Proc, (wchar_t *)L"SensorViewer", &sensor);
	//sensor.tchart.maxAxesX = App::count_zones;
	//zone.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<ZoneViewer>::Proc, (wchar_t *)L"ZoneViewer", &zone);
	aScan.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<AScanZoneViewer>::Proc, (wchar_t *)L"AScanZoneViewer", &aScan);

	countSamples = Load();
	aScan.tchart.items.get<LineSeries>().SetData(data[0], 256);
	currentOffset = 0;
	currentSensor = 0;
	return 0;
}

void ZonesWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
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
	//MoveWindow(sensor.hWnd, 0, dy, rt.right, 100, TRUE);
	//dy += 100;
	//MoveWindow(zone.hWnd, 0, dy, rt.right, 100, TRUE);
	//dy += 100;
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
	//__set_sensor_data_data__ data = {id, NULL};
	//VL::find<radio_btn_list, __set_sensor_data__>()(data);
	//sensor.data = data.data;
	//RepaintWindow(sensor.hWnd);
	//{
	//	wchar_t path[1024];
	//	ItemIni::GetPath(path);
	//	ItemIni::Set((wchar_t *)L"ZonesWindow", (wchar_t *)L"SelectedSensor", id, path);
	//}
}
static const int cnt = 512;
double xdata[2048];

struct __init_filtre_data__
{
	IDSPFlt xfiltre;
	char buf[2000];
	FiltersTable::TItems &items;
	IDSPFlt *filtre;
	__init_filtre_data__()
		: items(Singleton<FiltersTable>::Instance().items)
		, filtre(&xfiltre)
	{}
};

template<int A, int B, bool>struct A_above_B;
template<int A, int B>struct A_above_B<A, B, true> {};

template<class O, class P>struct __init_filtre__;
template<template<class>class type, template<class>class sub, class X, class P>struct __init_filtre__<type<sub<X>>, P>
{
	typedef type<sub<X>> O;
	bool operator()(P &p)
	{
		if (VL::IndexOf<type_flites_list, O>::value == p.items.get<CurrentFilter>().value)
		{
			typedef DSPFlt<type, sub> F;
			A_above_B<sizeof(p.buf), sizeof(F ), (sizeof(p.buf) > sizeof(F))>();
			F *f = new(p.buf)F;
			SetupFiltre<F>()(*f, p.items, 10000);
			p.filtre = (IDSPFlt *)p.buf;
			return false;
		}
		return true;
	}
};

double *Filtre(double *d, int count)
{
	FiltersTable::TItems &xxx = Singleton<FiltersTable>::Instance().items;
	memmove(xdata, d - 512, (count + 512) * sizeof(double));
	__init_filtre_data__ data;
	VL::foreach<type_flites_list, __init_filtre__>()(data);
	for (int i = 0; i < count + 512; ++i)
	{
		xdata[i] = (*data.filtre)(xdata[i]);
	}
	return &xdata[512];
}
void ZonesWindow::LeftCursor(HWND h)
{
	//TMouseWell w{sensor.hWnd, WM_MOUSEWHEEL};
	//w.delta = 120;
	//SendMessage(MESSAGE(w));
	currentOffset -= 256;
	if (currentOffset < 0)currentOffset = 0;
	wchar_t *buf = aScan.label.buffer;
	wsprintf(buf, L"<ff>смещение %d  датчик %d", currentOffset, currentSensor);
	aScan.tchart.items.get<LineSeries>().SetData(Filtre(&data[currentSensor][currentOffset], cnt), cnt);
	aScan.tchart.minAxesX = currentOffset;
	aScan.tchart.maxAxesX = currentOffset + cnt;
	RepaintWindow(aScan.hWnd);
}
void ZonesWindow::RightCursor(HWND h)
{
	//TMouseWell w{ sensor.hWnd, WM_MOUSEWHEEL};
	//w.delta = -120;
	//SendMessage(MESSAGE(w));
	currentOffset += 256;
	if (currentOffset > countSamples) currentOffset = countSamples - 256;
	wchar_t *buf = aScan.label.buffer;
	wsprintf(buf, L"<ff>смещение %d  датчик %d", currentOffset, currentSensor);
	aScan.tchart.items.get<LineSeries>().SetData(Filtre(&data[currentSensor][currentOffset], cnt), cnt);
	aScan.tchart.minAxesX = currentOffset;
	aScan.tchart.maxAxesX = currentOffset + cnt;
	RepaintWindow(aScan.hWnd);
}

void ZonesWindow::UpCursor(HWND h)
{
	--currentSensor;
	if (currentSensor < 0) currentSensor = 0;
	wchar_t *buf = aScan.label.buffer;
	wsprintf(buf, L"<ff>смещение %d  датчик %d", currentOffset, currentSensor);
	aScan.tchart.items.get<LineSeries>().SetData(Filtre(&data[currentSensor][currentOffset], cnt), cnt);
	RepaintWindow(aScan.hWnd);
}

void ZonesWindow::DownCursor(HWND h)
{
	++currentSensor;
	if(currentSensor > 31)  currentSensor = 31;
	wchar_t *buf = aScan.label.buffer;
	wsprintf(buf, L"<ff>смещение %d  датчик %d", currentOffset, currentSensor);
	aScan.tchart.items.get<LineSeries>().SetData(Filtre(&data[currentSensor][currentOffset], cnt), cnt);
	RepaintWindow(aScan.hWnd);
}
