#pragma once
#include "DspFilters/Filters.hpp"
#include "templates/impl.hpp"
#include "App/AppBase.h"

class ComputeFrame
{
public:
	unsigned frequency;
	unsigned packetSize;
	bool bipolar;
	Impl<IDSPFlt, 1032> filter;
	FiltersTable::TItems paramFlt;
	unsigned &framesCount;
	char *buffer;
	unsigned offsAlarmStart, offsAlarmStop;
	double gainAlarmOffs, gainAlarmDelta, threshAlarm;

	unsigned offsReflectionStart, offsReflectionStop;
	double gainReflectionOffs, gainReflectionDelta, threshReflection;

	bool bottomReflectionOn;
public:
	ComputeFrame();
	void UpdateFiltre();
	void Frame(int sensor, unsigned offs, double *data);
};

