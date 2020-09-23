#include "MedianFiltre\MedianFiltre.h"
#include <windows.h>

MedianFiltre::MedianFiltre()
{
	width = 0;
	index = 0;
	medianIndex = 0;
	memset(buf, 0, sizeof(buf));
}


void MedianFiltre::InitWidth(int width_)
{
	width_ |= 1;
	width_ &= 0xf;
	width = width_;
	index = width_;
	medianIndex = width_ / 2;
}

void MedianFiltre::Clear()
{
	memset(buf, 0, sizeof(buf));
}

double MedianFiltre::Val(double d, char &s)
{
	int index_ = index % width;
	++index;
	
	memset(ind, 0, width * sizeof(int));
	
	int cnt = 0;
	buf[index_] = d;
	status[index_] = s;

	for(int i = 0; i < width - 1; ++i)
	{
		for(int j = i + 1; j < width; ++j)
		{
		   if(buf[i] >= buf[j]) ++ind[i];
		   else if(buf[i] < buf[j]) ++ind[j];
		}
	}

	for(int i = 0; i < width; ++i)
	{
		if (medianIndex == ind[i])
		{
			s = status[i];
			return buf[i];
		}
	}
	s = status[0];
	return buf[0];
}
