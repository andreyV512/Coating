#pragma once
#include "window_tool/message.h"
#include "ZonesWindowToolBar.h"
#include "window_tool/TEvent.h"
#include "templates/typelist.hpp"
#include "Windows/Viewers/MainViewers/SensorViewer.h"
#include "Windows/Viewers/ZonesViewer/ZoneViewer.h"
#include "Windows/Viewers/ZonesViewer/AScanZoneViewer.h"
#include "DspFilters/Filters.hpp"
#include "Windows/ZonesWindow/ZonesAxes.h"
#include "Compute/Compute.h"

class ZonesWindow
{
public:
	int currentSensor;
	int currentZone;
	int currentOffset;
	HWND hWnd;
	HWND hStatuisBar;
	ZonesWindowToolBar toolBar;

	MedianFiltreTable::TItems median;
	FiltersTable::TItems filter;
	TresholdsTable::TItems tresh;

	Data::InputData &data;
	Compute &compute;

	typedef Vlst<ZoneViewer, AScanZoneViewer>viewers_list;
	VL::Factory< viewers_list> viewers;

	double zoneViewerData[10240];
	char zoneViewerStatus[10240];

	ZoneViewer &zoneViewer;
	AScanZoneViewer &aScan;

	MedianFiltre medianFiltre;
	DSPFltDump filtre;

	ZonesWindow();

	/// <summary>
	/// 
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);

	void ChangeSensor(int);
	static wchar_t *Title() { return (wchar_t *)L"Просмотр зон"; }
	void LeftCursor(HWND h);
	void RightCursor(HWND h);

	void UpCursor(HWND h);
	void DownCursor(HWND h);

	void Update();

	void MouseMove(int);

	void UpdateZone();
	void UpdateAScan();
};