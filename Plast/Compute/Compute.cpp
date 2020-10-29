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
	VL::foreach<VL::CreateNumList< Data::Sensor, 0, 2>::Result, __compute_set_data__>()(sensorData);
	packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
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

	if (VL::find<filters_list, __init_filtre_X__>()(filter)) filter.Init<DSPFltDump>();

	auto medianParams = Singleton<MedianFiltreTable>::Instance().items;
	medianProc = medianParams.get<MedianFiltreON>().value ? &MedianFiltre::Val: &MedianFiltre::noop;
	int width = medianParams.get<MedianFiltreWidth>().value;
	for (int i = 0; i < dimention_of(median); ++i) median[i].InitWidth(width);

	auto t = Singleton<TresholdsTable>::Instance().items;

	threshAlarm    = t.get<AlarmThresh>().value;
	offsAlarmStart = int(t.get<AlarmThreshStart>().value * packetSize * 0.01);
	offsAlarmStop  = int(t.get<AlarmThreshStop>().value * packetSize * 0.01) ;
	gainAlarmOffs = t.get<AlarmGainStart>().value;
	gainAlarmDelta = (t.get<AlarmGainStop>().value - t.get<AlarmGainStart>().value) 
		/ (offsAlarmStop - offsAlarmStart);

	threshReflection    = t.get<BottomReflectionThresh>().value;
	offsReflectionStart = int(t.get<BottomReflectionThreshStart>().value * packetSize * 0.01);
	offsReflectionStop  = int(t.get<BottomReflectionThreshStop>().value * packetSize * 0.01);
	gainReflectionOffs = t.get<BottomReflectionGainStart>().value;
	gainReflectionDelta = (t.get<BottomReflectionGainStop>().value - t.get<BottomReflectionGainStart>().value) 
		/ (offsReflectionStop - offsReflectionStart);
	bottomReflectionOn = t.get<BottomReflectionOn>().value;

	auto &deadZones = Singleton<DeadZonesTable>::Instance().items;
	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	wholeStart = deadZoneStart / App::size_zone_mm;
	wholeStop = deadZoneStop / App::size_zone_mm;

	fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;
}

#define MAX(a, b) a > b ? a: b
#define MIN(a, b) a < b ? a: b

bool Compute::Strobes()
{
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

	const int inc = packetSize * App::count_sensors;


	if (zoneOffsetsIndex > 0)
	{
		unsigned i = zoneOffsetsIndex;
		unsigned start = zoneOffsets[i - 1];
		start /= inc;
		start *= inc;
		start -= 7 * inc;
		int stop = start;

		double ldata;
		char lstatus;

		for (int sens = 0; sens < App::count_sensors; ++sens)
		{
			Zone(sens
				, &data.buffer[start + sens * packetSize]
				, &data.buffer[stop + sens * packetSize]
				, ldata
				, lstatus
			);
		}
	}

	for (unsigned i = zoneOffsetsIndex; i < zoneOffsetsIndexStart; ++i)
	{
		if (0 == i) continue;

		unsigned start = zoneOffsets[i - 1];
		unsigned stop = zoneOffsets[i];

		start /= inc;
		start *= inc;

		stop /= inc;
		stop *= inc;

		unsigned borderStop = zoneOffsetsIndexStart - wholeStop - 1;

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
		
		for (int sens = 0; sens < App::count_sensors; ++sens)
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

	for (int i = zoneOffsetsIndex; i < (int)zoneOffsetsIndexStart; ++i)
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
			ComputeFrame((IDSPFlt &)filter, &data.buffer[i + j * packetSize], result, status);
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
		ComputeFrame((IDSPFlt &)filter, i, result, status);
		double t = (m.*medianProc)(result, status);
		if (t > result)
		{
			result = t;
			status = status;
		}
	}
}

void Compute::ComputeFrame(IDSPFlt &f, char *d, double &value, char &status)
{
	f.Clean();
	status = StatusData::norm;
	value = 0;
	int i = 0;
	for (; i < offsAlarmStart; ++i)
	{
		f(d[i] * 100.0 / 128);
	}
	double gain = gainAlarmOffs;
	for (; i < offsAlarmStop; ++i)
	{
		double t = f(d[i] * 100.0 / 128);
		t *= gain;
		if(value < t) value = t;
		if(t > threshAlarm)
		{
			status = StatusData::defect;
		}
		gain += gainAlarmDelta;
	}

	if (!bottomReflectionOn) return;
	for (; i < offsReflectionStart; ++i)
	{
		f(d[i] * 100.0 / 128);		
	}

	gain = gainReflectionOffs;
	bool refl = true;
	for (; i < offsReflectionStop; ++i)
	{
		double t = f(d[i] * 100.0 / 128);
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

