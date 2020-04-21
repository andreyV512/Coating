#pragma once
#include "App/App.h"
namespace Data
{
	struct SensorData
	{
		int count;
		double outputData[App::count_zones];
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
		unsigned char satatus[App::count_zones];
	};

	struct InputData
	{
		static const int buffSize = 10 * 120 * 100 * 986 * App::count_sensors;
		static const int sensorBuffSize = buffSize / 986 / App::count_sensors;
		int countFrames;
		char buffer[buffSize];
		double result[App::count_sensors][sensorBuffSize];
		char status[App::count_sensors][sensorBuffSize];
		double startOffsetLir;
		int offsetFrames[2000];
		double offsetsLir[2000];
	};
}