#pragma once
#include "Units/Lan/include/RshApi.h"
#include "App/AppBase.h"

class LanN
{
	static const int maxFrames = 4;
public:
	static inline int bufSize;
private:
	int numberUnit;
	char *data = NULL;
public:
	void(*ptr)(char *);
	IRshDevice *device;
private:
	void Err(U32 err, wchar_t(&str)[256]);
	HANDLE	hThread;
	static DWORD WINAPI __frame__(PVOID);
public:
	~LanN();
	static void SetParams(RshInitMemory &, LanParametersTable::TItems &);
	U32 Init(int num, RshInitMemory &p, RshDllClient &client);
	void Start();
	void Stop();
	void Frame();
};

