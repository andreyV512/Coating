#include <windows.h>
#include "Units/Lan/Lan.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "LanDirect/LanDirect.h"
#include "tools_debug/DebugMess.h"
#include "CommonApp.h"
#include "templates\templates.hpp"
#include "LanDirect\EventNames.h"

class LanProcess
{
	static const int maxFrames = 16;
	int currentFrameHead, currentFrameTail;
public:
	int &numberPackets, &packetSize, bufSize;
public:
	HANDLE hStart, hStop, hExit;
private:
	HANDLE hWritePipe;
public:
	char *data;
public:
	LanProcess(HANDLE hWritePipe);
	~LanProcess();
	int Buff(char *&buf);
	void Confirm(unsigned b);
};

LanProcess::LanProcess(HANDLE hWritePipe)
	: numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, hWritePipe(hWritePipe)
	, currentFrameHead(0)
	, currentFrameTail(0)
{
	hStart = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStart);
	hStop = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStop);
	hExit = OpenEvent(EVENT_ALL_ACCESS, TRUE, wExit);
	bufSize = packetSize * numberPackets * App::count_sensors;
	data = new char[bufSize * maxFrames];

}

LanProcess::~LanProcess()
{
	CloseHandle(hStart);
	CloseHandle(hStop);
	CloseHandle(hExit);
	delete[] data;
}

int LanProcess::Buff(char *&buf)
{
	buf = &data[bufSize * currentFrameHead];
	++currentFrameHead;
	currentFrameHead %= maxFrames;
	return bufSize;
}

void LanProcess::Confirm(unsigned)
{
	DWORD bytesWritten;
	while (currentFrameHead != currentFrameTail)
	{
		char *c = &data[bufSize * currentFrameTail];
		int k = 0;
		for (int i = 0; i < bufSize; i += packetSize, ++k)
		{
			if (0 == (k % App::count_sensors))
			{
				char *x = &c[i];
				for (int j = 0; j < packetSize; ++j)
				{
					x[j] = 64;
				}
			}
		}
		if (!WriteFile(hWritePipe, c, bufSize, &bytesWritten, NULL))
		{
			DWORD ret = GetLastError();
			dprint("client WriteFalted %d\n", ret);
			SetEvent(hExit);
		}
		++currentFrameTail;
		currentFrameTail %= maxFrames;
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

void InitBase(int argc, char **argv)
{
	LanParametersTable &table = Singleton<LanParametersTable>::Instance();
	for (int i = 0; i < argc; ++i)
	{
		__set_params_data__ data(argv[i]);
		VL::find<LanParametersTable::items_list, __set_params__>()(table.items, data);
	}
}

int main(int argc, char **argv)
{
	dprint("START LAN<<<<<<<<<<");
	if (!CommonApp::TestAppRun()) return 0;
	dprint("START LAN>>>>>>>>>");
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
	};

	while (true)
	{
		switch (WaitForMultipleObjects(3, h, FALSE, 200))
		{
		case 0 + WAIT_OBJECT_0:
		{
			dprint("LanProcess start\n");
			l.Start();
		}
		break;
		case 1 + WAIT_OBJECT_0:
			dprint("LanProcess stop\n");
			l.Stop();
			break;
		case 2 + WAIT_OBJECT_0: 
			dprint("LanProcess EXIT\n");
			return 0;
		}
	}
	return 0;
}
