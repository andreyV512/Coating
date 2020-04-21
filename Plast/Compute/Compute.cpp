#include "Compute.h"
#include "templates/typelist.hpp"
#include "Data/Data.h"

Compute::Compute()
	: inputData(Singleton<Data::InputData>::Instance())
	, offsetSensorMM(10)
{
}

void Compute::Start()
{
	inputData.countFrames = 0;
}

bool Compute::StartStrobes()
{
	if (inputData.offsetsLir[inputData.countFrames] > inputData.startOffsetLir + offsetSensorMM)
	{
		start = inputData.startOffsetLir + offsetSensorMM;
		offsetLir = 0;
		countZones = 0;
		return true;
	}
	return false;
}

bool Compute::Strobes()
{
	static const int size_array = dimention_of(inputData.offsetFrames);
	int currentCountFrames = inputData.countFrames < size_array? inputData.countFrames: size_array;
	bool res = false;
	for (; offsetLir < currentCountFrames; ++offsetLir)
	{
		while(start < inputData.offsetsLir[offsetLir])
		{
			double delta = (start - inputData.offsetsLir[offsetLir - 1]) / (inputData.offsetsLir[offsetLir] - inputData.offsetsLir[offsetLir - 1]);
			zoneOffsets[countZones] = inputData.offsetFrames[offsetLir - 1] + int(delta * (inputData.offsetFrames[offsetLir] - inputData.offsetFrames[offsetLir - 1]));

			if(dimention_of(zoneOffsets) > countZones)++countZones;
			start += App::size_zone_mm;
			res = true;
		}
	}
	return res;
}
