#pragma once
#include "MedianFiltre/MedianFiltre.h"
#include "Compute/InitFiltre.hpp"

template<class Owner, class Tresh>struct __set_tresholds_data__
{
	Owner &o;
	Tresh &t;
	__set_tresholds_data__(Owner &o, Tresh &t) : o(o), t(t) {}
};

template<class O, class P>struct __set_tresholds__
{
	void operator()(P &p)
	{
		static const int N = O::value;
		p.o.threshAlarm[N] = p.t.get<Num<AlarmThresh, N>>().value;
		p.o.offsAlarmStart[N] = int(p.t.get<Num<AlarmThreshStart, N>>().value * p.o.packetSize * 0.01);
		p.o.offsAlarmStop[N] = int(p.t.get<Num<AlarmThreshStop, N>>().value * p.o.packetSize * 0.01);
		p.o.gainAlarmOffs[N] = p.t.get<Num<AlarmGainStart, N>>().value;
		p.o.gainAlarmDelta[N] = (p.t.get<Num<AlarmGainStop, N>>().value - p.t.get<Num<AlarmGainStart, N>>().value)
			/ (p.o.offsAlarmStop[N] - p.o.offsAlarmStart[N]);

		p.o.threshReflection[N] = p.t.get<Num<BottomReflectionThresh, N>>().value;
		p.o.offsReflectionStart[N] = int(p.t.get<Num<BottomReflectionThreshStart, N>>().value * p.o.packetSize * 0.01);
		p.o.offsReflectionStop[N] = int(p.t.get<Num<BottomReflectionThreshStop, N>>().value * p.o.packetSize * 0.01);
		p.o.gainReflectionOffs[N] = p.t.get<Num<BottomReflectionGainStart, N>>().value;
		p.o.gainReflectionDelta[N] = (p.t.get<Num<BottomReflectionGainStop, N>>().value - p.t.get<Num<BottomReflectionGainStart, N>>().value)
			/ (p.o.offsReflectionStop[N] - p.o.offsReflectionStart[N]);
		p.o.bottomReflectionOn[N] = p.t.get<Num<BottomReflectionOn, N>>().value;
	}
};

template<class Owner> void SetParam(Owner &o, TresholdsTable::TItems &t)
{
	__set_tresholds_data__<Owner, TresholdsTable::TItems>data(o, t);
	VL::foreach<typename VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __set_tresholds__>()(data);
}

template<class T, class Ret, class ...P>void set_median_proc(bool b, Ret(T:: *&res)(P...), Ret(T:: *t)(P...), Ret(T:: *f)(P...)) { res = b ? t : f; }
template<class O, class P>struct __set_median__
{
	void operator()(P &p)
	{
		static const int N = O::value;
		bool b = p.t.get<Num<MedianFiltreON, N>>().value;
		set_median_proc(b, p.o.medianProc, &MedianFiltre::Val, &MedianFiltre::noop);
		set_median_proc(b, p.o.medianProc_stat, &ChMedFlt::Val, &ChMedFlt::noop);
		int width = p.t.get<Num<MedianFiltreWidth, N>>().value;
		p.o.median[N].InitWidth(width);
		p.o.median_stat[N].InitWidth(width);
	}
};

template<class O, class P>struct __set_median_once__
{
	bool operator()(P &p)
	{
		static const int N = O::value;
		if (N == p.sens)
		{
			if (p.o.amplFilt)
			{
				set_median_proc(p.t.get<Num<MedianFiltreON, N>>().value, p.o.medianProc, &MedianFiltre::Val, &MedianFiltre::noop);
			}
			else
			{
				set_median_proc(p.t.get<Num<MedianFiltreON, N>>().value, p.o.medianProc, &MedianFiltre::Stat, &MedianFiltre::noop);
			}
			int width = p.t.get<Num<MedianFiltreWidth, N>>().value;
			p.o.median.InitWidth(width);
			return false;
		}
		return true;
	}
};

template<class Owner>void SetParam(Owner &o, MedianFiltreTable::TItems &t)
{
	__set_tresholds_data__<Owner, MedianFiltreTable::TItems>data(o, t);
	VL::foreach<typename VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __set_median__>()(data);
}

template<class Owner>void SetParam(Owner &o, DeadZonesTable::TItems &deadZones)
{
	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	o.wholeStart = deadZoneStart / App::size_zone_mm;
	o.wholeStop = deadZoneStop / App::size_zone_mm;

	o.fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	o.fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;
}

struct __wrap_filters__
{
	Impl<DSPFltDump, 1032>(&filter)[App::count_sensors];
	FiltersTable::TItems &paramFlt;
	unsigned frequency;
	__wrap_filters__(Impl<DSPFltDump, 1032>(&filter)[App::count_sensors], unsigned frequency, FiltersTable::TItems &paramFlt)
		: filter(filter)
		, paramFlt(paramFlt)
		, frequency(frequency)
	{}
};

template<class Owner>void SetParam(Owner &o, FiltersTable::TItems &items)
{
	__wrap_filters__ x(o.filter, o.frequency, items);
	__init_filtre__()(x);
}

template<class Owner>void SetParam(Owner &o, LanParametersTable::TItems &items)
{
	o.packetSize = items.get<PacketSize>().value;
	o.numberPackets = items.get<NumberPackets>().value;
	o.frequency = 1000000 *  items.get<Frequency>().value;
}
