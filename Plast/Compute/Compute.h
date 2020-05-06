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
	int packetSize;
	int numberPackets;
	double start;
	int offsetLir;
	int offsetFrames;
	int offsetSensorBegMM, offsetSensorEndMM;
public:
	int countZones;
	int zoneOffsets[1 + App::count_zones];
	Compute();

	bool StartStrobes();
	bool Strobes();
	void Frames();
	void ComputeFrame(int sensor, int zone, char *data);
	void ComputeZone(int zone);
	void Update();
	void Done();
};