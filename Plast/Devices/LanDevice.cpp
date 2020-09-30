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
	unsigned t = data.framesCount + b;
	if (t < dimention_of(data.buffer)) data.framesCount = t;
	
	if (data.offsetsTickCount < dimention_of(data.offsetsTick))
	{
		data.offsetsTick[data.offsetsTickCount] = Performance::Counter();
		++data.offsetsTickCount;
	}
}

CollectionData::CollectionData()
{
	device.lan.SetHandler(&device
		, &LanDevice::Buff
		, &LanDevice::Confirm
	);
	RshInitMemory p{};
	device.lan.SetParams(p);
	U32 st;
	st = device.lan.device1->Init(&p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		device.lan.Err(st, mess);
		dprint("1 %S\n", mess);
		return;
	}
	st = device.lan.device2->Init(&p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		device.lan.Err(st, mess);
		dprint("2 %S\n", mess);
		return;
	}
	device.lan.Start();
}

CollectionData::~CollectionData()
{
	device.lan.Stop();
}


