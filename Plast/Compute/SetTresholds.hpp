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

template<class Owner, class Tresh> void SetTresholds(Owner &o, Tresh &t)
{
	__set_tresholds_data__<Owner, Tresh>data(o, t);
	VL::foreach<typename VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __set_tresholds__>()(data);
}

template<class T, class ...P>void set_median_proc(bool b, double(T:: *&res)(P...), double(T:: *t)(P...), double(T:: *f)(P...)) { res = b ? t : f; }
template<class O, class P>struct __set_median__
{
	void operator()(P &p)
	{
		static const int N = O::value;
		set_median_proc(p.t.get<Num<MedianFiltreON, N>>().value, p.o.medianProc, &MedianFiltre::Val, &MedianFiltre::noop);
		int width = p.t.get<Num<MedianFiltreWidth, N>>().value;
		p.o.median[N].InitWidth(width);
	}
};

template<class O, class P>struct __set_median_once__
{
	bool operator()(P &p)
	{
		static const int N = O::value;
		if (N == p.sens)
		{
			set_median_proc(p.t.get<Num<MedianFiltreON, N>>().value, p.o.medianProc, &MedianFiltre::Val, &MedianFiltre::noop);
			int width = p.t.get<Num<MedianFiltreWidth, N>>().value;
			p.o.median.InitWidth(width);
			return false;
		}
		return true;
	}
};

template<class Owner, class Tresh>void SetMedian(Owner &o, Tresh &t)
{
	__set_tresholds_data__<Owner, Tresh>data(o, t);
	VL::foreach<typename VL::CreateNumList<VL::IntToType, 0, App::count_sensors - 1>::Result, __set_median__>()(data);
}

template<class Owner>void SetDeadZones(Owner &o, DeadZonesTable::TItems &deadZones)
{
	//auto &deadZones = Singleton<DeadZonesTable>::Instance().items;
	int deadZoneStart = deadZones.get<DeadZoneStart>().value;
	int deadZoneStop = deadZones.get<DeadZoneStop>().value;

	o.wholeStart = deadZoneStart / App::size_zone_mm;
	o.wholeStop = deadZoneStop / App::size_zone_mm;

	o.fractionalStart = double(deadZoneStart % App::size_zone_mm) / App::size_zone_mm;
	o.fractionalStop = double(deadZoneStop % App::size_zone_mm) / App::size_zone_mm;
}

struct __wrap_filters__
{
	Impl<IDSPFlt, 1032>(&filter)[App::count_sensors];
	FiltersTable::TItems &paramFlt;
	unsigned frequency;
	__wrap_filters__(Impl<IDSPFlt, 1032>(&filter)[App::count_sensors], unsigned frequency, FiltersTable::TItems &paramFlt)
		: filter(filter)
		, paramFlt(paramFlt)
		, frequency(frequency)//1000000 * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value)
	{}
};

template<class Owner>void SetFilters(Owner &o, FiltersTable::TItems &items)
{
	__wrap_filters__ x(o.filter, o.frequency, items);
	__init_filtre__()(x);
}

template<class Owner>void SetLan(Owner &o, LanParametersTable::TItems &items)
{
	o.packetSize = items.get<PacketSize>().value;
	o.numberPackets = items.get<NumberPackets>().value;
	o.frequency = items.get<Frequency>().value;
}
