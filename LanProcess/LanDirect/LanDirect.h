#pragma once
#include <windows.h>

extern const wchar_t *wStart;
extern const wchar_t *wStop;

namespace Data { struct InputData; }
class LanRead
{
	int numberPackets, packetSize, bufSize;
	HANDLE hStart, hStop, hReadPipe, hThread; 
	Data::InputData &data;
	void Read();
	static DWORD WINAPI __proc__(PVOID);
public:
	LanRead();
	~LanRead();	
	void Start();
	void Stop();
};

