#include "Emulator.h"
#include <windows.h>
#include "templates/typelist.hpp"
#include "Devices/1730Parameters.h"

bool terminated;

DWORD WINAPI __emulator__(PVOID)
{
	terminated = false;
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	int count = 0;
	Emulator::iputBits = 0;
	unsigned bit = Singleton<InputBitsTable>::Instance().items.get<iStrobe>().value;
	while (!terminated)
	{
		switch (WaitForSingleObject(hEvent, 100))
		{
		case WAIT_TIMEOUT:
			++count;
			switch (count)
			{
			case 10:
				Emulator::iputBits |= Singleton<InputBitsTable>::Instance().items.get<iIn>().value;
				break;
			case 30:
				Emulator::iputBits |= Singleton<InputBitsTable>::Instance().items.get<iOut>().value;
				break;
			case 200:
				Emulator::iputBits &= ~Singleton<InputBitsTable>::Instance().items.get<iIn>().value;
				break;
			case 230:
				Emulator::iputBits &= ~Singleton<InputBitsTable>::Instance().items.get<iOut>().value;
				break;
			}

		}
		if (count >= 10 && count < 231)
		{
			if (0 == (count % 5))
			{
				Emulator::iputBits |= bit;
			}
			else if (1 == (count % 5))
			{
				Emulator::iputBits &= ~bit;
			}
		}
	}
	return 0;
}

Emulator::Emulator()
{
	CloseHandle(CreateThread(NULL, 0, __emulator__, 0, 0, NULL));
}

Emulator::~Emulator()
{
	terminated = true;
}

unsigned Emulator::iputBits = 0;
