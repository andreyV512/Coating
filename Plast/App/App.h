#pragma once

namespace App
{
	static const int count_sensors = 20;
	void Init();
	void Destroy();

	wchar_t *Salt();
}