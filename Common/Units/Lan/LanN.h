#pragma once
#include "Units/Lan/include/RshApi.h"
#include "App/AppBase.h"

class LanN
{
	static const int maxFrames = 128;
	int counter;
public:
	static inline int bufSize;
private:
	int numberUnit;
	char *data = NULL;
public:
	IRshDevice *device;
private:
	void Err(U32 err, wchar_t(&str)[256]);
	HANDLE	hThread;
	static inline HANDLE hWritePipe;
	static inline HANDLE hExit;
	static DWORD WINAPI __frame__(PVOID);
	static DWORD WINAPI __write_file__(LPVOID data);
public:
	~LanN();
	static void SetParams(RshInitMemory &, LanParametersTable::TItems &);
	U32 Init(int num, RshInitMemory &p, RshDllClient &client, HANDLE writePipe, HANDLE exit);
	void Start();
	void Stop();
	void Frame();
};

