#include "Lan.h"
#include "Devices/LanParameters.h"
#include "App/App.h"
#include "App/Config.h"
#include "tools_debug/DebugMess.h"

const char *BoardName = "LAn10M8PCI";

DWORD WINAPI Lan::__frame1__(PVOID p)
{
	Lan *l = (Lan *)p;
	l->Frame(l->device1);
	return 0;
}

DWORD WINAPI Lan::__frame2__(PVOID p)
{
	Lan *l = (Lan *)p;
	l->Frame(l->device2);
	return 0;
}

DWORD __stdcall Lan::__send__(PVOID p)
{
	((Lan *)p)->Send();
	return 0;
}

Lan::Lan()
	: terminate(false)
	, device1(NULL)
	, device2(NULL)
{
	InitializeCriticalSection(&cs);
	hTresh1 = CreateThread(NULL, 0, __frame1__, this, CREATE_SUSPENDED, NULL);
	hTresh2 = CreateThread(NULL, 0, __frame2__, this, CREATE_SUSPENDED, NULL);
	hTreshSend = CreateThread(NULL, 0, __send__, this, 0, NULL);
	hEventSend = CreateEvent(NULL, FALSE, FALSE, NULL);
}

Lan::~Lan()
{
	terminate = true;
	CloseHandle(hEventSend);
	SuspendThread(hTresh1);
	SuspendThread(hTresh2);
	CloseHandle(hTresh1);
	CloseHandle(hTresh2);
	CloseHandle(hTreshSend);
}

void Lan::Err(U32 err, wchar_t(&str)[256])
{
	unsigned xerr = err & ~0xffff;
	wsprintf(str, L"%x ", xerr);
	int len = (int)wcslen(str);
	U32 res = RshError::GetErrorDescription(xerr, &str[len], 256 - len);
	if (RSH_API_SUCCESS == res) return;
	xerr = err & 0xffff;
	wsprintf(str, L"%x ", xerr);
	len = (int)wcslen(str);
	res = RshError::GetSystemErrorDescription(xerr, &str[len], 256 - len);
}

void Lan::Start()
{
	while(!ResumeThread(hTresh1));
	while(!ResumeThread(hTresh2));
}

void Lan::Stop()
{
	SuspendThread(hTresh1);
	SuspendThread(hTresh2);
}

struct Tbuf: RshBaseType
{
	char *ptr;
	size_t m_size;
	size_t m_psize;
	Tbuf(char *ptr, size_t m_psize)
		: RshBaseType(rshBufferTypeS8, sizeof(RshBufferType<char, rshBufferTypeS8>))
		, ptr(ptr)
		, m_size(0)
		, m_psize(m_psize)
	{}
};

void Lan::Frame(IRshDevice *d)
{
	RSH_U32 waitTime = 10000;
	
	while (!terminate)
	{
		EnterCriticalSection(&cs);
		S32 st = d->Start();
		if (RSH_API_SUCCESS == st)
		{
			st = d->Get(RSH_GET_WAIT_BUFFER_READY_EVENT, &waitTime);
			d->Stop();
			if (RSH_API_SUCCESS == st)
			{
				char *addr = NULL;
				size_t count = (obj->*ptr)(addr);
				Tbuf buf(addr, count);
				st = d->GetData(&buf);
				if (RSH_API_SUCCESS == st)
				{
					//(obj->*confirmPtr)((unsigned)buf.m_size);
					SetEvent(hEventSend);
				}
			}
		}
		LeaveCriticalSection(&cs);
		if (RSH_API_SUCCESS != st)
		{
			wchar_t m[256];
			Err(st, m);
			int num = d == device1 ? 1 : 2;
			dprint("%d %S\n", num, m);
		}
		
	}
}

void Lan::Send()
{
	while (!terminate)
	{
		switch (WaitForSingleObject(hEventSend, INFINITE))
		{
		case WAIT_OBJECT_0:
			(obj->*confirmPtr)(0);
			break;
		case WAIT_ABANDONED:
			dprint("Send WAIT_ABANDONED\n");
			return;
		case WAIT_FAILED:
			dprint("Send WAIT_FAILED\n");
			return;
		}
	}
	dprint("Send terminate\n");
}

unsigned Lan::Init(int numDevece, IRshDevice *&d, RshInitMemory &p)
{
	U32 st = RSH_API_SUCCESS;
	RshDllInterfaceKey DevKey(BoardName, d);
	if (RSH_API_SUCCESS != (st = Client.GetDeviceInterface(DevKey)))return st;

	RshDeviceKey connectKey(numDevece);
	if (RSH_API_SUCCESS != (st = d->Connect(&connectKey)))return st;
	return d->Init(&p);
}
#define q(tpe)Singleton<LanParametersTable>::Instance().items.get<tpe>().value
void Lan::SetParams(RshInitMemory &p)
{
#ifndef TEST_LAN10
	p.startType = RshInitMemory::External;
#else 
	p.startType = RshInitMemory::Program;
#endif // !TEST_LAN10
	p.controlSynchro = q(SynchronizationEdge) ? RshInitMemory::SlopeDecline : 0;
	p.bufferSize = q(PacketSize);
	p.frequency = 1e6 * q(Frequency);
	p.channels[0].control = RshChannel::Used;
	if (!q(MeasurementInput))p.channels[0].control |= RshChannel::AC;
	p.channels[0].gain = q(Gain0);

	p.packetNumber = q(NumberPackets) * App::count_sensors;

	p.channelSynchro.gain = q(SyncGain);
	p.channelSynchro.control = q(SyncInput)? RshSynchroChannel::DC: RshSynchroChannel::AC;
	p.threshold = q(SyncLevel);
	p.startDelay = q(StartDelay);
}
#undef q
