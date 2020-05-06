#pragma once
#include "App/App.h"
namespace Data { struct InputData; }

/*
INFO ���������� ���������� ��������
����� ������ ������ �������:
 -Start
 -StartStrobes - ��������� ������ ������ � ������ �������� ������� 
 -Strobes - ��������� ��������� ��������� ������ ��� �� ����� ������������ ����.
 ���� ���� ���������� ������� ���������� true 
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