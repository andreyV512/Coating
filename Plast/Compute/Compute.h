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
	unsigned packetSize;
private:
	unsigned numberPackets, framesCount, strobesTickCount, offsetsTickCount;
public:
	unsigned zoneOffsetsIndex;
private:
	MedianFiltre median[App::count_sensors];
	double(MedianFiltre:: *medianProc)(double, char &);
public:
	int zoneOffsets[1 + App::count_zones];
private:
	Data::SensorData *sensorData[App::count_sensors];

	int offsAlarmStart, offsAlarmStop;
	double gainAlarmOffs, gainAlarmDelta, threshAlarm;

	int offsReflectionStart, offsReflectionStop;
	double gainReflectionOffs, gainReflectionDelta, threshReflection;
	bool bottomReflectionOn;
	Impl<IDSPFlt, 1032> filter;
	void __Update__();
	void __Recalculation__();
public:
	
	Compute();
	~Compute();

	void Start();

	bool Strobes();
	void Zone(int zone, int sens);
	void ComputeFrame(IDSPFlt &f, char *d, double &value, char &status);
	void ComputeZone(int zone);
	
	void Done();
	void Update();
	void Recalculation();
};