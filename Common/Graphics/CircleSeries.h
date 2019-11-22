#pragma once
#include "Graphics/Chart.h"

class CircleSeries
{
public:
	unsigned *color;
	double *xOffs, *yOffs;
	bool **enabled;
	int count;
	int radius;
	Chart &chart;
	CircleSeries(Chart &chart);
	void Draw();
};