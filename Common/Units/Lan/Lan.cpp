#include "Lan.h"
#include "PerformanceCounter/PerformanceCounter.h"
#include "Units/Lan/INTERFACES/IRshDevice.h"
#include "Units/Lan/INTERFACES/RshDllClient.h"
#include "Units/Lan/INTERFACES/RshError.h"
//--------------------------------------------------------------------
Lan::Lan(): ptr(0) {}
//---------------------------------------------------------------------
void Lan::DataCollection()
{
}
//---------------------------------------------------------------------
void Lan::Init()
{

}
//---------------------------------------------------------------------
void Lan::Destroy()
{
}
//---------------------------------------------------------------------
void Lan::Preparation()
{
}
//---------------------------------------------------------------------
void Lan::Do()	
{
	if(ptr)(this->*ptr)();
}
//---------------------------------------------------------------------
void Lan::Start()
{
	timeBeforeStop = Performance::Counter();
	ptr = &Lan::DataCollection;
}
//---------------------------------------------------------------------
void Lan::Stop()
{
#pragma message("Остановить плату Lan")
	ptr = 0;
}

#define a(tpe)Singleton<LanTable>::Instance().get<tpe>().value

unsigned Lan::SetParams()
{
	RshInitMemory  p;
	p.startType = RshInitMemory::External;
	p.controlSynchro = 0;//Синхронизация по фронту
	if (Singleton<LanParametersTable>::Instance().items.get<SynchronizationEdge>().value)p.controlSynchro = RshInitMemory::SlopeDecline;
	p.bufferSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	p.frequency = 1e6 * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value;
	p.channels[0].control = RshChannel::Used;
	if (Singleton<LanParametersTable>::Instance().items.get<MeasurementInput>().value)p.channels[0].control |= RshChannel::AC;
	p.channels[0].gain = Singleton<LanParametersTable>::Instance().items.get<Gain0>().value;

	p.packetNumber = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value * count_sensors;

	p.channelSynchro.gain = Singleton<LanParametersTable>::Instance().items.get<SyncGain>().value;
	p.channelSynchro.control = RshSynchroChannel::DC;
	if (Singleton<LanParametersTable>::Instance().items.get<SyncInput>().value)p.channelSynchro.control = RshSynchroChannel::AC;
	p.threshold = Singleton<LanParametersTable>::Instance().items.get<SyncLevel>().value;
	p.startDelay = Singleton<LanParametersTable>::Instance().items.get<StartDelay>().value;
#ifndef DEBUG_ITEMS
	return device->Init(&p);
#else
	return RSH_API_SUCCESS;
#endif
}
//-----------------------------------------------------------------------