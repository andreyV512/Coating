#pragma once
#include "Units/Lan/include/RshApi.h"

class Lan
{
	static DWORD WINAPI __frame1__(PVOID);
	static DWORD WINAPI __frame2__(PVOID);
	static DWORD WINAPI __send__(PVOID);
	struct TObj {};
	TObj *obj;
	int(TObj:: *ptr)(char *&);
	void(TObj:: *confirmPtr)(unsigned);
	RshDllClient Client;
	CRITICAL_SECTION cs;
public:
	IRshDevice *device1, *device2;
private:
	HANDLE hTresh1, hTresh2, hTreshSend, hEventSend;
	bool terminate;
public:
	Lan();
	~Lan();
	unsigned Init(int numDevece, IRshDevice *&d, RshInitMemory &p);
	void SetParams(RshInitMemory &);
	void Err(U32 err, wchar_t(&str)[256]);

	void Start();
	void Stop();

	void Frame(IRshDevice *);
	void Send();

	template<class T>void SetHandler(T *t, int(T:: *p)(char *&), void(T:: *c)(unsigned))
	{
		obj = (TObj *)t;
		ptr = (int(TObj:: *)(char *&))p;
		confirmPtr = (void(TObj::*)(unsigned))c;
	}
};