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
	while (!terminated)
	{
		switch (WaitForSingleObject(hEvent, 500))
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
			case 70:
				Emulator::iputBits &= ~Singleton<InputBitsTable>::Instance().items.get<iIn>().value;
				break;
			case 100:
				Emulator::iputBits &= ~Singleton<InputBitsTable>::Instance().items.get<iOut>().value;
				break;
			}

		}
		if (count >= 10 && 0 == (count % 5))
		{
			Emulator::iputBits ^= Singleton<InputBitsTable>::Instance().items.get<iStrobe>().value;
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
