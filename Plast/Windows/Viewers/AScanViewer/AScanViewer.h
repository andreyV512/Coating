#pragma once 
#include "Windows/Viewers/CommonSensorViewerLine.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/Borders.h"
#include "Data/Data.h"
#include "Windows/Viewers/NegThresh.h"
#include "Windows/Viewers/Gain.h"

class AScanViewer : public CommonSensorViewerLine
{
public:
	typedef CommonSensorViewerLine Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, FixedGrid
		, LineSeries
		, AlThr
		, BtmRefThr
		, Neg<AlThr>
		, Neg<BtmRefThr>
		, Gain
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;
	double *&data;

	AScanViewer();
	bool Draw(TMouseMove &l, VGraphics &g);
};