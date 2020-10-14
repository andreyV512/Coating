#pragma once
#include <windows.h>

extern const wchar_t *wStart;
extern const wchar_t *wStop; 
extern const wchar_t *wExit;

namespace Data { struct InputData; }
class LanRead
{
	int numberPackets, packetSize, bufSize;
	HANDLE hStart, hStop, hExit, hReadPipe, hThread; 
	Data::InputData &data;
	void Read();
	static DWORD WINAPI __proc__(PVOID);
public:
	LanRead();
	~LanRead();	
	void Start();
	void Stop();
};

