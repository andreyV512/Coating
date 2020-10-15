#pragma once 
#include "Windows/Viewers/CommonSensorViewer.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/Borders.h"
#include "Data/Data.h"
#include "Windows/Viewers/NegThresh.h"

class AScanViewer : public CommonSensorViewer
{
public:
	typedef CommonSensorViewer Parent;
	typedef ChartDraw< Chart, Vlst<
		NoOffsetLeftAxes
		, BottomAxesInt
		, LineSeries
		, FixedGrid
		, AlThr
		, BtmRefThr
		, Neg<AlThr>
		, Neg<BtmRefThr>
	> > TChart;
	TChart tchart;
	Cursor tcursor;
	int numSensor;

	AScanViewer();
};