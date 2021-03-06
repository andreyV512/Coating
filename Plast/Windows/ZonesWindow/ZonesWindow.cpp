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
#include "Compute/SetTresholds.hpp"
#include "Data/StoreAllParam.h"

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
	ZonesWindow *owner;
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
		if (p.owner->XinMM)
		{
			o.tchart.maxAxesX *= ALLPatramsItems<TresholdsTable>().get<SoundSpeed>().value;
			o.tchart.maxAxesX /= 2000.0 * ALLPatramsItems<LanParametersTable>().get<Frequency>().value;
		}
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
	__move_window_data__ data{this, y, l.Width, height, l.Height - st.bottom};
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
	bool bmedian     = TestX<MedianFiltreTable>(medianItems);
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
					if(bmedian    )  StoreBaseX<MedianFiltreTable>(base, medianItems);
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

void ZonesWindow::GainEnable(bool b)
{
	aScan.gainLine.enable = b;
}

#define SET_ITEMS(table)Singleton<ALLPatrams>::Instance().items.get<VL::Factory<table::items_list>>()

ZonesWindow::ZonesWindow()
	: data(Singleton<Data::InputData>::Instance())
	, computeZone(Singleton<Compute>::Instance())
	, currentSensor(0)
    , currentZone(0)
	, currentOffset(0)
	, zoneViewer(viewers.get<ZoneViewer>())
	, aScan(viewers.get<Sens>())
	, filter(ALLPatramsItems<FiltersTable>())
	, treshItems(ALLPatramsItems<TresholdsTable>())
	, medianItems(ALLPatramsItems<MedianFiltreTable>())
	, deadZones(ALLPatramsItems<DeadZonesTable>())
{
	UpdateMedian();
	zoneViewer.tcursor.SetMouseMoveHandler(this, &ZonesWindow::Draw);
}
#undef SET_ITEMS

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
template<class O, class P>struct __select_tresh_sens__;
void ZonesWindow::UpCursor(HWND h)
{
	--currentSensor;
	currentSensor %= App::count_sensors;
	UpdateMedian();
	UpdateZone();
	VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __select_tresh_sens__>()(*this);
	UpdateAScan();
}

void ZonesWindow::DownCursor(HWND h)
{
	++currentSensor;
	currentSensor %= App::count_sensors;
	UpdateMedian();
	UpdateZone();
	VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __select_tresh_sens__>()(*this);
	UpdateAScan();
}

void ZonesWindow::UpdateZone()
{
	const int inc = computeZone.packetSize * App::count_sensors;

	if (currentSensor < 0) currentSensor = App::count_sensors - 1;
	else if (currentSensor >= App::count_sensors) currentSensor = 0;

	const int maxZone = computeZone.zoneOffsetsIndex + computeZone.wholeStop - 1;

	if (currentZone < 0) currentZone = maxZone;
	else if (currentZone >= maxZone) currentZone = 0;

	unsigned offsStart = computeZone.zoneOffsets[0 + currentZone];
	unsigned offsStop = computeZone.zoneOffsets[1 + currentZone];

	zoneViewer.tchart.minAxesX = (offsStart - computeZone.zoneOffsets[0]) / inc;
	zoneViewer.tchart.maxAxesX = (offsStop - computeZone.zoneOffsets[0]) / inc;

	offsStart /= inc;
	offsStart *= inc;
	offsStop /= inc;
	offsStop *= inc;
	offsStart += currentSensor * computeZone.packetSize;
	offsStop += currentSensor * computeZone.packetSize;

	unsigned deadStart = offsStart;

	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	int wholeStart = deadZoneStart / App::size_zone_mm;
	int wholeStop = deadZoneStop / App::size_zone_mm;

	double fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	double fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;

	unsigned borderStop = data.framesCount - wholeStop - 1;

	if (offsStop > Data::InputData::buffSize)
	{
		dprint("ERROR BUFFER OVERFLOY %u buffer size %u\n", offsStop, Data::InputData::buffSize);
	}

	static const int leftOffs = dimention_of(MedianFiltre::buf);
	unsigned k = 0;
	double ldata;
	char lstatus;
	
	int xoffsStart = offsStart - leftOffs * inc;
	if (xoffsStart < 0)
	{
		xoffsStart = 0;//offsStart;
		median.Clear();
	}

	for (unsigned i = xoffsStart; i < offsStart; i += inc)
	{
		computeZone.ComputeFrame(
			currentSensor
			, &data.buffer[i]
			, ldata, lstatus
		);
		unsigned z = i;
		(median.*medianProc)(ldata, lstatus, z);
	}

	if (currentZone <= wholeStart || currentZone >= computeZone.zoneOffsetsIndex - 1)
	{
		if (currentZone < wholeStart)
		{
			for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
			}
		}
		else if (currentZone == wholeStart)
		{
			deadStart += unsigned((offsStop - offsStart) * fractionalStart);
			deadStart /= inc;
			deadStart *= inc;
			deadStart += currentSensor * computeZone.packetSize;
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = lstatus;
				zoneOffs[k] = z;
			}
		}
		else if (currentZone == computeZone.zoneOffsetsIndex - 1)
		{
			deadStart += int((offsStop - offsStart) * (1.0 - fractionalStop));
			deadStart /= inc;
			deadStart *= inc;
			deadStart += currentSensor * computeZone.packetSize;
			for (unsigned i = offsStart; i < deadStart; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = lstatus;
				zoneOffs[k] = z;
			}
			for (unsigned i = deadStart; i < offsStop; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
			}
		}
		else
		{
			for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
			{
				computeZone.ComputeFrame(
					currentSensor
					, &data.buffer[i]
					, ldata, lstatus
				);
				unsigned z = i;
				zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
				zoneViewerStatus[k] = VL::IndexOf<zone_status_list, DeadZone>::value;
				zoneOffs[k] = z;
			}
		}
	}
	else
	{
		for (unsigned i = offsStart; i < offsStop; i += inc, ++k)
		{
			computeZone.ComputeFrame(
				currentSensor
				, &data.buffer[i]
				, ldata, lstatus
			);
			unsigned z = i;
			zoneViewerData[k] = (median.*medianProc)(ldata, lstatus, z);
			zoneViewerStatus[k] = lstatus;
			zoneOffs[k] = z;
		}
	}

	zoneViewer.count = int(zoneViewer.tchart.maxAxesX - zoneViewer.tchart.minAxesX);
	zoneViewer.data = zoneViewerData;
	zoneViewer.status = zoneViewerStatus;
	zoneViewer.zone = 1 + currentZone;
	zoneViewer.sensor = 1 + currentSensor;
	RepaintWindow(zoneViewer.hWnd);
}
struct __get_axes_Y_data__
{
	int sensor;
	double value = 0.0;
	TresholdsTable::TItems &data;
	__get_axes_Y_data__(int sensor, TresholdsTable::TItems &data)
		: sensor(sensor)
		, data(data)
	{}
};

