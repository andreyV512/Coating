#pragma once
#include "App/App.h"
#include "DspFilters/Filters.hpp"
#include "MedianFiltre/MedianFiltre.h"
#include "templates/impl.hpp"

namespace Data
{ 
	struct SensorData;
	struct InputData; 
}

class Compute
{
	Data::InputData &data;
	unsigned packetSize, numberPackets, framesCount, strobesTickCount, offsetsTickCount, zoneOffsetsIndex;
	IDSPFlt dspFilt;
	VL::Factory<filters_list> factoryFilters[App::count_sensors];
	IDSPFlt *filters[App::count_sensors];

	MedianFiltre median[App::count_sensors];
	double(MedianFiltre:: *medianProc)(double, char &);

	int zoneOffsets[1 + App::count_zones];
	Data::SensorData *sensorData[App::count_sensors];

	int offsAlarmStart, offsAlarmStop;
	double gainAlarmOffs, gainAlarmDelta, threshAlarm;

	int offsReflectionStart, offsReflectionStop;
	double gainReflectionOffs, gainReflectionDelta, threshReflection;
	bool bottomReflectionOn;
	Impl<IDSPFlt, 1032> filter;
public:
	
	Compute();

	void Start();

	bool Strobes();
	void Zone(int zone, int sens);
	void ComputeFrame(IDSPFlt &f, int sensor, char *d, double &value, char &status);
	void ComputeZone(int zone);
	void Update();
	void Done();
	void Recalculation();
};