#pragma once
#include "App/App.h"
#include "DspFilters/Filters.hpp"
#include "MedianFiltre/MedianFiltre.h"
#include "templates/impl.hpp"

namespace Data
{ 
	struct SensorData;
	struct InputData; 
	struct ResultData;
}

class Compute
{
	template<class T, void (T:: *ptr)()>static DWORD __stdcall func(LPVOID Parameter)
	{
		(((T *)Parameter)->*ptr)();
		return 0;
	}
	Data::InputData &data;
	Data::ResultData &result;
public:
	unsigned packetSize, frequency, numberPackets;
private:
	unsigned framesCount, strobesTickCount, offsetsTickCount;
public:
	int zoneOffsetsIndex, zoneOffsetsIndexStart;
	MedianFiltre median[App::count_sensors];
	ChMedFlt median_stat[App::count_sensors];
	double(MedianFiltre:: *medianProc)(double, char &);
	char(ChMedFlt:: *medianProc_stat)(char);
public:
	unsigned zoneOffsets[1 + App::count_zones];
private:
	Data::SensorData *sensorData[App::count_sensors];
public:
	unsigned offsAlarmStart[App::count_sensors], offsAlarmStop[App::count_sensors];
	double gainAlarmOffs[App::count_sensors]
		, gainAlarmDelta[App::count_sensors]
		, threshAlarm[App::count_sensors];

	unsigned offsReflectionStart[App::count_sensors], offsReflectionStop[App::count_sensors];
	double gainReflectionOffs[App::count_sensors]
		, gainReflectionDelta[App::count_sensors]
		, threshReflection[App::count_sensors];
	bool bottomReflectionOn[App::count_sensors];
	int wholeStart;
public:
	int wholeStop;
	double fractionalStart, fractionalStop;
	Impl<IDSPFlt, 1032> filter[App::count_sensors];
private:
	void __Update__();
	void __Recalculation__();
public:
	
	Compute();
	void Start();
	bool Strobes();
	void Zone(int zone, int sens);
	void Zone(int sens, char *start, char *stop, double &data, char &status);
	void ComputeFrame(int sens, char *d, double &value, char &status);
	void Update();
	void Recalculation();
};