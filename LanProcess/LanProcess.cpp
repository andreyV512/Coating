#include <windows.h>
#include "Units/Lan/Lan.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "LanDirect/LanDirect.h"
#include "tools_debug/DebugMess.h"

class LanProcess
{
public:
	int &numberPackets, &packetSize, bufSize;
public:
	HANDLE hStart, hStop;
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
	bufSize = packetSize * numberPackets * App::count_sensors;
	data = new char[bufSize];
}

LanProcess::~LanProcess()
{
	CloseHandle(hStart);
	CloseHandle(hStop);
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
	}
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

int main(int argc, char **argv)
{
	Initialize initialize;
	InitBase();
	//инициализация АЦП
	Lan &l = Singleton<Lan>::Instance();
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

	HANDLE h[] = {
		lan.hStart
		, lan.hStop
	};

	while (true)
	{
		switch (WaitForMultipleObjects(2, h, FALSE, INFINITE))
		{
		case 0 + WAIT_OBJECT_0:
		{
			InitBase();

			RshInitMemory p{};
			l.SetParams(p);
			U32 st;
			st = l.device1->Init(&p);
			if (RSH_API_SUCCESS != st)
			{
				wchar_t mess[256];
				l.Err(st, mess);
				dprint("1 %S\n", mess);
				return 0;
			}
			st = l.device2->Init(&p);
			if (RSH_API_SUCCESS != st)
			{
				wchar_t mess[256];
				l.Err(st, mess);
				dprint("2 %S\n", mess);
				return 0;
			}

			int t = lan.packetSize * lan.numberPackets * App::count_sensors;
			if(t > lan.bufSize)
			{ 
				delete [] lan.data;
				lan.bufSize = t;
				lan.data = new char[lan.bufSize];
			}

			l.Start();
		}
		break;
		case 1 + WAIT_OBJECT_0:
			l.Stop();
			break;
		}

	}
	return 0;
}
