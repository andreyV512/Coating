#include "ComputeFrame.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"

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
}

void ComputeFrame::UpdateFiltre()
{
	if(VL::find<filters_list, __init_filtre_XX__>()(*this)) filter.Init<DSPFltDump>();
}

void ComputeFrame::Frame(int sensor, __int64 offs, double *data)
{

	offs *= packetSize * App::count_sensors;
	offs += sensor * packetSize;

	//dprint("frame num %d sens %d\n", *(unsigned *)&buffer[offs], (*(unsigned *)&buffer[offs]) % 3);

	filter->Clean();
	if (bipolar)
	{
		for (int i = 0; i < packetSize && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = 100.0 * buffer[offs];
			data[i] = (*filter)(t / 128);
		}
	}
	else
	{
		for (int i = 0; i < packetSize && offs < Data::InputData::buffSize; ++i, ++offs)
		{
			double t = 100.0 * buffer[offs];
			t = (*filter)(t / 128);
			data[i] = t > 0 ? t : -t;
		}
	}
	
}
