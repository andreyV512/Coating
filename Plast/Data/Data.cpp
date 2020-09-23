#include "Data.h"
#include "templates/typelist.hpp"

namespace Data
{
	struct TestData
	{
		TestData() 
		{
			{
				SensorData(&t) = Singleton<Sensor<0>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<1>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<2>>::Instance().data;
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
			{
				ResultData(&t) = Singleton<ResultData>::Instance();
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.maxData[i] = rand() % 10;
					t.minData[i] = 0.8 * t.maxData[i];
					t.status[i] = rand() % 6;
				}
			}
			{
				InputData &t = Singleton<InputData>::Instance();
				memset(t.buffer, 0, dimention_of(t.buffer));
				for(int i = 0; i < InputData::buffSize; i += 986)
				{ 
					int offsZ = rand() % 50;
					int widthZ = rand() % 15 + 5;
					int amplZ = rand() % 70 + 50;

					int offsR = rand() % 200 + 150;
					int widthR = rand() % 15 + 5;
					int amplR = rand() % 80 + 30;

					char *z = &t.buffer[i] + offsZ;
					for (int k = 0; k < widthZ; ++k)
					{
						z[k] = amplZ;
					}

					char *r = &t.buffer[i] + offsR;
					for (int k = 0; k < widthR; ++k)
					{
						r[k] = amplR;
					}
				}
				unsigned f = 0;
				double l = 0;
			}
		}
	} x;
}

