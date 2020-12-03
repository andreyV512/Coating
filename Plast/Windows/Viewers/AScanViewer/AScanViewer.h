#pragma once 
#include "Windows/Viewers/CommonSensorViewerLine.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/Borders.h"
#include "Data/Data.h"
#include "Windows/Viewers/NegThresh.h"

class Line : public LineSeries {public: Line(Chart &chart) : LineSeries(chart) {}};
class Gain : public LineSeries
{
public:
	Gain(Chart &chart)
		: LineSeries(chart)
	{
		color = 0xffffff00;
		dashStyle = DashStyleDot;
		widthPen = 1;
	}
};

class AScanViewer : public CommonSensorViewerLine
{
public:
	typedef CommonSensorViewerLine Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, FixedGrid
		, Line
		, Gain
		, AlThr
		, BtmRefThr
		, Neg<AlThr>
		, Neg<BtmRefThr>
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	double *&data;

	AScanViewer();
	bool Draw(TMouseMove &l, VGraphics &g);
};