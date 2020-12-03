#pragma once 
#include "Windows/Viewers/CommonSensorViewerBar.h"
#include "Graphics/FixedGridSeries.h"
#include "Data/Data.h"

class ResultViewer : public CommonSensorViewerBar
{
public:
	typedef CommonSensorViewerBar Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, BarSeriesDouble
		, FixedGrid
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	Data::ResultData(&data);

	ResultViewer();

	void operator()(TMouseWell &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorCell(int, double &, unsigned &, double &, unsigned &);
};