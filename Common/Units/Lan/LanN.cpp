#include "LanN.h"
#include "App/App.h"
#include "tools_debug/DebugMess.h"
#include "../LanProcess/LanDirect/EventNames.h"

struct RshCharType : RshBaseType
{
	char *ptr;
	size_t m_size;
	size_t m_psize;
	RshCharType(char *ptr, size_t m_psize)
		: RshBaseType(rshBufferTypeS8, sizeof(RshBufferType<char, rshBufferTypeS8>))
		, ptr(ptr)
		, m_size(0)
		, m_psize(m_psize)
	{}
};

DWORD __stdcall LanN::__frame__(PVOID p)
{
	((LanN *)p)->Frame();
    return 0;
}

void LanN::Err(U32 err, wchar_t(&str)[256])
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

LanN::~LanN()
{
	TerminateThread(hThread, 0);
}

#define q(tpe)items.get<tpe>().value
void LanN::SetParams(RshInitMemory &p, LanParametersTable::TItems &items)
{
	p.startType = RshInitMemory::External;
	p.controlSynchro = q(SynchronizationEdge) ? RshInitMemory::SlopeFront : RshInitMemory::SlopeDecline;
	dprint("p.controlSynchro %d\n", p.controlSynchro);
	p.bufferSize = q(PacketSize);
	p.frequency = 1e6 * q(Frequency);
	p.channels[0].control = q(MeasurementInput) ? RshChannel::DC : RshChannel::AC;
	p.channels[0].control |= RshChannel::Used;
	p.channels[0].gain = q(Gain0);

	p.packetNumber = q(NumberPackets) * App::count_sensors;

	p.channelSynchro.gain = q(SyncGain);
	p.channelSynchro.control = q(SyncInput) ? RshSynchroChannel::DC : RshSynchroChannel::AC;
	p.threshold = q(SyncLevel);
	p.startDelay = q(StartDelay);
}
#undef q

U32 LanN::Init(int num, RshInitMemory &p, RshDllClient &client, HANDLE writePipe, HANDLE exit)
{
	numberUnit = num;

	U32 st = RSH_API_SUCCESS;
	RshDllInterfaceKey DevKey("LAn10M8PCI", device);
	if (RSH_API_SUCCESS != (st = client.GetDeviceInterface(DevKey)))return st;

	RshDeviceKey connectKey(num);
	if (RSH_API_SUCCESS != (st = device->Connect(&connectKey)))return st;
	st = device->Init(&p);

	if (RSH_API_SUCCESS != st)
	{
		HWND h = FindWindow(L"MainWindowCoating", NULL);
		if (NULL != h)
		{
			COPYDATASTRUCT ct;
			ct.cbData = sizeof(int);
			ct.dwData = ID_ERROR_INIT_LAN_BOARD;
			ct.lpData = &num;
			SendMessage(h, WM_COPYDATA, NULL, (LPARAM)&ct);
			dprint("FIND WINDOW MainWindowCoating %x ID_ERROR_INIT_LAN_BOARD\n", h);
		}
	}

	bufSize = p.bufferSize * p.packetNumber;
	hWritePipe = writePipe;
	hExit = exit;
	if (0 != hThread)SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
	return st;
}

void LanN::Start()
{
	while (!ResumeThread(hThread));
}

void LanN::Stop()
{
	SuspendThread(hThread);
}

void LanN::Frame() 
{
	RSH_U32 waitTime = 3000;
	unsigned counter = 0;
	data = new char[bufSize * maxFrames];
	while (true)
	{
		S32 st = device->Start();
		if (RSH_API_SUCCESS == st)
		{
			st = device->Get(RSH_GET_WAIT_BUFFER_READY_EVENT, &waitTime);
			device->Stop();
			if (RSH_API_SUCCESS == st)
			{
				char *addr = &data[bufSize * (counter % maxFrames)];
				++counter;
				RshCharType buf(addr, bufSize);
				st = device->GetData(&buf);
				if (RSH_API_SUCCESS == st) 
					QueueUserWorkItem(__write_file__, addr, WT_EXECUTEDEFAULT);
			}
		}
		if (RSH_API_SUCCESS != st)
		{
			wchar_t m[256];
			Err(st, m);
			dprint("%d %S\n", numberUnit, m);

			HWND h = FindWindow(L"MainWindowCoating", NULL);
			if (NULL != h)
			{
				COPYDATASTRUCT ct;
				ct.cbData = sizeof(int);
				ct.dwData = ID_NO_DATA_COLLECTION_FROM_LAN_BOARD;
				ct.lpData = &numberUnit;
				SendMessage(h, WM_COPYDATA, NULL, (LPARAM)&ct);
				dprint("FIND WINDOW MainWindowCoating %x ID_NO_DATA_COLLECTION_FROM_LAN_BOARD\n", h);
			}
		}
	}
}

DWORD WINAPI LanN::__write_file__(LPVOID data)
{
	DWORD bytesWritten;
	if (!WriteFile(hWritePipe, data, bufSize, &bytesWritten, NULL))
	{
		DWORD ret = GetLastError();
		dprint("client WriteFalted %d\n", ret);
		SetEvent(hExit);
	}
	return 0;
}
