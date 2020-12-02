#include "ComputeFrame.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"
#include "SetTresholds.hpp"

template<class O, class P>struct __init_filtre_XX__
{
	bool operator()(P &p)
	{
		if (VL::IndexOf<filters_list, O>::value == p.paramFlt.get<CurrentFilter>().value)
		{
			p.filter.Init<O>();
			SetupFiltre<O>()(
				(O &)p.filter
				, p.paramFlt
				, p.frequency
				);
			return false;
		}
		return true;
	}
};

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
	if(VL::find<filters_list, __init_filtre_XX__>()(*this)) filter.Init<DSPFltDump>();
}

void ComputeFrame::Frame(int sensor, unsigned offs_, double *data)
{
	//unsigned num = *(unsigned *)&buffer[offs_];
	//dprint("frame num %d sens %d %d\n", num, num % 3, sensor);

	filter->Clean();

	unsigned offs = offs_;
		unsigned i = 0;
		for (; i < offsAlarmStart && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = 100.0 * buffer[offs];
			data[i] = (*filter)(t / 128);
		}
		double gain = gainAlarmOffs;
		for (; i < offsAlarmStop && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = gain * 100.0 * buffer[offs];
			data[i] = (*filter)(t / 128);
			gain += gainAlarmDelta;
		}

		if (bottomReflectionOn)
		{
			for (; i < offsReflectionStart && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = 100.0 * buffer[offs];
				data[i] = (*filter)(t / 128);
			}
			gain = gainReflectionOffs;
			for (; i < offsReflectionStop && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = gain * 100.0 * buffer[offs];
				data[i] = (*filter)(t / 128);
				gain += gainReflectionDelta;
			}
		}
		else
			for (; i < packetSize && offs < Data::InputData::buffSize; ++i, ++offs)
			{
				double t = 100.0 * buffer[offs];
				data[i] = (*filter)(t / 128);
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
