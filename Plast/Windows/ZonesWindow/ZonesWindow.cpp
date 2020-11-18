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
#include "Windows/StoreParamsBase.hpp"

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
	UpdateAScan();
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

void ZonesWindow::operator()(TClose &l)
{
	bool bmedian     = TestX<MedianFiltreTable>(median);
	bool bfilter     = TestX<FiltersTable>(filter);
	bool btreshItems = TestX<TresholdsTable>(treshItems);
	bool bdeadZones  = TestX<DeadZonesTable>(deadZones);

	if (
		bmedian
		|| bfilter
		|| btreshItems
		|| bdeadZones
		)
	{
		if (IDYES == MessageBox(l.hwnd, L"Данные были изменены. Сохранить изменения?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
		{
			if (TypesizePasswordDlg().Do(l.hwnd))
			{
				CBase base(ParametersBase().name());
				if (base.IsOpen())
				{
					if(bmedian    )  StoreBaseX<MedianFiltreTable>(base, median);
					if(bfilter    )  StoreBaseX<FiltersTable>     (base, filter);
					if(btreshItems)  StoreBaseX<TresholdsTable>   (base, treshItems);
					if(bdeadZones )  StoreBaseX<DeadZonesTable>   (base, deadZones);

					MessageBox(l.hwnd, L"Данные сохранены!", L"Cообщение", MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
	}
	DestroyWindow(l.hwnd);
}

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

void ZonesWindow::UpdateZone()
{
	const int inc = compute.packetSize * App::count_sensors;

	if (currentSensor < 0) currentSensor = App::count_sensors - 1;
	else if (currentSensor >= App::count_sensors) currentSensor = 0;

	const unsigned maxZone = compute.zoneOffsetsIndex + compute.wholeStop - 1;	 

	if (currentZone < 0) currentZone = maxZone;
	else if (currentZone >= maxZone) currentZone = 0;

	dprint("sensor %d zone %d\n", currentSensor, currentZone);

	unsigned offsStart = compute.zoneOffsets[0 + currentZone];
	unsigned offsStop = compute.zoneOffsets[1 + currentZone];

	zoneViewer.tchart.minAxesX = (offsStart - compute.zoneOffsets[0]) / inc;
	zoneViewer.tchart.maxAxesX = (offsStop - compute.zoneOffsets[0]) / inc;

	offsStart /= inc;
	offsStart *= inc;
	offsStop /= inc;
	offsStop *= inc;
	offsStart += currentSensor * compute.packetSize;
	offsStop += currentSensor * compute.packetSize;

	unsigned deadStart = offsStart;

	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	unsigned wholeStart = deadZoneStart / App::size_zone_mm;
	unsigned wholeStop = deadZoneStop / App::size_zone_mm;

	double fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	double fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;

	unsigned borderStop = data.framesCount - wholeStop - 1;

	if (offsStop > sizeof(data.buffer))
	{
		dprint("ERROR BUFFER OVERFLOY %d buffer size %d\n", offsStop, sizeof(data.buffer));
	}

	static const int leftOffs = 7;
	unsigned k = 0;
	double ldata;
	char lstatus;
	
	unsigned xoffsStart = offsStart - leftOffs * inc;
	xoffsStart /= inc;
	xoffsStart *= inc;

	for (unsigned i = xoffsStart; i < offsStart; i += inc)
	{
		compute.ComputeFrame(
			(IDSPFlt &)computeFrame.filter
			, &data.buffer[i]
			, ldata, lstatus
		);
		unsigned z = i;
		(medianFiltre.*medianProc)(ldata, lstatus, z);
	}
	
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
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
			}
		}
		else if (currentZone == wholeStart)
		{
			deadStart += unsigned((offsStop - offsStart) * fractionalStart);
			deadStart /= inc;
			deadStart *= inc;
			deadStart += currentSensor * compute.packetSize;
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = lstatus;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
			}
		}
		else if (currentZone == compute.zoneOffsetsIndex - 1)
		{
			deadStart += int((offsStop - offsStart) * (1.0 - fractionalStop));
			deadStart /= inc;
			deadStart *= inc;
			deadStart += currentSensor * compute.packetSize;
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = lstatus;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				compute.ComputeFrame(
					(IDSPFlt &)computeFrame.filter
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
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
				unsigned z = i;
				zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
				numbers[k] = *(unsigned *)&data.buffer[i];
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
			unsigned z = i;
			zoneViewerData[k] = (medianFiltre.*medianProc)(ldata, lstatus, z);
			zoneViewerStatus[k] = lstatus;
			zoneOffs[k] = z;
			numbers[k] = *(unsigned *)&data.buffer[i];
		}
	}

	zoneViewer.count = int(zoneViewer.tchart.maxAxesX - zoneViewer.tchart.minAxesX);
	zoneViewer.data = zoneViewerData;
	zoneViewer.status = zoneViewerStatus;
	zoneViewer.zone = 1 + currentZone;
	zoneViewer.sensor = 1 + currentSensor;
	RepaintWindow(zoneViewer.hWnd);
}

void ZonesWindow::UpdateAScan()
{
	aScan.tchart.minAxesX = 0;
	aScan.tchart.maxAxesX = compute.packetSize;

	auto &t = treshItems;
	computeFrame.threshAlarm = t.get<AlarmThresh>().value;
	computeFrame.offsAlarmStart = int(t.get<AlarmThreshStart>().value * computeFrame.packetSize * 0.01);
	computeFrame.offsAlarmStop = int(t.get<AlarmThreshStop>().value * computeFrame.packetSize * 0.01);
	computeFrame.gainAlarmOffs = t.get<AlarmGainStart>().value;
	computeFrame.gainAlarmDelta = (t.get<AlarmGainStop>().value - t.get<AlarmGainStart>().value)
		/ (computeFrame.offsAlarmStop - computeFrame.offsAlarmStart);

	computeFrame.threshReflection = t.get<BottomReflectionThresh>().value;
	computeFrame.offsReflectionStart = int(t.get<BottomReflectionThreshStart>().value * computeFrame.packetSize * 0.01);
	computeFrame.offsReflectionStop = int(t.get<BottomReflectionThreshStop>().value * computeFrame.packetSize * 0.01);
	computeFrame.gainReflectionOffs = t.get<BottomReflectionGainStart>().value;
	computeFrame.gainReflectionDelta = (t.get<BottomReflectionGainStop>().value - t.get<BottomReflectionGainStart>().value)
		/ (computeFrame.offsReflectionStop - computeFrame.offsReflectionStart);
	computeFrame.bottomReflectionOn = t.get<BottomReflectionOn>().value;

	computeFrame.Frame(currentSensor, zoneOffs[zoneViewer.currentX], aScan.data);
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
		? &MedianFiltre::ValOffs : &MedianFiltre::noopOffs;
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
	dprint(" current %d number %d\n", zoneViewer.currentX, numbers[zoneViewer.currentX]);
	UpdateAScan();
	return b;
}

