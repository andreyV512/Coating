#pragma once
#include "App/App.h"
namespace Data
{
	struct SensorData
	{
		int count;
		double data[App::count_zones];
		char status[App::count_zones];
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
		unsigned char status[App::count_zones];
	};

	struct InputData
	{
		static const unsigned buffSize = 10 * 140 * 100 * 4058 * App::count_sensors;
		unsigned framesCount, strobesTickCount, offsetsTickCount;
		char buffer[buffSize];
		unsigned strobesTick[100];
		unsigned offsetsTick[12000];
	};
}