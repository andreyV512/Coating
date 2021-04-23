#include "stdafx.h"
#include "crc.h"
namespace
{
	unsigned short buf[256];
	struct CRC16
	{
		CRC16()
		{
			for (int i = 0; i < 256; ++i)
			{
				unsigned short t = i;
				for (int j = 0; j < 8; ++j)
				{
					bool b = t & 1;
					t >>= 1;
					if (b) t ^= 0xa001;
				}
				buf[i] = t;
			}
		}
	}crc16;
}

unsigned short Crc16(unsigned char *data, int length)
{
	unsigned short result = 0xffff;
	while (--length >= 0)
	{
		unsigned char i = result ^ *data;
		result >>= 8;
		result ^= buf[i];
		++data;
	}
	return result;
}



