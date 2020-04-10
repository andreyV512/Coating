#pragma once
#include "App/App.h"
namespace Data
{
	struct SensorData
	{
		int count;
		double outputData[App::count_zones];
	};

	template<int N>struct Sensor
	{
		SensorData data;
	};

	struct ResultData
	{
		int count;
		double minData[App::count_zones];
		double maxData[App::count_zones];
		unsigned char satatus[App::count_zones];
	};
}