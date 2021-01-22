#include <windows.h>
#include "Units/Lan/Lan.h"
#include "Units/Lan/LanN.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "LanDirect/LanDirect.h"
#include "tools_debug/DebugMess.h"
#include "CommonApp.h"
#include "templates\templates.hpp"
#include "LanDirect\EventNames.h"

class LanProcess
{
public:
	static inline HANDLE hWritePipe;
	static inline HANDLE hExit;
	HANDLE hStart, hStop, hParams;
public:
	LanProcess();
	~LanProcess();
	static void SendData(char *);
	void Params(LanN &l);
};

LanProcess::LanProcess()
{
	hStart = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStart);
	hStop = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStop);
	hExit = OpenEvent(EVENT_ALL_ACCESS, TRUE, wExit);
	hParams = CreateEvent(NULL, FALSE, FALSE, wLanParams);
}

LanProcess::~LanProcess()
{
	CloseHandle(hStart);
	CloseHandle(hStop);
	CloseHandle(hExit);
	CloseHandle(hParams);
}

DWORD WINAPI ThreadProc(LPVOID data)
{
	DWORD bytesWritten;
	if (!WriteFile(LanProcess::hWritePipe, data, LanN::bufSize, &bytesWritten, NULL))
	{
		DWORD ret = GetLastError();
		dprint("client WriteFalted %d\n", ret);
		SetEvent(LanProcess::hExit);
	}
	return 0;
}

void LanProcess::SendData(char *data)
{
	QueueUserWorkItem(ThreadProc, data, WT_EXECUTEDEFAULT);
}

void LanProcess::Params(LanN &l)
{
	RSH_BUFFER_U32 buf;
	l.device->Get(RSH_GET_DEVICE_PACKET_LIST, &buf);

	int len = buf.size();
	dprint("len %d\n", len);
	for (int i = 2, k = 0; i < 8 && i < len; ++i, ++k)
	{
		dprint("%d packet_size %d\n", k, buf.ptr[i]);
	}

	HWND h = FindWindow(L"MainWindowCoating", NULL);
	if (NULL != h)
	{
		COPYDATASTRUCT ct;
		ct.cbData = 6 * sizeof(int);
		ct.dwData = ID_GET_PACKET_SIZE;
		ct.lpData = &buf.ptr[2];
		SendMessage(h, WM_COPYDATA, NULL, (LPARAM)&ct);
		dprint("FIND WINDOW MainWindowCoating %x\n", h);
	}
}

template<class O, class P>struct __params__
{
	void operator()(O &o)
	{
		Wchar_from<typename O::type_value> p(o.value);
		dprint("pipe %S %S\n", o.name(), p());
	}
};

struct __set_params_data__
{
	wchar_t buf[512];
	wchar_t *name, *value;
	__set_params_data__(char *val)
	{
		wchar_t *s = buf;
		mbstowcs(s, val, 512);
		name = s;
		while (*s && '=' != *s) ++s;
		*s = '\0';
		++s;
		value = s;
	}
};
template<class O, class P>struct __set_params__
{
	bool operator()(O &o, P &p)
	{
		if (0 == wcscmp(p.name, o.name()))
		{
			o.value = Wchar_to<typename O::type_value>()(p.value);
			return false;
		}
		return true;
	}
};

void InitBase(int argc, char **argv, LanParametersTable::TItems &items)
{
	for (int i = 0; i < argc; ++i)
	{
		__set_params_data__ data(argv[i]);
		VL::find<LanParametersTable::items_list, __set_params__>()(items, data);
	}
}

int main(int argc, char **argv)
{
#if 0
	dprint("START LAN<<<<<<<<<<\n");
	if (!CommonApp::TestAppRun()) return 0;
	dprint("START LAN>>>>>>>>>\n");
	InitBase(argc - 2, &argv[2]);

	LanParametersTable &table = Singleton<LanParametersTable>::Instance();
	VL::foreach<LanParametersTable::items_list, __params__>()(table.items);
	//инициализация АЦП
	Lan l;
	RshInitMemory p{};
	l.SetParams(p);
	
	U32 st = l.Init(1, l.device1, p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		l.Err(st, mess);
		MessageBox(NULL, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(1)!!!", MB_ICONEXCLAMATION | MB_TOPMOST);
		return 0;
	}
	st = l.Init(2, l.device2, p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		l.Err(st, mess);
		MessageBox(NULL, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(2)!!!", MB_ICONEXCLAMATION | MB_TOPMOST);
		return 0;
	}
	
	int handle = atoi(argv[1]);
	dprint("HANDLE %d\n", handle);
	LanProcess lan((HANDLE)handle);

	l.SetHandler(&lan, &LanProcess::Buff, &LanProcess::Confirm);

	HANDLE h[] = {
		lan.hStart
		, lan.hStop
		, lan.hExit
		, lan.hParams
	};

	while (true)
	{
		switch (WaitForMultipleObjects(4, h, FALSE, 200))
		{
		case 0 + WAIT_OBJECT_0:
			dprint("LanProcess start\n");
			lan.currentFrameHead = lan.currentFrameTail = 0;
			l.Start();
		break;
		case 1 + WAIT_OBJECT_0:
			dprint("LanProcess stop\n");
			l.Stop();
			break;
		case 2 + WAIT_OBJECT_0: 
			dprint("LanProcess EXIT\n");
			return 0;
		case 3 + WAIT_OBJECT_0:
			dprint("LanProcess Params\n");
			lan.Params(l);
			break;
		}
	}
#else
	if (!CommonApp::TestAppRun()) return 0;

	LanParametersTable::TItems items;
	InitBase(argc - 2, &argv[2], items);
	VL::foreach<LanParametersTable::items_list, __params__>()(items);

	RshInitMemory p{};

	LanN::SetParams(p, items);

	RshDllClient client;
	LanN unit1, unit2;

	if (RSH_API_SUCCESS != unit1.Init(1, p, client)) return 0;
	if (RSH_API_SUCCESS != unit2.Init(2, p, client)) return 0;

	LanProcess::hWritePipe = (HANDLE)atoi(argv[1]);
	dprint("HANDLE %d\n", LanProcess::hWritePipe);

	unit1.ptr = LanProcess::SendData;
	unit2.ptr = LanProcess::SendData;

	LanProcess lan;
	HANDLE h[] = {
		lan.hStart
		, lan.hStop
		, lan.hExit
		, lan.hParams
	};

	while (true)
	{
		switch (WaitForMultipleObjects(4, h, FALSE, 200))
		{
		case 0 + WAIT_OBJECT_0:
			dprint("LanProcess start\n");
			unit1.Start();
			unit2.Start();
			break;
		case 1 + WAIT_OBJECT_0:
			dprint("LanProcess stop\n");
			unit1.Stop();
			unit2.Stop();
			break;
		case 2 + WAIT_OBJECT_0:
			dprint("LanProcess EXIT\n");
			return 0;
		case 3 + WAIT_OBJECT_0:
			dprint("LanProcess Params\n");
			lan.Params(unit1);
			break;
		}
	}

#endif
	return 0;
}
