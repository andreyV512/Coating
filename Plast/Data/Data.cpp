#include "Data.h"
#include "templates/typelist.hpp"
#include "MessageText/status.h"

namespace Data
{
	struct TestData
	{
		TestData() 
		{
			double *d[3];
			char *c[3];
			{
				SensorData(&t) = Singleton<Sensor<0>>::Instance().data;
				t.count = 50;
				d[0] = t.data;
				c[0] = t.status;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<1>>::Instance().data;
				t.count = 50;
				d[1] = t.data;
				c[1] = t.status;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
			{
				SensorData(&t) = Singleton<Sensor<2>>::Instance().data;
				t.count = 50;
				d[2] = t.data;
				c[2] = t.status;
				for (int i = 0; i < t.count; ++i)
				{
					t.data[i] = rand() % 10;
					t.status[i] = rand() % 6;
				}
			}
#define MAX2(a, b) (a) > (b) ? (a): (b)
#define MAX(a, b, c) MAX2(MAX2(a, b), c)
#define MIN2(a, b) (a) < (b) ? (a): (b)
#define MIN(a, b, c) MIN2(MIN2(a, b), c)
			{
				ResultData(&t) = Singleton<ResultData>::Instance();
				t.count = 50;
				for (int i = 0; i < t.count; ++i)
				{
					t.maxData[i] = MAX(d[0][i], d[1][i], d[2][i]);
					t.minData[i] = MIN(d[0][i], d[1][i], d[2][i]);
					t.status[i] = StatusData::Compute(StatusData::Compute(c[0][i], c[1][i]), c[2][i]);
				}
			}
#undef MAX2
#undef MAX
#undef MIN2
#undef MIN
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