template<class O, class P>struct __get_axes_Y__
{
	bool operator()(P &p)
	{
		static const int N = O::value;
		if (N == p.sensor)
		{
			p.value = p.data.get<Num<AlarmGainStop, N>>().value;
			return false;
		}
		return true;
	}
};
void ZonesWindow::UpdateAScan()
{
	aScan.tchart.minAxesX = 0;
	aScan.tchart.maxAxesX = computeZone.packetSize - 1;

	if (XinMM)
	{
		aScan.tchart.maxAxesX *= ALLPatramsItems<TresholdsTable>().get<SoundSpeed>().value;
		aScan.tchart.maxAxesX /= 2000.0 * ALLPatramsItems<LanParametersTable>().get<Frequency>().value;
	}

	SetParam(computeFrame, treshItems);
	computeFrame.Frame(currentSensor, zoneOffs[zoneViewer.currentX], aScan.data);
	aScan.line.count = computeFrame.packetSize;

	__get_axes_Y_data__ data(currentSensor, treshItems);
	VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __get_axes_Y__>()(data);
	aScan.gainLine.maxAxeY = 1.1 * data.value;
	computeFrame.Gain(currentSensor, aScan.gain);
	aScan.gainLine.count = computeFrame.packetSize;
	aScan.chart->count = computeFrame.packetSize;
	RepaintWindow(aScan.hWnd);
}

void ZonesWindow::SwitchBipolar(bool b)
{
	VL::foreach<Vlst<Sens>, __switch_bipolar__>()(viewers, b);
	computeFrame.bipolar = b;
}
template<class Owner, class Param>struct __param_median_filtre__
{
	Owner &o;
	Param &t;
	int sens;
	__param_median_filtre__(Owner &o, Param &t, int sens) : o(o), t(t), sens(sens) {}
};
void ZonesWindow::UpdateMedian()
{
	__param_median_filtre__<ZonesWindow, MedianFiltreTable::TItems> data(*this, medianItems, currentSensor);
	VL::find<typename VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __set_median_once__>()(data);
}

template<int N, class T>struct ZonesWindow_SetThresh_Wrap: T
{
	static const int NUM = N;
};

template<class O, class P>struct __select_tresh_sens__
{
	bool operator()(P &p)
	{
		static const int i = O::value;
		if (i == p.currentSensor)
		{
			VL::foreach<Vlst<ZonesWindow::Sens>, __set_thresh__>()(p.viewers, (ZonesWindow_SetThresh_Wrap<i, TresholdsTable::TItems> &)p.treshItems);
			return false;
		}
		return true;
	}
};

void ZonesWindow::SetThresh()
{
	VL::find<VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __select_tresh_sens__>()(*this);
	SetParam(computeZone, treshItems);
	SwitchBipolar(computeFrame.bipolar);
}

bool ZonesWindow::Draw(TMouseMove &l, VGraphics &g)
{
	bool b = zoneViewer.Draw(l, g);
	UpdateAScan();
	return b;
}
