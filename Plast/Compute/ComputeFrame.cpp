#include "ComputeFrame.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"
#include "SetTresholds.hpp"
#include "InitFiltre.hpp"
#include "Compute/SetTresholds.hpp"
#include "Data/StoreAllParam.h"

ComputeFrame::ComputeFrame()
	: paramFlt(ALLPatramsItems<FiltersTable>())//Singleton<ALLPatrams>::Instance().Items<FiltersTable>())
	, treshItems(ALLPatramsItems<TresholdsTable>())//Singleton<ALLPatrams>::Instance().Items<TresholdsTable>())
	, framesCount(Singleton<Data::InputData>::Instance().framesCount)
	, buffer(Singleton<Data::InputData>::Instance().buffer)
	, bipolar(false)
{
	//ALLPatrams &all = Singleton<ALLPatrams>::Instance();

	frequency = 1000000 * ALLPatramsItems<LanParametersTable>().get<Frequency>().value;
	packetSize = ALLPatramsItems<LanParametersTable>().get<PacketSize>().value;

	//VL::CopyFromTo(all.Items<FiltersTable>(), paramFlt);
	UpdateFiltre();
	//VL::CopyFromTo(all.Items<TresholdsTable>(), treshItems);
	SetParam(*this, treshItems);
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

		if (bottomReflectionOn[sensor])
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

void ComputeFrame::Gain(int sensor, double *data)
{
	unsigned i = 0;
	for (; i < offsAlarmStart[sensor]; ++i)
	{
		data[i] = 1;
	}
	double gain = gainAlarmOffs[sensor];
	for (; i < offsAlarmStop[sensor]; ++i)
	{
		data[i] = gain;
		gain += gainAlarmDelta[sensor];
	}

	if (bottomReflectionOn[sensor])
	{
		for (; i < offsReflectionStart[sensor]; ++i)
		{
			data[i] = 1;
		}
		gain = gainReflectionOffs[sensor];
		for (; i < offsReflectionStop[sensor]; ++i)
		{
			data[i] = gain;
			gain += gainReflectionDelta[sensor];
		}
	}
	else
		for (; i < packetSize; ++i)
		{
			data[i] = 1;
		}
}
