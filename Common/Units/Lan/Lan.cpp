#include "Lan.h"
#include "Devices/LanParameters.h"
#include "App/App.h"
const char *BoardName = "LAn10M8PCI";
#define dprint printf

DWORD WINAPI __frame1__(PVOID p)
{
	Lan *l = (Lan *)p;
	l->Frame(l->device1);
	return 0;
}

DWORD WINAPI __frame2__(PVOID p)
{
	Lan *l = (Lan *)p;
	l->Frame(l->device2);
	return 0;
}

Lan::Lan()
	: terminate(false)
	, device1(NULL)
	, device2(NULL)
{
	hTresh1 = CreateThread(NULL, 0, __frame1__, this, CREATE_SUSPENDED, NULL);
	hTresh2 = CreateThread(NULL, 0, __frame2__, this, CREATE_SUSPENDED, NULL);
}

Lan::~Lan()
{
	terminate = true;
	//WaitForSingleObject(hTresh1, INFINITE);
	//WaitForSingleObject(hTresh2, INFINITE);
	SuspendThread(hTresh1);
	SuspendThread(hTresh2);
	CloseHandle(hTresh1);
	CloseHandle(hTresh2);
}

bool Lan::Err(U32 err, wchar_t(&str)[256])
{
	//char str[256];
	unsigned xerr = err & ~0xffff;
	wsprintf(str, L"%x ", xerr);
	int len = (int)wcslen(str);
	U32 res = RshError::GetErrorDescription(xerr, &str[len], 256 - len);
	if (RSH_API_SUCCESS != res)
	{
		return true;
	}
	xerr = err & 0xffff;
	wsprintf(str, L"%x ", xerr);
	len = (int)wcslen(str);
	res = RshError::GetSystemErrorDescription(xerr, &str[len], 256 - len);
	return RSH_API_SUCCESS != res;
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
		: RshBaseType(rshBufferTypeDouble, sizeof(RshBufferType<char, rshBufferTypeDouble>))
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
		S32 st = d->Start();
		st = d->Get(RSH_GET_WAIT_BUFFER_READY_EVENT, &waitTime);
		char *addr = NULL;
		size_t count = (obj->*ptr)(addr);
		Tbuf buf(addr, count);
		st = d->GetData(&buf);
		if (RSH_API_SUCCESS == st)
		{
			(obj->*confirmPtr)((unsigned)buf.m_size);
		}
		else
		{
			wchar_t m[256];
			char c[256];
			Err(st, m);
			wcstombs(c, m, dimention_of(m));
			int num = d == device1 ? 1 : 2;
			dprint("%d %s\n", num, c);
		}
	}
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
	p.startType = RshInitMemory::External;
	p.controlSynchro = q(SynchronizationEdge) ? RshInitMemory::SlopeDecline : 0;
	p.bufferSize = q(PacketSize);
	p.frequency = 1e6 * q(Frequency);
	p.channels[0].control = RshChannel::Used;
	if (q(MeasurementInput))p.channels[0].control |= RshChannel::AC;
	p.channels[0].gain = q(Gain0);

	p.packetNumber = q(NumberPackets) * App::count_sensors;

	p.channelSynchro.gain = q(SyncGain);
	p.channelSynchro.control = q(SyncInput)? RshSynchroChannel::AC: RshSynchroChannel::DC;
	p.threshold = q(SyncLevel);
	p.startDelay = q(StartDelay);
}
#undef q
