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
	memset(offs, 0, sizeof(offs));
}

double MedianFiltre::Val(double d)
{
	int index_ = index % width;
	++index;
	
	memset(ind, 0, width * sizeof(int));
	
	int cnt = 0;
	buf[index_] = d;

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
			return buf[i];
		}
	}
	return buf[0];
}

double MedianFiltre::Val(double d, char &s, unsigned &o)
{
	int index_ = index % width;
	++index;

	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;
	status[index_] = s;
	offs[index_] = o;

	for (int i = 0; i < width - 1; ++i)
	{
		for (int j = i + 1; j < width; ++j)
		{
			if (buf[i] >= buf[j]) ++ind[i];
			else if (buf[i] < buf[j]) ++ind[j];
		}
	}

	for (int i = 0; i < width; ++i)
	{
		if (medianIndex == ind[i])
		{
			s = status[i];
			o = offs[i];
			return buf[i];
		}
	}
	s = status[0];
	o = offs[0];
	return buf[0];
}

double MedianFiltre::Stat(double d, char &s, unsigned &o)
{
	int index_ = index % width;
	++index;

	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;
	status[index_] = s;
	offs[index_] = o;

	for (int i = 0; i < width - 1; ++i)
	{
		for (int j = i + 1; j < width; ++j)
		{
			if (status[i] >= status[j]) ++ind[i];
			else if (status[i] < status[j]) ++ind[j];
		}
	}

	for (int i = 0; i < width; ++i)
	{
		if (medianIndex == ind[i])
		{
			s = status[i];
			o = offs[i];
			return buf[i];
		}
	}
	s = status[0];
	o = offs[0];
	return buf[0];
}

double MedianFiltre::noop(double value)
{
	return value; 
}

double MedianFiltre::noop(double value, char &, unsigned &)
{
	return value; 
}

ChMedFlt::ChMedFlt()
{
	width = 0;
	index = 0;
	medianIndex = 0;
	memset(buf, 0, sizeof(buf));
}

void ChMedFlt::InitWidth(int width_)
{
	width_ |= 1;
	width_ &= 0xf;
	width = width_;
	index = width_;
	medianIndex = width_ / 2;
}

void ChMedFlt::Clear()
{
	memset(buf, 0, sizeof(buf));
}

char ChMedFlt::Val(char d)
{
	int index_ = index % width;
	++index;

	memset(ind, 0, width * sizeof(int));

	int cnt = 0;
	buf[index_] = d;

	for (int i = 0; i < width - 1; ++i)
	{
		for (int j = i + 1; j < width; ++j)
		{
			if (buf[i] >= buf[j]) ++ind[i];
			else if (buf[i] < buf[j]) ++ind[j];
		}
	}

	for (int i = 0; i < width; ++i)
	{
		if (medianIndex == ind[i])
		{
			return buf[i];
		}
	}
	return buf[0];
}

char ChMedFlt::noop(char)
{
	return 0;
}
