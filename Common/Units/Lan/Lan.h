#pragma once
#include "Units/Lan/include/RshApi.h"

class Lan
{
	friend DWORD WINAPI __frame1__(PVOID);
	friend DWORD WINAPI __frame2__(PVOID);
	struct TObj {};
	TObj *obj;
	int(TObj:: *ptr)(char *&);
	RshDllClient Client;
public:
	IRshDevice *device1, *device2;
private:
	HANDLE hTresh1, hTresh2;
	bool terminate;
public:
	Lan();
	~Lan();
	unsigned Init(int numDevece, IRshDevice *&d, RshInitMemory &p);
	void SetParams(RshInitMemory &);
	bool Err(U32 err, wchar_t(&str)[256]);

	void Start();
	void Stop();

	void Frame(IRshDevice *);

	template<class T>void SetHandler(T *t, int(T:: *p)(char *&))
	{
		obj = (TObj *)t;
		ptr = (int(TObj:: *)(char *&))p;
	}
};