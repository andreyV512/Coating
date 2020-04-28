#pragma once
#include "App/App.h"
namespace Data { struct InputData; }

/*
INFO Вычисление результата контроля
перед сбором данных вызвать:
 -Start
 -StartStrobes - вычисляет начало прутка с учётом смещения датчика 
 -Strobes - вычисляет разбиение собранных данных АЦП по зонам относительно лира.
 если зона добаляется функция возвращаен true 
*/
class Compute
{
	Data::InputData &inputData;
	double start;
	int offsetLir;
	int offsetFrames;
	double offsetSensorMM;
public:
	int countZones;
	int zoneOffsets[1 + App::count_zones];
	Compute();

	bool StartStrobes();
	bool Strobes();
	void Frames();
	void ComputeFrame(int i);
	void Update();
	void Done();
};