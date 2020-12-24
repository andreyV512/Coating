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
	FiltersTable::TItems &paramFlt;
	TresholdsTable::TItems &treshItems;
	unsigned &framesCount;
	char *buffer;
	unsigned offsAlarmStart[App::count_sensors], offsAlarmStop[App::count_sensors];
	double gainAlarmOffs[App::count_sensors], gainAlarmDelta[App::count_sensors], threshAlarm[App::count_sensors];

	unsigned offsReflectionStart[App::count_sensors], offsReflectionStop[App::count_sensors];
	double gainReflectionOffs[App::count_sensors]
		, gainReflectionDelta[App::count_sensors]
		, threshReflection[App::count_sensors];

	bool bottomReflectionOn[App::count_sensors];
public:
	ComputeFrame();
	void UpdateFiltre();
	void Frame(int sensor, unsigned offs, double *data);
	void Gain(int sensor, double *data);
};

