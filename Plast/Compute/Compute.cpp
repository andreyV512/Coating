#include "Compute.h"
#include "Data/Data.h"
#include "App/AppBase.h"
#include <math.h> 
#include "MessageText/status.h"
#include "tools_debug/DebugMess.h"
#include "Windows/MainWindow/MainWindow.h"
#include "window_tool/Emptywindow.h"
#include "Log/LogBuffer.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "SetTresholds.hpp"
#include "InitFiltre.hpp"

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
	, result(Singleton<Data::ResultData>::Instance())
{
	VL::foreach<VL::CreateNumList< Data::Sensor, 0, App::count_sensors - 1>::Result, __compute_set_data__>()(sensorData);
	//packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	//numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
}



void Compute::Start()
{
	//packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	//numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	framesCount = strobesTickCount = offsetsTickCount = zoneOffsetsIndex = 0;
	zoneOffsetsIndex = zoneOffsetsIndexStart = 0;

	//Wrap x(filter, Singleton<FiltersTable>::Instance().items);
	//__init_filtre__()(x);
	SetLan(*this, Singleton<LanParametersTable>::Instance().items);
	SetFilters(*this, Singleton<FiltersTable>::Instance().items);

	SetMedian(*this, Singleton<MedianFiltreTable>::Instance().items);
	SetTresholds(*this, Singleton<TresholdsTable>::Instance().items);

	//auto &deadZones = Singleton<DeadZonesTable>::Instance().items;
	//int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	//int deadZoneStop = deadZones.get<DeadZoneStop>().value;
	//
	//wholeStart = deadZoneStart / App::size_zone_mm;
	//wholeStop = deadZoneStop / App::size_zone_mm;
	//
	//fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	//fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;
	SetDeadZones(*this, Singleton<DeadZonesTable>::Instance().items);
}

#define MAX(a, b) (a) > (b) ? (a): (b)
#define MIN(a, b) (a) < (b) ? (a): (b)


int deugcount = 0;

bool Compute::Strobes()
{
	dprint("%d ", deugcount++);
	unsigned strobesStop = data.strobesTickCount;
	if (strobesStop == strobesTickCount || zoneOffsetsIndex >= App::count_zones) return false;
	//количество кадров в зоне
	unsigned offsetsTickStop = data.offsetsTickCount;
	unsigned offsetTickStart = offsetsTickCount;
	for (unsigned i = strobesTickCount; i < strobesStop; ++i)
	{
		unsigned tickStrobes = data.strobesTick[i];
		while (data.offsetsTick[offsetTickStart] < tickStrobes) ++offsetTickStart;
		zoneOffsets[zoneOffsetsIndexStart] = offsetTickStart * packetSize * numberPackets;
		if(zoneOffsetsIndexStart < dimention_of(zoneOffsets) - 1)++zoneOffsetsIndexStart;
	}

	if (zoneOffsetsIndexStart - (wholeStart + wholeStop + 1) < 0)
	{
		zoneOffsetsIndexStart = 0;
		return false;
	}

	strobesTickCount = strobesStop;

	const int inc = packetSize * App::count_sensors;

	for (int i = zoneOffsetsIndex; i < zoneOffsetsIndexStart && i < App::count_zones; ++i)
	{
		if (0 == i) continue;

		unsigned start = zoneOffsets[i - 1];
		unsigned stop = zoneOffsets[i];

		start /= inc;
		start *= inc;

		stop /= inc;
		stop *= inc;

		int borderStop = zoneOffsetsIndexStart - wholeStop - 1;

		if (i <= 1 + wholeStart || i > borderStop)
		{
			if (i == 1 + wholeStart)
			{
				start += int((stop - start) * fractionalStart);
				start /= inc;
				start *= inc;
			}
			else if (i == borderStop)
			{
				stop -= int((stop - start) * (1.0 - fractionalStop));
				stop /= inc;
				stop *= inc;
			}
			else
			{
				for (int sens = 0; sens < App::count_sensors; ++sens)
				{
					sensorData[sens]->data[i - 1] = 80;
					sensorData[sens]->status[i - 1] = VL::IndexOf<zone_status_list, DeadZone>::value;
					sensorData[sens]->count = i;
				}
				continue;
			}
		}
		
		for (unsigned sens = 0; sens < App::count_sensors; ++sens)
		{
			Zone(sens
				, &data.buffer[start + sens * packetSize]
				, &data.buffer[stop + sens * packetSize]
				, sensorData[sens]->data[i - 1]
				, sensorData[sens]->status[i - 1]
			);
			sensorData[sens]->count = i;
		}
	}

	for (int i = zoneOffsetsIndex; i < zoneOffsetsIndexStart; ++i)
	{
		result.status[i] = StatusData::Compute(
			StatusData::Compute(sensorData[0]->status[i], sensorData[1]->status[i])
			, sensorData[2]->status[i]
		);
		double _0 = sensorData[0]->data[i];
		double _1 = sensorData[1]->data[i];
		double _2 = sensorData[2]->data[i];
		
		result.minData[i] = MIN(MIN(_0, _1), _2);
		result.maxData[i] = MAX(MAX(_0, _1), _2);
	}
	
	result.count = zoneOffsetsIndexStart - 1;

	zoneOffsetsIndex = zoneOffsetsIndexStart - wholeStop - 1;
	return true;
}

