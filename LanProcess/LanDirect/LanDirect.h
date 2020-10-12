#pragma once
#include <windows.h>

class LanProcess
{
	int numberPackets, packetSize, bufSize;
public:
	HANDLE hStart, hStop;
private:
	HANDLE hWritePipe;
	char *data;
public:
	LanProcess(HANDLE hWritePipe);
	~LanProcess();
	int Buff(char *&buf);
	void Confirm(unsigned b);
};

namespace Data { struct InputData; }
class LanRead
{
	int numberPackets, packetSize, bufSize;
	HANDLE hStart, hStop, hReadPipe; 
	Data::InputData &data;
public:
	LanRead();
	~LanRead();
	void Read();
	void Start();
	void Stop();
};

