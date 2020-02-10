#pragma once
#include "1730/Device1730.h"

namespace App
{
	static const int count_sensors = 20;
	void Init();
	void Destroy();
	void TopLabel(wchar_t(&)[128]);

	wchar_t *Salt();
}

extern Device1730 device1730;