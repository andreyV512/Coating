#include "LanDirect.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "Data/Data.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "window_tool/RunExecute.h"

const wchar_t *wStart = L"Communication pipe start";
const wchar_t *wStop  = L"Communication pipe stop";
const wchar_t *wExit = L"Communication pipe exit";

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
//	Stop();
//	SetEvent(hExit);
//CloseHandle(hReadPipe);
//Sleep(500);
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
}

void LanRead::Read()
{
	unsigned i = data.framesCount;
	DWORD bytesReaded;
	if (!ReadFile(
		hReadPipe
		, &data.buffer[i]
		, bufSize
		, &bytesReaded
		, NULL
	))
	{
		DWORD ret = GetLastError();
		dprint("Read from the pipe failed %d\n", ret);
	}
	unsigned t = i + bytesReaded;
	if (t < dimention_of(data.buffer)) data.framesCount = t;

	if (data.offsetsTickCount < dimention_of(data.offsetsTick))
	{
		data.offsetsTick[data.offsetsTickCount] = Performance::Counter();
		++data.offsetsTickCount;
	}
}

void LanRead::Start()
{
	SetEvent(hStart);
	while(ResumeThread(hThread));
	dprint("LanRead::Start()\n");
}

void LanRead::Stop()
{
	SetEvent(hStop);
	SuspendThread(hThread);
	dprint("LanRead::Stop()\n");
}
