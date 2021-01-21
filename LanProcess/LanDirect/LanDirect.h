#pragma once
#include <windows.h>

namespace Data { struct InputData; }
class LanRead
{
	bool start = false;
	int &numberPackets, &packetSize, bufSize;
	HANDLE hStart, hStop, hExit, hReadPipe, hThread; 
	Data::InputData &data;
	void Read();
	static DWORD WINAPI __proc__(PVOID);
public:
	LanRead();
	~LanRead();	
	void Update();
	void Start();
	void Stop();

	void Reload();
};

