#pragma once
#include "window_tool/message.h"
#include "ZonesWindowToolBar.h"
#include "window_tool/TEvent.h"
#include "templates/typelist.hpp"
#include "Windows/Viewers/MainViewers/SensorViewer.h"
#include "Windows/Viewers/ZonesViewer/ZoneViewer.h"
#include "DspFilters/Filters.hpp"
#include "Windows/ZonesWindow/ZonesAxes.h"
#include "Compute/Compute.h"
#include "Compute/ComputeFrame.h"
#include "Windows/Viewers/AScanViewer/AScanViewer.h"

class ZonesWindow
{
public:
	int currentSensor;
	int currentZone;
	int currentOffset;
	HWND hWnd;
	HWND hStatuisBar;
	bool XinMM = true;
	ZonesWindowToolBar toolBar;

	Compute &computeZone;
	ComputeFrame computeFrame;
	MedianFiltreTable::TItems medianItems;
	FiltersTable::TItems &filter;
	TresholdsTable::TItems &treshItems;
	DeadZonesTable::TItems deadZones;

	Data::InputData &data;

	class Sens : public AScanViewer
	{
	public:
		typedef AScanViewer Parent;
		LineSeries &line;
		Gain &gainLine;
		double data[8154];
		double gain[8154];
		Sens() 
			: line(tchart.items.get<LineSeries>())
			, gainLine(tchart.items.get<Gain>())
		{
			line.data = data;
			gainLine.data = gain;
		}
	};

	typedef Vlst<ZoneViewer, Sens>viewers_list;
	VL::Factory< viewers_list> viewers;

	double zoneViewerData[10240];
	char zoneViewerStatus[10240];
	unsigned zoneOffs[10240];

	ZoneViewer &zoneViewer;
	Sens &aScan;

	double(MedianFiltre:: *medianProc)(double, char &, unsigned &);
	MedianFiltre median;
	

	void GainEnable(bool);

	ZonesWindow();

	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);

	static wchar_t *Title() { return (wchar_t *)L"Просмотр зон"; }
	void LeftCursor(HWND h);
	void RightCursor(HWND h);

	void UpCursor(HWND h);
	void DownCursor(HWND h);

	void UpdateZone();
	void UpdateAScan();

	void SwitchBipolar(bool);
	void UpdateMedian();

	void SetThresh();
	bool Draw(TMouseMove &l, VGraphics &g);
};