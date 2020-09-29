#pragma once
#include "DspFilters/Filters.hpp"
#include "templates/impl.hpp"
#include "App/AppBase.h"

class ComputeFrame
{
public:
	int frequency;
	int packetSize;
	Impl<IDSPFlt, 1032> filter;
	FiltersTable::TItems paramFlt;
	const unsigned &framesCount;
	char *buffer;
public:
	ComputeFrame();
	void UpdateFiltre();
	void Frame(int sensor, int offs, double *data);
};

