#include "Compute.h"
#include "Data/Data.h"
#include "App/AppBase.h"
#include <math.h> 

template<class O, class P>struct __compute_set_data__;
template<template<int>class W, int N, class P>struct __compute_set_data__<W<N>, P>
{
	void operator()(P &p)
	{
		p[N] = &Singleton<W<N>>::Instance().data;
	}
};

Compute::Compute()
	: data(Singleton<Data::InputData>::Instance())
{
	VL::foreach<VL::CreateNumList< Data::Sensor, 0, 2>::Result, __compute_set_data__>()(sensorData);
}

void Compute::Start()
{
	packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	framesCount = strobesTickCount = offsetsTickCount = zoneOffsetsIndex = 0;

	auto &params = Singleton<FiltersTable>::Instance().items;
	for (int i = 0; i < dimention_of(filters); ++i)
	{
		filters[i] = &dspFilt;
		__init_filtre_data__ data(filters[i], params);
		VL::foreach<filters_list, __init_filtre__>()(factoryFilters[i], data);
	}

	auto medianParams = Singleton<MedianFiltreTable>::Instance().items;
	medianProc = medianParams.get<MedianFiltreON>().value ? &MedianFiltre::Val: &MedianFiltre::noop;
	int width = medianParams.get<MedianFiltreWidth>().value;
	for (int i = 0; i < dimention_of(median); ++i) median[i].InitWidth(width);

	auto t = Singleton<TresholdsTable>::Instance().items;

	threshAlarm    = t.get<AlarmThresh>().value;
	offsAlarmStart = t.get<AlarmThreshStart>().value;
	offsAlarmStop  = t.get<AlarmThreshStop>().value;
	gainAlarmOffs  = t.get<AlarmGainStart>().value - t.get<AlarmGainStop>().value;
	gainAlarmDelta = gainAlarmOffs / ((_int64)offsAlarmStop - offsAlarmStart);

	threshReflection    = t.get<BottomReflectionThresh>().value;
	offsReflectionStart = t.get<BottomReflectionThreshStart>().value;
	offsReflectionStop  = t.get<BottomReflectionThreshStop>().value;
	gainReflectionOffs  = t.get<BottomReflectionGainStart>().value - t.get<BottomReflectionGainStop>().value;
	gainReflectionDelta = gainReflectionOffs / ((_int64)offsReflectionStop - offsReflectionStart);
}

bool Compute::Strobes()
{
	unsigned frameStop = data.framesCount;
	if (frameStop == framesCount) return false;
	//вычисление превышения порога брака в кадре
	for (unsigned i = framesCount; i < frameStop; i+= packetSize)
	{
		double result = 0;
		char status = 0;
		int sensor = i % App::count_zones;
		int zone = i / App::count_zones;
		ComputeFrame(sensor, &data.buffer[i], result, status);
		data.result[sensor][zone] = result;
		data.status[sensor][zone] = status;
	}
	framesCount = frameStop;

	unsigned strobesStop = data.strobesTickCount;
	if (strobesStop == strobesTickCount) return false;
	//количество кадров в зоне
	unsigned offsetsTickStop = data.offsetsTickCount;
	unsigned offsetTickStart = offsetsTickCount;
	unsigned zoneOffsetsIndexStart = zoneOffsetsIndex;
	for (unsigned i = strobesTickCount; i < frameStop; ++i)
	{
		unsigned tickStrobes = data.strobesTick[i];
		while (data.offsetsTick[offsetTickStart] < tickStrobes) ++offsetTickStart;
		zoneOffsets[zoneOffsetsIndexStart] = offsetTickStart * packetSize * numberPackets;
		++zoneOffsetsIndexStart;
	}

	for (int i = zoneOffsetsIndex; i < (int)zoneOffsetsIndexStart; ++i)
	{
		if (0 == i) continue;
		for (int sens = 0; sens < App::count_sensors; ++sens)  Zone(i, sens);
	}

	zoneOffsetsIndex = zoneOffsetsIndexStart;
	return true;
}

void Compute::Zone(int zone, int sens)
{
	int startZone = zone - 1;
	auto m = median[sens];
	double *dt = data.result[sens];
	char   *st = data.status[sens];
	char status = 0;
	double ldata = 0;
	char lstatus = 0;
	for (int i = data.strobesTick[startZone], end = data.strobesTick[zone]; i < end; ++i)
	{
		//double t = round(dt[i] * 100);
		//t *= 0.01;
		char status = st[i];
		double t = (m.*medianProc)(dt[i], status);
		if (t > ldata)
		{
			ldata = t;
			lstatus = status;
		}
	}
	sensorData[sens]->data[zone]   = ldata;
	sensorData[sens]->status[zone] = lstatus;
	sensorData[sens]->count        = zone;
}

void Compute::ComputeFrame(int sensor, char *d, double &value, char &status)
{
	status = 0;
	auto f = filters[sensor];
	int i = 0;
	for (; i < offsAlarmStart; ++i)
	{
		(*f)(d[i]);
	}
	double gain = gainAlarmOffs;
	for (; i < offsAlarmStop; ++i)
	{
		double t = (*f)(d[i]);
		t *= gain;
		if(t > threshAlarm)
		{

		}
		gain += gainAlarmDelta;
	}
}

void Compute::ComputeZone(int zone)
{
	//double result[App::count_sensors][sensorBuffSize];
	//char status[App::count_sensors][sensorBuffSize];
	//TODO ...
}

void Compute::Update()
{
	if (Strobes())
	{
		//Frames();
	}
}

void Compute::Done()
{
}
