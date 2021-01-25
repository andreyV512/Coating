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

	double noop(double value);// {return value; }
	double noop(double value, char &, unsigned &);// { return value; }
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