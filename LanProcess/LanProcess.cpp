#include <windows.h>
#include "Units/Lan/Lan.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "LanDirect/LanDirect.h"
#include "tools_debug/DebugMess.h"
#include "CommonApp.h"
#include "templates\templates.hpp"

class LanProcess
{
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
{
	hStart = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStart);
	hStop = OpenEvent(EVENT_ALL_ACCESS, TRUE, wStop);
	hExit = OpenEvent(EVENT_ALL_ACCESS, TRUE, wExit);
	bufSize = packetSize * numberPackets * App::count_sensors;
	data = new char[bufSize];

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
	buf = data;
	return bufSize;
}

void LanProcess::Confirm(unsigned b)
{
	DWORD bytesWritten;
	if (!WriteFile(hWritePipe, data, b, &bytesWritten, NULL))
	{
		DWORD ret = GetLastError();
		dprint("client WriteFalted %d\n", ret);
		SetEvent(hExit);
	}
	dprint("%d %d\n", b, bytesWritten);
}

void InitBase()
{
	ParametersBase param;
	CBase base(
		(wchar_t *)param.name()
	);
	if (base.IsOpen())
	{
		LanParametersTable &o = Singleton<LanParametersTable>::Instance();
		Select<LanParametersTable>(base).ID(__id__).Execute(o);
	}
}

template<class O, class P>struct __params__
{
	void operator()(O &o)
	{
		Wchar_from<typename O::type_value> p(o.value);
		dprint("%S %S\n", o.name(), p());
	}
};

int main(int argc, char **argv)
{
	if (!CommonApp::IsAppRun()) return 0;
	Initialize initialize;
	InitBase();
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
	
	LanProcess lan((HANDLE)atoi(argv[1]));
	l.SetHandler(&lan, &LanProcess::Buff, &LanProcess::Confirm);

	LanParametersTable &table = Singleton<LanParametersTable>::Instance();
	VL::foreach<LanParametersTable::items_list, __params__>()(table.items);

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
		case 2 + WAIT_OBJECT_0: return 0;
		case WAIT_TIMEOUT:
			if (!CommonApp::IsAppRun()) return 0;
			break;
		}
	}
	return 0;
}
