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
		static const unsigned long long buffSize = (unsigned long long)3 * 1024 * 1024 * 1024;
		unsigned framesCount;
		int strobesTickCount, offsetsTickCount;
		static char *buffer;
		unsigned strobesTick[10000];
		unsigned offsetsTick[12000];
	};
}