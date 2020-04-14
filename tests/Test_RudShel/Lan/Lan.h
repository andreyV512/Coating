#pragma once
#include "INTERFACES\IADCDevice.h"
#include "INTERFACES\IFactory.h"
#include "INTERFACES\DllClient.h"
#include "INTERFACES\REGKEY\VT_Drivers.ri"
#include "INTERFACES\REGKEY\VT_Library.ri"
#include "INTERFACES\ADC_Errors.h"
#undef POLLING
class Lan
{
	DllClient LAxClient;
	int bufSize_;
	unsigned timeBeforeStop;
	void(Lan::*ptr)();	
	IADCDevice *ADC1, *ADC2;
#ifndef POLLING
	int missedCycles;
	HANDLE hEvent[2];
#else
	bool start1, start2;
#endif
	void DataCollection();
public:
	const int &bufSize;
	Lan();
	bool Init();
	void Destroy();
	bool Do();	
	void Start();
	void Stop();
	void InitParameters();
};

extern Lan lan;
