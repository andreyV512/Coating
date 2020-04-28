#include "LanDevice.h"
#include "App/App.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"
#include "Devices/LanParameters.h"
#include "Units/Lir/Lir.h"
#include "Units/Lan/Lan.h"
#include "tools_debug/DebugMess.h"

LanDevice::LanDevice(int numberPackets)
	: numberPackets(numberPackets)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, data(Singleton<Data::InputData>::Instance())
	, lir(Singleton<Lir>::Instance())
	, lan(Singleton<Lan>::Instance())
{
	data.countFrames = 0;
}

int LanDevice::Buff(char *&buf)
{
	int sizeBuf = packetSize * numberPackets * App::count_sensors;
	if (data.countFrames + sizeBuf < dimention_of(data.buffer))
	{
		buf = &data.buffer[data.countFrames];
		data.countFrames += sizeBuf;
		lir.Tick();
		return sizeBuf;
	}
	return 0;
}

CollectionData::CollectionData(int numberPackets)
	: device(numberPackets)
{
	device.lan.SetHandler(&device, &LanDevice::Buff);
	RshInitMemory p{};
	device.lan.SetParams(p);
	p.packetNumber = numberPackets * App::count_sensors;
	U32 st = device.lan.Init(1, device.lan.device1, p);
	wchar_t mess[256];
	if (device.lan.Err(st, mess))
	{
		char m[256];
		wcstombs(m, mess, dimention_of(m));
		dprint("1 %s\n", m);
		return;
	}
	st = device.lan.Init(2, device.lan.device2, p);
	if (device.lan.Err(st, mess))
	{
		char m[256];
		wcstombs(m, mess, dimention_of(m));
		dprint("2 %s\n", m);
		return;
	}
	device.lir.Clear();
	device.lan.Start();
}

CollectionData::~CollectionData()
{
	device.lan.Stop();
}

void CollectionData::ChangeLir()
{
	device.lir.Change();
}
