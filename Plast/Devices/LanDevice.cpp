#include "LanDevice.h"
#include "App/App.h"
#include "Data/Data.h"
#include "templates/typelist.hpp"
#include "Devices/LanParameters.h"
#include "Units/Lan/Lan.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "App/Config.h"
#include "../LanProcess/LanDirect/LanDirect.h"
#include "App/AppBase.h"

#ifdef INNER_LAN
class LanDevice
{
public:
	int numberPackets;
	int packetSize;
	Data::InputData &data;
	class Lan &lan;
public:
	LanDevice();
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
	lan.SetHandler(
		this
		, &LanDevice::Buff
		, &LanDevice::Confirm
	);
}

int LanDevice::Buff(char *&buf)
{
	unsigned short sizeBuf = packetSize * numberPackets * App::count_sensors;
	if (data.framesCount + sizeBuf < dimention_of(data.buffer))
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
#else
//class LanDevice : public LanRead {};
#endif

CollectionData::CollectionData()
	: device(Singleton<LanDevice>::Instance())
	, generatorBit(Singleton<OutputBitsTable>::Instance().items.get<oGenerator>().value)
{
	device1730.WriteOutput(generatorBit, 0);
	device.Start();
	dprint("START LAN\n");
}

CollectionData::~CollectionData()
{
	device1730.WriteOutput(0, generatorBit);
	device.Stop();
	dprint("STOP LAN\n");
}


