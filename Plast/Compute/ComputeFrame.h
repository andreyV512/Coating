#pragma once
#include "DspFilters/Filters.hpp"
#include "templates/impl.hpp"
#include "App/AppBase.h"

class ComputeFrame
{
public:
	int frequency;
	int packetSize;
	bool bipolar;
	Impl<IDSPFlt, 1032> filter;
	FiltersTable::TItems paramFlt;
	__int64 &framesCount;
	char *buffer;
public:
	ComputeFrame();
	void UpdateFiltre();
	void Frame(int sensor, __int64 offs, double *data);
};

