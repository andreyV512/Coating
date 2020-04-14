#pragma once
#include "INTERFACES\IADCDevice.h"
#include "INTERFACES\IFactory.h"
#include "INTERFACES\DllClient.h"
#include "INTERFACES\REGKEY\VT_Drivers.ri"
#include "INTERFACES\REGKEY\VT_Library.ri"
#include "INTERFACES\ADC_Errors.h"
class Lan
{
	unsigned timeBeforeStop;
	void(Lan::*ptr)();	
	void DataCollection();
public:
	Lan();
	void Init();
	void Destroy();
	void Preparation();
	void Do();	
	void Start();
	void Stop();

	unsigned SetParams();
};