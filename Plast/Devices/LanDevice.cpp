#include "LanDevice.h"
#include "App/App.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"
#include "Devices/LanParameters.h"
#include "Units/Lir/Lir.h"
#include "Units/Lan/Lan.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"

LanDevice::LanDevice()
	: numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, data(Singleton<Data::InputData>::Instance())
	, lan(Singleton<Lan>::Instance())
{
	data.framesCount = 0;
}

int LanDevice::Buff(char *&buf)
{
	unsigned short sizeBuf = packetSize * numberPackets * App::count_sensors;
	if ((__int64)data.framesCount + sizeBuf < dimention_of(data.buffer))
	{
		buf = &data.buffer[data.framesCount];
		return sizeBuf;
	}
	return 0;
}

void LanDevice::Confirm(unsigned b)
{
	data.framesCount += b;
	if (++data.offsetsTickCount >= dimention_of(data.offsetsTick))
	{
		data.offsetsTickCount = dimention_of(data.offsetsTick) - 1;
	}
	
	data.offsetsTick[data.offsetsTickCount] = Performance::Counter();
}

CollectionData::CollectionData()
{
	device.lan.SetHandler(&device
		, &LanDevice::Buff
		, &LanDevice::Confirm
	);
	RshInitMemory p{};
	device.lan.SetParams(p);
	U32 st = device.lan.Init(1, device.lan.device1, p);
	static const int buf_len = 256;
	wchar_t mess[buf_len];
	if (device.lan.Err(st, mess))
	{
		char m[buf_len];
		wcstombs(m, mess, buf_len);
		dprint("1 %s\n", m);
		return;
	}
	st = device.lan.Init(2, device.lan.device2, p);
	if (device.lan.Err(st, mess))
	{
		char m[buf_len];
		wcstombs(m, mess, buf_len);
		dprint("2 %s\n", m);
		return;
	}
	device.lan.Start();
}

CollectionData::~CollectionData()
{
	device.lan.Stop();
}


