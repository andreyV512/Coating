#pragma once
#include "Windows/Viewers/CommonSensorViewerBar.h"
#include "Graphics/FixedGridSeries.h"

class ZoneViewer : public CommonSensorViewerBar
{
public:
	typedef CommonSensorViewerBar Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, BarSeriesNoFixed
		, FixedGrid
	> > TChart;
	TChart tchart;
	Cursor tcursor;

	int count;
	int zone;
	int sensor;
	double *data;
	char *status;
	ZoneViewer();
	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorCell(int zone, double &data, unsigned &color);
};