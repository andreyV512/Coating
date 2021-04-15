#pragma once
#include "Units/1730/Device1730.h"

namespace App
{
	static const int count_zones = 100;
	static const int count_sensors = 3;
	static const int size_zone_mm = 50;

	static const int first_section = 0;
	static const int operator_status_section = 3;

    unsigned (&packet_size_buffer())[6];

	void Init();
	void Destroy();
	void TopLabel(wchar_t(&)[128]);
	void StatusBar(int, wchar_t *);

	void  CurrentOperatorName(wchar_t(&)[64]);

	wchar_t *Salt();

	bool &IsRun();
}

extern Device1730 device1730;