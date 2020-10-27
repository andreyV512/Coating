#include "ZonesWindow.h"
#include "ZonesWindowMenu.hpp"
#include "window_tool/MenuAPI.h"
#include <window_tool/Emptywindow.h>
#include "window_tool/Common.h"
#include <CommCtrl.h>
#include "window_tool/ItemIni.h"
#include "DspFilters/DspFiltrParams.h"

#include "DspFilters/ChebyshevFiltre.hpp"

#include "window_tool/TableIni.hpp"
#include "DspFilters/Filters.hpp"
#include "Windows/Viewers/NegThresh.hpp"
#include "MessageText/status.h"

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

	VL::foreach<viewers_list, Common::__create_window__>()(viewers, l.hwnd);

	aScan.tchart.minAxesY = 0;
	aScan.tchart.maxAxesY = 100;

	zoneViewer.tchart.minAxesY = 0;
	zoneViewer.tchart.maxAxesY = 100;

	VL::CopyFromTo(Singleton< TresholdsTable>::Instance().items, treshItems);
	SetThresh();

	UpdateZone();
	return 0;
}

void ZonesWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, 0);
	delete this;
}

struct __move_window_data__
{
	int y, width, height, maxYHeight;
};

template<class O, class P>struct __move_window__
{
	void operator()(O &o, P &p)
	{
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, (WORD)p.height };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.height, TRUE);
		p.y += p.height;
	}
};

template<class P>struct __move_window__<ZonesWindow::Sens, P>
{
	typedef AScanViewer O;
	void operator()(O &o, P &p)
	{
		TSize size{ o.hWnd, WM_SIZE, 0, (WORD)p.width, WORD(p.maxYHeight - p.y) };
		SendMessage(MESSAGE(size));
		MoveWindow(o.hWnd, 0, p.y, p.width, p.maxYHeight - p.y, TRUE);
	}
};

