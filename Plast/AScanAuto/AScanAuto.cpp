#include "AScanAuto.h"
#include <Windows.h>
#include "Windows/AScanWindow/AScanKeyHandler.h"
#include "Devices/LanDevice.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"
#include "tools_debug/DebugMess.h"

class Scan: public AScanAuto::IScan
{
	AScanAuto *owner;
public:
	HANDLE hThread, hEvent;
	void Run();
	static DWORD WINAPI __run__(_In_ LPVOID lpParameter) { ((Scan *)lpParameter)->Run(); return 0; }
public:
	Scan(AScanAuto *owner)
		: owner(owner)
		, hThread(NULL)
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		
	}
	~Scan()
	{
		CloseHandle(hEvent);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
};

void Scan::Run()
{
	unsigned time = Performance::Counter();
	CollectionData data;
	while (true)
	{
		//TODO Доделать сбор данных
		switch (WaitForSingleObject(hEvent, 50))
		{
		case WAIT_OBJECT_0:
			dprint("WAIT_OBJECT_0\n");
			CloseHandle(hThread);
			hThread = NULL;
			return;
		case WAIT_TIMEOUT: 
		{
			(owner->obj->*owner->Update)();
			unsigned t = Performance::Counter();
			if (time - t > 3000)
			{
				Singleton<Data::InputData>::Instance().framesCount = 0;
			}
		}
			break;
		case WAIT_ABANDONED:
			dprint("WAIT_ABANDONED\n");
		[[fallthrough]];
		case WAIT_FAILED:
			dprint("WAIT_FAILED\n");
			return;
		}
	}
}

AScanAuto::AScanAuto()
{
	impl.Init<Scan>(this);
}

void AScanAuto::Start() 
{
	AScanKeyHandler::Run();
	Scan *s = (Scan *)&impl;
    s->hThread = CreateThread(NULL, 0, s->__run__, s, 0, NULL);
}
void AScanAuto::Stop() 
{
	AScanKeyHandler::Stop();
	SetEvent(((Scan *)&impl)->hEvent);
}
