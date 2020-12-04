#pragma once
#include "DspFilters/Filters.hpp"
#include "templates/impl.hpp"
#include "App/AppBase.h"
#include <App/App.h>

class ComputeFrame
{
public:
	unsigned frequency;
	unsigned packetSize;
	bool bipolar;
	Impl<IDSPFlt, 1032> filter[App::count_sensors];
	FiltersTable::TItems paramFlt;
	TresholdsTable::TItems treshItems;
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

