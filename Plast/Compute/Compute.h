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
	double start;
	int offsetLir;
	double offsetSensorMM;
public:
	int countZones;
	int zoneOffsets[1 + App::count_zones];
	Compute();

	void Start();
	bool StartStrobes();
	bool Strobes();
};