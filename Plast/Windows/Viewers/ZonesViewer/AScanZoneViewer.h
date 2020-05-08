#pragma once 
#include "Windows/Viewers/CommonSensorViewer.h"
#include "Graphics/FixedGridSeries.h"
#include "Data/Data.h"

class AScanZoneViewer : public CommonSensorViewer
{
public:
	typedef CommonSensorViewer Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, LineSeries
		, FixedGrid
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	//Data::ResultData(&data);

	AScanZoneViewer();

	//void operator()(TMouseWell &);

//	bool Draw(TMouseMove &, VGraphics &);
//	bool GetColorCell(int, double &, unsigned &, double &, unsigned &);
};