void ZonesWindow::operator()(TSize &l)
{
	if (SIZE_MINIMIZED == l.resizing || 0 == l.Width || 0 == l.Height) return;
	toolBar.Size();
	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	MoveWindow(hStatuisBar, 0, 0, 0, 0, FALSE);
	int y = rt.bottom;	
	RECT st;
	GetClientRect(hStatuisBar, &st);
	int height = l.Height - rt.bottom - st.bottom;
	height /= 4;
	__move_window_data__ data{ y, l.Width, height, l.Height - st.bottom};
	VL::foreach<viewers_list, __move_window__>()(viewers, data);
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

template<class O, class P>struct __test_sub__
{
	bool operator()(O &o, P &p)
	{
		return o.value == p.get<O>().value;
	}
};

template<class List, template<class>class type, template<class>class sub>struct __sub_list__;
template<template<class>class type, template<class>class sub, class Head, class... Tail>struct __sub_list__<Vlst<Head, Tail...>, type, sub>
{
	typedef typename __sub_list__<Vlst<Tail...>, type, sub>::Result Result;
};
template<template<class>class type, template<class>class sub, class Head, class... Tail>struct __sub_list__<Vlst<type<sub<Head>>, Tail...>, type, sub>
{
	typedef typename VL::Append<type<sub<Head>>, typename __sub_list__<Vlst<Tail...>, type, sub>::Result>::Result Result;
};
template<template<class>class type, template<class>class sub>struct __sub_list__<Vlst<>, type, sub>
{
	typedef Vlst<> Result;
};

template<class O, class P>struct __test_param__
{
	bool operator()(P &) { return true; }
};

template<class O, class P>struct __save_sub__
{
	void operator()(O &o, P &p)
	{
		o.value = p.get<O>().value;
	}
};

template<class O, class P>struct __save_param__
{
	void operator()(P &p) { }
};

void ZonesWindow::operator()(TClose &l)
{
	if ( !VL::find< FiltersTable::items_list, __test_param__>()(*this))
	{
		if (IDYES == MessageBox(l.hwnd, L"Сохранить изменения!", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
		{
			VL::foreach< FiltersTable::items_list, __save_param__>()(*this);
			CBase base(ParametersBase().name());
			if (base.IsOpen())
			{
				int id = CurrentId<ID<FiltersTable> >();
				UpdateWhere<FiltersTable>(
					Singleton<FiltersTable>::Instance()
					, base
					).ID(id).Execute();
			}
		}
	}
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

ZonesWindow::ZonesWindow()
	: data(Singleton<Data::InputData>::Instance())
	, compute(Singleton<Compute>::Instance())
	, currentSensor(0)
    , currentZone(0)
	, currentOffset(0)
	, zoneViewer(viewers.get<ZoneViewer>())
	, aScan(viewers.get<Sens>())
{
	VL::CopyFromTo(Singleton<MedianFiltreTable>::Instance().items, median);
	VL::CopyFromTo(Singleton<FiltersTable  >::Instance().items, filter	  );
	VL::CopyFromTo(Singleton<TresholdsTable>::Instance().items, treshItems);
	VL::CopyFromTo(Singleton<DeadZonesTable>::Instance().items, deadZones );

	UpdateMedian();

	zoneViewer.tcursor.SetMouseMoveHandler(this, &ZonesWindow::Draw);
}

void ZonesWindow::LeftCursor(HWND h)
{
	--currentZone;
	UpdateZone();
	UpdateAScan();
}
void ZonesWindow::RightCursor(HWND h)
{
	++currentZone;
	UpdateZone();
	UpdateAScan();
}

void ZonesWindow::UpCursor(HWND h)
{
	--currentSensor;
	UpdateZone();
	UpdateAScan();
}

void ZonesWindow::DownCursor(HWND h)
{
	++currentSensor;
	UpdateZone();
	UpdateAScan();
}

void ZonesWindow::MouseMove(int x)
{
	//wchar_t *buf = aScan.label.buffer;
	//offsetX = x + 1;
	//wsprintf(buf, L"<ff>смещение %d  датчик %d", currentOffset + offsetX, 1 + currentSensor);
	//RepaintWindow(aScan.hWnd);
}

void ZonesWindow::UpdateZone()
{
	if (currentSensor < 0) currentSensor = App::count_sensors - 1;
	else if (currentSensor >= App::count_sensors) currentSensor = 0;

	const int maxZone = compute.zoneOffsetsIndex + compute.wholeStop - 1;	 

	if (currentZone < 0) currentZone = maxZone;
	else if (currentZone >= maxZone) currentZone = 0;

	dprint("sensor %d zone %d\n", currentSensor, currentZone);

	unsigned offsStart = compute.zoneOffsets[0 + currentZone];
	unsigned offsStop = compute.zoneOffsets[1 + currentZone];

	unsigned deadStart = offsStart;
	unsigned deadStop = offsStop;

	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	int wholeStart = deadZoneStart / App::size_zone_mm;
	int wholeStop = deadZoneStop / App::size_zone_mm;

	double fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	double fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;

	__int64 borderStop = data.framesCount - wholeStop - 1;

	const int inc = compute.packetSize * App::count_sensors;
	zoneViewer.tchart.minAxesX = (offsStart - compute.zoneOffsets[0]) / inc;
	zoneViewer.tchart.maxAxesX = (offsStop - compute.zoneOffsets[0]) / inc;
	int count = offsStop - offsStart;
	count /= inc;

	if (offsStop > sizeof(data.buffer))
	{
		dprint("ERROR BUFFER OVERFLOY %d buffer size %d\n", offsStop, sizeof(data.buffer));
	}

	static const int leftOffs = 7;
	int k = 0;
	double ldata;
	char lstatus;
	offsStart -= leftOffs * inc;
	offsStart /= inc;
	offsStart *= inc;
	offsStart += currentSensor * compute.packetSize;
	
	if (currentZone <= wholeStart || currentZone >= compute.zoneOffsetsIndex - 1)
	{
		if (currentZone < wholeStart)
		{
			for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
			}
		}
		else if (currentZone == wholeStart)
		{
			deadStart += int((offsStop - offsStart) * fractionalStart);
			deadStart /= inc;
			deadStart *= inc;
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = lstatus;
			}
		}
		else if (currentZone == compute.zoneOffsetsIndex - 1)
		{
			deadStart = offsStop;
			deadStart -= int((offsStop - offsStart) * fractionalStop);
			deadStart /= inc;
			deadStart *= inc;
			
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = lstatus;
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
			}
		}
		else
		{
			for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
			}
		}
	}
	else
	{
		for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
		{
			compute.ComputeFrame(
				(IDSPFlt &)computeFrame.filter
				, &data.buffer[i]
				, ldata, lstatus
			);
			zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
			zoneViewerStatus[k] = lstatus;
		}
	}

	
	

	
	
	//dprint("offsStart %d offsStop %d   size buff %d\n", offsStart, offsStop, sizeof(data.buffer));
	//for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
	//{
	//	compute.ComputeFrame(
	//		(IDSPFlt &)computeFrame.filter
	//		, &data.buffer[i]
	//		, ldata, lstatus
	//	);
	//	zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
	//	zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
	//}
	//for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
	//{
	//	compute.ComputeFrame(
	//		(IDSPFlt &)computeFrame.filter
	//		, &data.buffer[i]
	//		, ldata, lstatus
	//	);
	//	zoneViewerData[k] = medianFiltre.Val(ldata, lstatus);
	//	zoneViewerStatus[k] = lstatus;
	//}
	zoneViewer.count = count;
	zoneViewer.data = &zoneViewerData[leftOffs] ;
	zoneViewer.status = &zoneViewerStatus[leftOffs] ;
	zoneViewer.zone = 1 + currentZone;
	zoneViewer.sensor = 1 + currentSensor;
	RepaintWindow(zoneViewer.hWnd);
}

void ZonesWindow::UpdateAScan()
{
	aScan.tchart.minAxesX = 0;
	aScan.tchart.maxAxesX = compute.packetSize;

	__int64 offsStart = compute.zoneOffsets[0 + currentZone];
	offsStart /= computeFrame.packetSize;
	offsStart /= App::count_sensors;
	//offsStart *= App::count_sensors;

	offsStart += zoneViewer.currentX;

	computeFrame.Frame(currentSensor, offsStart, aScan.data);
	aScan.line.count = computeFrame.packetSize;
	RepaintWindow(aScan.hWnd);
}

void ZonesWindow::SwitchBipolar(bool b)
{
	VL::foreach<Vlst<Sens>, __switch_bipolar__>()(viewers, b);
	computeFrame.bipolar = b;
}

void ZonesWindow::UpdateMedian()
{
	medianProc = median.get<MedianFiltreON>().value
		? &MedianFiltre::Val: &MedianFiltre::noop;
	medianFiltre.InitWidth(median.get<MedianFiltreWidth>().value);
}

template<class P>struct __set_thresh__<ZoneViewer, P>
{
	void operator()(ZoneViewer &, P &) {}
};

void ZonesWindow::SetThresh()
{
	SetTresh(treshItems, viewers);
}

bool ZonesWindow::Draw(TMouseMove &l, VGraphics &g)
{
	bool b = zoneViewer.Draw(l, g);
	UpdateAScan();
	return b;
}