#undef MAX
#undef MIN

void Compute::Zone(int zone, int sens)
{
	auto m = median[sens];
	double ldata[App::count_sensors] = {};
	char lstatus[App::count_sensors] = {};
	const int inc = packetSize * App::count_sensors;
	int start = zoneOffsets[zone - 1];
	int stop = zoneOffsets[zone - 0];
	if (1 + wholeStart > zone) 
	{
		for (int i = 0; i < App::count_sensors; ++i)
		{
			sensorData[i]->data[zone - 1] = 80;
			sensorData[i]->status[zone - 1] = VL::IndexOf<zone_status_list, DeadZone>::value;
			sensorData[i]->count = zone;
		}
		return;
	}
	else if (2 + wholeStart == zone)
	{
		start += int((stop - start) * fractionalStart);
		start /= inc;
		start *= inc;
	}
	for (int i = start; i < stop; i += inc)
	{
		for (int j = 0; j < App::count_sensors; ++j)
		{
			double result;
			char status;
			ComputeFrame(sens, &data.buffer[i + j * packetSize], result, status);
			double t = (m.*medianProc)(result, status);
			if (t > ldata[j])
			{
				ldata[j] = t;
				lstatus[j] = status;
			}
		}
	}
	for (int i = 0; i < App::count_sensors; ++i)
	{
		sensorData[i]->data[zone - 1] = ldata[i];
		sensorData[i]->status[zone - 1] = lstatus[i];
		sensorData[i]->count = zone;
	}
}

void Compute::Zone(int sens, char *start, char *stop, double &result, char &status)
{
	const int inc = packetSize * App::count_sensors;
	auto m = median[sens];
	result = 0;
	for (char *i = start; i < stop; i += inc)
	{
		//ComputeFrame((IDSPFlt &)filter, i, result, status);
		ComputeFrame(sens, i, result, status);
		double t = (m.*medianProc)(result, status);
		if (t > result)
		{
			result = t;
			status = status;
		}
	}
}

void Compute::ComputeFrame(int sens, char *d, double &value, char &status)
{
	IDSPFlt &f = (IDSPFlt &)filter[sens];
	f.Clean();
	status = StatusData::norm;
	value = 0;
	unsigned i = 0;
	for (; i < offsAlarmStart[sens]; ++i)
	{
		f(d[i] * 100.0 / 128);
	}
	double gain = gainAlarmOffs[sens];
	for (; i < offsAlarmStop[sens]; ++i)
	{
		double t = f(d[i] * 100.0 / 128);
		t *= gain;
		if(value < t) value = t;
		if(t > threshAlarm[sens])
		{
			status = StatusData::defect;
		}
		gain += gainAlarmDelta[sens];
	}

	if (!bottomReflectionOn[sens]) return;
	for (; i < offsReflectionStart[sens]; ++i)
	{
		f(d[i] * 100.0 / 128);		
	}

	gain = gainReflectionOffs[sens];
	bool refl = true;
	for (; i < offsReflectionStop[sens]; ++i)
	{
		double t = f(d[i] * 100.0 / 128);
		t *= gain;
		if (t > threshReflection[sens])
		{
			refl = false;
			break;
		}
		gain += gainReflectionDelta[sens];
	}
	if(refl)status = StatusData::Compute(status, StatusData::noBottomReflection);
}

void Compute::Update()
{
	QueueUserWorkItem(func<Compute, &Compute::__Update__>, (LPVOID)this, WT_EXECUTEDEFAULT);
}

void Compute::Recalculation()
{
	Log::Mess <LogMess::Recalculation>();
	MainWindow::EnableMenu(false);
	AppKeyHandler::DisableAll();

	framesCount = strobesTickCount = offsetsTickCount = zoneOffsetsIndex = 0;
	zoneOffsetsIndex = zoneOffsetsIndexStart = 0;

	QueueUserWorkItem(func<Compute, &Compute::__Recalculation__>, (LPVOID)this, WT_EXECUTEDEFAULT);
}

void Compute::__Update__()
{
	if (Strobes())
	{
		RepaintWindow<MainWindow>();
	}
}

void Compute::__Recalculation__()
{
	if (Strobes())
	{
		RepaintWindow<MainWindow>();
		MainWindow::EnableMenu(true);
		AppKeyHandler::Stop();
		Log::Mess <LogMess::RecalculationStop>();
	}
}

