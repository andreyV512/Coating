#include "Compute.h"
//#include "templates/typelist.hpp"
#include "Data/Data.h"
#include "App/AppBase.h"

Compute::Compute()
	: inputData(Singleton<Data::InputData>::Instance())
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
	, offsetSensorBegMM(Singleton<UnitTable>::Instance().items.get<OffsetSensorBegMM>().value)
	, offsetSensorEndMM(Singleton<UnitTable>::Instance().items.get<OffsetSensorEndMM>().value)
{
}

bool Compute::StartStrobes()
{
	if (inputData.offsetsLir[inputData.countFrames] > inputData.startOffsetLir + offsetSensorBegMM)
	{
		start = inputData.startOffsetLir + offsetSensorBegMM;
		offsetLir = 0;
		countZones = 0;
		offsetFrames = 0;
		return true;
	}
	return false;
}

bool Compute::Strobes()
{
	static const int size_array = dimention_of(inputData.offsetFrames);
	int currentCountFrames = inputData.countFrames < size_array? inputData.countFrames: size_array;
	bool res = false;
	const int frameSize = packetSize * numberPackets * App::count_zones;
	for (; offsetLir < currentCountFrames; ++offsetLir)
	{
		while(start < inputData.offsetsLir[offsetLir])
		{
			double delta = (start - inputData.offsetsLir[offsetLir - 1]) / (inputData.offsetsLir[offsetLir] - inputData.offsetsLir[offsetLir - 1]);
			int offs_1 = inputData.offsetFrames[offsetLir - 1] / frameSize;
			int offs = inputData.offsetFrames[offsetLir] / frameSize;
			zoneOffsets[countZones] = frameSize * (offs_1 + int(delta * (offs - offs_1)));

			if(dimention_of(zoneOffsets) > countZones)++countZones;
			start += App::size_zone_mm;
			res = true;
		}
	}
	return res;
}

void Compute::Frames()
{
	for (; offsetFrames < offsetLir; ++offsetFrames)
	{
		int sens = 0;
		for (int i = inputData.offsetFrames[offsetFrames]; i < inputData.offsetFrames[offsetFrames + 1]; i += packetSize)
		{
			ComputeFrame(sens, offsetFrames, &inputData.buffer[i]);
			++sens;
			sens %= App::count_sensors;
		}
		ComputeZone(offsetFrames);
	}
}

void Compute::ComputeFrame(int sensor, int zone, char *data)
{
	//double result[App::count_sensors][sensorBuffSize];
	//char status[App::count_sensors][sensorBuffSize];
	//TODO ...
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
		Frames();
	}
}

void Compute::Done()
{
}
