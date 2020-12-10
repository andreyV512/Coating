#pragma once
#include "Graphics/Chart.h"
#include "Graphics/FixedGridSeries.h"
class Gain
{
	LineSeries line;
	NoOffsetLeftAxes axes;
public:
	Chart &chart;
	bool enable;
	int &count;
	double *&data;
	double minAxeY, maxAxeY;
	Gain(Chart &chart);
	void Draw();
};