#include <windows.h>
#include "Units/Lan/Lan.h"
#include "App/AppBase.h"
#include "LanDirect/LanDirect.h"

char buf[8154] = {};

int main(int argc, char **argv)
{
    AppBase().Init();
	//инициализация АЦП
	Lan &l = Singleton<Lan>::Instance();
	RshInitMemory p{};
	l.SetParams(p);
	wchar_t mess[256];
	U32 st = l.Init(1, l.device1, p);
	if (RSH_API_SUCCESS != st)
	{
		l.Err(st, mess);
		MessageBox(NULL, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(1)!!!", MB_ICONEXCLAMATION);
		return 0;
	}
	st = l.Init(2, l.device2, p);
	if (RSH_API_SUCCESS != st)
	{
		l.Err(st, mess);
		MessageBox(NULL, mess, (wchar_t *)L"Ошибка платы La-n108-100PCI(2)!!!", MB_ICONEXCLAMATION);
		return 0;
	}

	LanProcess lan((HANDLE)atoi(argv[1]));

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
			RshInitMemory p{};
			l.SetParams(p);
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
