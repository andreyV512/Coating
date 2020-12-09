#include "ComputeFrame.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"
#include "SetTresholds.hpp"
#include "InitFiltre.hpp"

//template<class O, class P>struct __init_filtre_XX__
//{
//	bool operator()(P &p)
//	{
//		if (VL::IndexOf<filters_list, O>::value == p.paramFlt.get<Num<CurrentFilter, P::NUM>>().value)
//		{
//			p.filter[P::NUM].Init<O>();
//			SetupFiltre<O, P::NUM>()(
//				(O &)p.filter
//				, p.paramFlt
//				, p.frequency
//				);
//			return false;
//		}
//		return true;
//	}
//};

ComputeFrame::ComputeFrame()
	: frequency(1000000 * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, framesCount(Singleton<Data::InputData>::Instance().framesCount)
	, buffer(Singleton<Data::InputData>::Instance().buffer)
	, bipolar(false)
{
	VL::CopyFromTo(Singleton<FiltersTable>::Instance().items, paramFlt);
	UpdateFiltre();
	VL::CopyFromTo(Singleton< TresholdsTable>::Instance().items, treshItems);
	SetTresholds(*this, treshItems);
}

void ComputeFrame::UpdateFiltre()
{
	__init_filtre__()(*this);
}

void ComputeFrame::Frame(int sensor, unsigned offs_, double *data)
{
	auto &f = filter[sensor];
	f->Clean();

	unsigned offs = offs_;
		unsigned i = 0;
		for (; i < offsAlarmStart[sensor] && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = 100.0 * buffer[offs];
			data[i] = (*f)(t / 128);
		}
		double gain = gainAlarmOffs[sensor];
		for (; i < offsAlarmStop[sensor] && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = gain * 100.0 * buffer[offs];
			data[i] = (*f)(t / 128);
			gain += gainAlarmDelta[sensor];
		}

		if (bottomReflectionOn)
		{
			for (; i < offsReflectionStart[sensor] && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = 100.0 * buffer[offs];
				data[i] = (*f)(t / 128);
			}
			gain = gainReflectionOffs[sensor];
			for (; i < offsReflectionStop[sensor] && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = gain * 100.0 * buffer[offs];
				data[i] = (*f)(t / 128);
				gain += gainReflectionDelta[sensor];
			}
		}
		else
			for (; i < packetSize && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = 100.0 * buffer[offs];
				data[i] = (*f)(t / 128);
			}
	if (!bipolar)
	{
		offs = offs_;
		for (unsigned i = 0; i < packetSize && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			if (data[i] < 0)
			{
				data[i] = -data[i];
			}
		}
	}
}
