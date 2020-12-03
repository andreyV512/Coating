#pragma once 
#include "Windows/Viewers/CommonSensorViewerBar.h"
#include "Data/Data.h"
#include "Graphics/FixedGridSeries.h"

class SensorViewer : public CommonSensorViewerBar
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
	int numSensor;
	Data::SensorData &data;

	SensorViewer(Data::SensorData &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorCell(int zone, double &data, unsigned &color);
};