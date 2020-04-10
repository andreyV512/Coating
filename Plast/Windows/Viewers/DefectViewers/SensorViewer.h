#pragma once 
#include "CommonSensorViewer.h"
#include "Data/Data.h"

class SensorViewer : public CommonSensorViewer
{
public:
	typedef CommonSensorViewer Parent;
	typedef ChartDraw< Chart, Vlst<
		BottomAxesInt
		, NoOffsetLeftAxes
		, BarSeriesNoFixed
		, FixedGrid
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	Data::SensorData(&data);

	SensorViewer(Data::SensorData(&));

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorCell(int zone, double &data, unsigned &color);
};