#pragma once 
#include "CommonSensorViewer.h"
#include "Data/Data.h"

class ResultViewer : public CommonSensorViewer
{
public:
	typedef CommonSensorViewer Parent;
	typedef ChartDraw< Chart, Vlst<
		BottomAxesInt
		, NoOffsetLeftAxes
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