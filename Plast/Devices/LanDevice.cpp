#include "LanDevice.h"
#include "App/App.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"
#include "Devices/LanParameters.h"
#include "Units/Lir/Lir.h"
#include "Units/Lan/Lan.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"

class LanDevice
{
public:
	int numberPackets;
	int packetSize;
	Data::InputData &data;
	class Lan &lan;
public:
	LanDevice();
	void Init();
	int Buff(char *&);
	void Confirm(unsigned);
	void Start();
	void Stop();
};

LanDevice::LanDevice()
	: numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, data(Singleton<Data::InputData>::Instance())
	, lan(Singleton<Lan>::Instance())
{
	data.framesCount = 0;
}

void LanDevice::Init()
{
	lan.SetHandler(
		this
		, &LanDevice::Buff
		, &LanDevice::Confirm
	);
	RshInitMemory p{};
	lan.SetParams(p);
	U32 st;
	st = lan.device1->Init(&p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		lan.Err(st, mess);
		dprint("1 %S\n", mess);
		return;
	}
	st = lan.device2->Init(&p);
	if (RSH_API_SUCCESS != st)
	{
		wchar_t mess[256];
		lan.Err(st, mess);
		dprint("2 %S\n", mess);
		return;
	}
	lan.Start();
	dprint("START LAN\n");
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

void LanDevice::Start()
{
	lan.Start();
}

void LanDevice::Stop()
{
	lan.Stop();
}

CollectionData::CollectionData()
	: device(Singleton<LanDevice>::Instance())
{
	device.Init();
}

CollectionData::~CollectionData()
{
	device.Stop();
	dprint("STOP LAN\n");
}


