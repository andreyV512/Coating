#pragma once 

struct MedianFiltre
{
	int index;
	int width;
	int medianIndex;
	double buf[16];
	int status[16];
	unsigned offs[16];
	int ind[16];
	MedianFiltre();

	void InitWidth(int width_);
	
	void Clear();
	
	double Val(double d);
	double Val(double d, char &s, unsigned &o);
	double Stat(double d, char &s, unsigned &o);

	double noop(double value);
	double noop(double value, char &, unsigned &);
};

struct ChMedFlt
{
	int index;
	int width;
	int medianIndex;
	char buf[16];
	//unsigned offs[16];
	int ind[16];
	ChMedFlt();

	void InitWidth(int width_);

	void Clear();
	char Val(char);

	char noop(char);
};