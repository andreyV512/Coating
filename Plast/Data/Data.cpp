#include "Data.h"
#include "templates/typelist.hpp"

namespace Data
{
	struct TestData
	{
		TestData() 
		{
			{
				SensorData(&t) = Singleton<Sensor<1>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.outputData[i] = rand() % 10;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<2>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.outputData[i] = rand() % 10;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<3>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.outputData[i] = rand() % 10;
				}
			}
			{
				ResultData(&t) = Singleton<ResultData>::Instance();
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.maxData[i] = rand() % 10;
					t.minData[i] = 0.8 * t.maxData[i];
					t.satatus[i] = rand() % 5;
				}
			}
		}
	} x;
}

