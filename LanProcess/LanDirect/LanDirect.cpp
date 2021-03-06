#include "LanDirect.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "window_tool/RunExecute.h"
#include "EventNames.h"
#include "CommonApp.h"
#include "Automat/Automat.h"

DWORD WINAPI LanRead::__proc__(PVOID p)
{
	while(true)((LanRead *)p)->Read();
	return 0;
}

template<class O, class P>struct LanRead__params__
{
	void operator()(O &o, P &p)
	{
		Wchar_from<typename O::type_value> val(o.value);
		wsprintf(p, L"%s=%s ", o.name(), val());
		p += wcslen(p);
	}
};

VOID CALLBACK WaitCallBack(PVOID param, BOOLEAN b)
{
	HANDLE h = OpenEvent(EVENT_ALL_ACCESS, TRUE, wLanParams);
	if (0 != h)
	{
		SetEvent(h);
		CloseHandle(h);
		DeleteTimerQueueTimer(NULL, (HANDLE)param, NULL);
		dprint("TIMER SEND MESSAGE\n");
	}
}

LanRead::LanRead()
	: numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, data(Singleton<Data::InputData>::Instance())
	, hReadPipe(NULL)
{
	bufSize = packetSize * numberPackets * App::count_sensors;
	Performance::Init();
	hStart = CreateEvent(NULL, FALSE, FALSE, wStart);
	hStop = CreateEvent(NULL, FALSE, FALSE, wStop);
	hExit = CreateEvent(NULL, FALSE, FALSE, wExit);
	
	hThread = CreateThread(NULL, 0, __proc__, this, CREATE_SUSPENDED, NULL); 
	if(0 != hThread)SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
}

LanRead::~LanRead()
{
	SetEvent(hExit);
	TerminateThread(hThread, 0);
	CloseHandle(hStart);
	CloseHandle(hStop);
	CloseHandle(hReadPipe);
	
	CloseHandle(hExit);
}

void LanRead::Update()
{
	bufSize = packetSize * numberPackets * App::count_sensors;
	HANDLE hWritePipe, hInheritWritePipe;

	if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0))
	{
		DWORD ret = GetLastError();
		dprint("Create pipe failed %d\n", ret);
	}

	if (!DuplicateHandle(
		GetCurrentProcess()
		, hWritePipe
		, GetCurrentProcess()
		, &hInheritWritePipe
		, 0
		, TRUE
		, DUPLICATE_SAME_ACCESS
	))
	{
		DWORD ret = GetLastError();
		dprint("Duplicate handle  failed %d\n", ret);
	}
	CloseHandle(hWritePipe);

	wchar_t path[2048];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wsprintf(&path[wcslen(path)], L"\\LanProcess.exe %d ", (int)(__int64)hInheritWritePipe);
	LanParametersTable &table = Singleton<LanParametersTable>::Instance();
	wchar_t *p = &path[wcslen(path)];
	VL::foreach<LanParametersTable::items_list, LanRead__params__>()(table.items, p);

	if (!RunExecute(path))
	{
		DWORD ret = GetLastError();
		dprint("Create process failed %d\n", ret);
	}

	CloseHandle(hInheritWritePipe);

	HANDLE t = 0;
	CreateTimerQueueTimer(&t, NULL, WaitCallBack, (PVOID)t, 3000, 0, WT_EXECUTEONLYONCE);
}

void LanRead::Read()
{
	DWORD bytesReaded = 0;
	if (ReadFile(
		hReadPipe
		, &data.buffer[data.framesCount]
		, bufSize
		, &bytesReaded
		, NULL
	))
	{
		if (bytesReaded > 0)
		{
			unsigned t = data.framesCount + bytesReaded;
			if (t < data.buffSize) data.framesCount = t;

			if (data.offsetsTickCount < dimention_of(data.offsetsTick))
			{
				data.offsetsTick[data.offsetsTickCount] = Performance::Counter();
				++data.offsetsTickCount;
			}
		}
	}
	else
	{
		DWORD ret = GetLastError();
		dprint("Read from the pipe failed %d\n", ret);
		Automat::Stop();
		Stop();
	}
}

void LanRead::Start()
{
	if (start)return;
	start = true;
	data.framesCount = 0;
	data.offsetsTickCount = 0;
	data.strobesTickCount = 0;

	SetEvent(hStart);
	while(0 != ResumeThread(hThread));
	dprint("LanRead::Start()\n");
}

void LanRead::Stop()
{
	if (!start)return;
	start = false;
	SetEvent(hStop);
	SuspendThread(hThread);
	dprint("LanRead::Stop()\n");
}

void LanRead::Reload()
{
	if (CommonApp::TestProcessLan())
	{
		SetEvent(hExit);
		Sleep(1000);
	}
	Update();
}
