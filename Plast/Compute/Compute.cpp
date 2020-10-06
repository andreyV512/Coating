#include "Compute.h"
#include "Data/Data.h"
#include "App/AppBase.h"
#include <math.h> 
#include "MessageText/status.h"
#include "tools_debug/DebugMess.h"

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

template<class O, class P>struct __init_filtre_X__
{
	bool operator()(P &p)
	{
		FiltersTable::TItems &items = Singleton<FiltersTable>::Instance().items;
		if (VL::IndexOf<filters_list, O>::value == items.get<CurrentFilter>().value)
		{
			p.Init<O>();
			SetupFiltre<O>()(
				(O &)p
				, items
				, 1000000 * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value
				);
			dprint("filtre %d %d %s"
				, VL::IndexOf<filters_list, O>::value
				, items.get<CurrentFilter>().value
				, typeid(O).name());
			return false;
		}
		return true;
	}
};

void Compute::Start()
{
	packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	framesCount = strobesTickCount = offsetsTickCount = zoneOffsetsIndex = 0;

	if (VL::find<filters_list, __init_filtre_X__>()(filter)) filter.Init<IDSPFlt>();

	auto medianParams = Singleton<MedianFiltreTable>::Instance().items;
	medianProc = medianParams.get<MedianFiltreON>().value ? &MedianFiltre::Val: &MedianFiltre::noop;
	int width = medianParams.get<MedianFiltreWidth>().value;
	for (int i = 0; i < dimention_of(median); ++i) median[i].InitWidth(width);

	auto t = Singleton<TresholdsTable>::Instance().items;

	threshAlarm    = t.get<AlarmThresh>().value;
	offsAlarmStart = int(t.get<AlarmThreshStart>().value * packetSize * 0.01);
	offsAlarmStop  = int(t.get<AlarmThreshStop>().value * packetSize * 0.01) ;
	gainAlarmOffs = t.get<AlarmGainStart>().value;
	gainAlarmDelta = (t.get<AlarmGainStart>().value - t.get<AlarmGainStop>().value) 
		/ ((_int64)offsAlarmStop - offsAlarmStart);

	threshReflection    = t.get<BottomReflectionThresh>().value;
	offsReflectionStart = int(t.get<BottomReflectionThreshStart>().value * packetSize * 0.01);
	offsReflectionStop  = int(t.get<BottomReflectionThreshStop>().value * packetSize * 0.01);
	gainReflectionOffs = t.get<BottomReflectionGainStart>().value;
	gainReflectionDelta = (t.get<BottomReflectionGainStart>().value - t.get<BottomReflectionGainStop>().value) 
		/ ((_int64)offsReflectionStop - offsReflectionStart);
	bottomReflectionOn = t.get<BottomReflectionOn>().value;
}

bool Compute::Strobes()
{
	unsigned frameStop = data.framesCount;
	if (frameStop == framesCount) return false;
	//вычисление превышения порога брака в кадре
	unsigned start = GetTickCount();
	dprint("compute start \n");
	for (unsigned i = framesCount; i < frameStop; i+= packetSize)
	{
		double result = 0;
		char status = 0;
		int frame = i / packetSize;
		int sensor = frame % App::count_sensors;
		int zone = frame / App::count_sensors;
	ComputeFrame((IDSPFlt &)filter, sensor, &data.buffer[i], result, status);
		data.result[sensor][zone] = result;
		data.status[sensor][zone] = status;
	}
	framesCount = frameStop;
	unsigned stop = GetTickCount();
	dprint("compute stop %d\n", stop - start);

	unsigned strobesStop = data.strobesTickCount;
	if (strobesStop == strobesTickCount) return false;
	//количество кадров в зоне
	unsigned offsetsTickStop = data.offsetsTickCount;
	unsigned offsetTickStart = offsetsTickCount;
	unsigned zoneOffsetsIndexStart = zoneOffsetsIndex;
	for (unsigned i = strobesTickCount; i < strobesStop; ++i)
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

void Compute::ComputeFrame(IDSPFlt &f, int sensor, char *d, double &value, char &status)
{
	f.Clean();
	status = StatusData::norm;
	value = 0;
	int i = 0;
	for (; i < offsAlarmStart; ++i)
	{
		f(d[i]);
	}
	double gain = gainAlarmOffs;
	for (; i < offsAlarmStop; ++i)
	{
		double t = f(d[i]);
		t *= gain;
		if(value < t) value = t;
		if(t > threshAlarm)
		{
			status = StatusData::defect;
			//break;
		}
		gain += gainAlarmDelta;
	}

	if (!bottomReflectionOn) return;
	for (; i < offsReflectionStart; ++i)
	{
		f(d[i]);		
	}

	gain = gainReflectionOffs;
	bool refl = true;
	for (; i < offsReflectionStop; ++i)
	{
		double t = f(d[i]);
		t *= gain;
		if (t > threshReflection)
		{
			refl = false;
			break;
		}
		gain += gainReflectionDelta;
	}
	if(refl)status = StatusData::Compute(status, StatusData::noBottomReflection);
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

void Compute::Recalculation()
{
	Start();
	framesCount = 0;
	offsetsTickCount = 0;
	if (Strobes())
	{

	}
}
