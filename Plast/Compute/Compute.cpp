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
#include "Data/StoreAllParam.h"

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
	framesCount = strobesTickCount = offsetsTickCount = zoneOffsetsIndex = 0;
	zoneOffsetsIndex = zoneOffsetsIndexStart = 0;

	SetParam(*this, Singleton<LanParametersTable>::Instance().items);
	SetParam(*this, Singleton<FiltersTable>::Instance().items);
	SetParam(*this, Singleton<MedianFiltreTable>::Instance().items);
	SetParam(*this, Singleton<TresholdsTable>::Instance().items);
	SetParam(*this, Singleton<DeadZonesTable>::Instance().items);
	Singleton< ALLPatrams>::Instance().SetParams();
}

#define MAX(a, b) (a) > (b) ? (a): (b)
#define MIN(a, b) (a) < (b) ? (a): (b)


//int deugcount = 0;

bool Compute::Strobes()
{
	//dprint(" st %d ", deugcount++);
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
	status = StatusData::norm;
	char tstatus;
	double t;
	for (char *i = start; i < stop; i += inc)
	{
		ComputeFrame(sens, i, t, tstatus);
		t = (m.*medianProc)(t, tstatus);
		if (t > result)
		{
			result = t;	
			status = StatusData::Compute(status, tstatus);
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
		if (t < 0) t = -t;
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
		if (t < 0) t = -t;
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

	auto &items = Singleton<ALLPatrams>::Instance().items;

	SetParam(*this, items.get<VL::Factory<LanParametersTable::items_list>>());
	SetParam(*this, items.get<VL::Factory<FiltersTable      ::items_list>>());
	SetParam(*this, items.get<VL::Factory<MedianFiltreTable ::items_list>>());
	SetParam(*this, items.get<VL::Factory<TresholdsTable    ::items_list>>());
	SetParam(*this, items.get<VL::Factory<DeadZonesTable    ::items_list>>());

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
		auto &items = Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>();
		dprint("1 start offset %x %f %f %f\n"
			, Singleton<ALLPatrams>::Instance()
			, items.get< Num<AlarmThreshStart, 0>>().value
			, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 1>>().value
			, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 2>>().value
		);
		RepaintWindow<MainWindow>();
		MainWindow::EnableMenu(true);
		AppKeyHandler::Stop();
		Log::Mess <LogMess::RecalculationStop>();

		dprint("2 start offset %f %f %f\n"
			, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 0>>().value
			, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 1>>().value
			, Singleton<ALLPatrams>::Instance().items.get<VL::Factory<TresholdsTable::items_list>>().get< Num<AlarmThreshStart, 2>>().value
		);
	}
}

