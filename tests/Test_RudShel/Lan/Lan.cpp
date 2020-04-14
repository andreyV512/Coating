#include "stdafx.h"
#ifndef DEBUG_ITEMS
#include "Lan.h"
#include "AppBase.h"
#include "LanParameters.h"
static const int count_sensors = 6;
//--------------------------------------------------------------------
Lan::Lan(): bufSize(bufSize_), ptr(0) {}
//---------------------------------------------------------------------
#ifdef POLLING
void Lan::DataCollection()
{
	if(start1)
	{
		ADC1->Start();
		start1 = false;
	}
	else
	{
		int m_nReady = 0;
		ADC1->Get(ADC_GET_READY, &m_nReady);
		if(timeGetTime() - timeBeforeStop > 2500) 
		{
			ptr = NULL;//&Lan::Error;
			return;
		}
		if(m_nReady)
		{
			start1 = true;
			short *data = primaryData.CurrentData();
			ADC1->GetData(ADC_DATA_MODE_DATACH0 | ADC_DATA_MODE_CONVERT2INT16, data, bufSize, 0);
			timeBeforeStop = timeGetTime();
			primaryData.IncrementOffset();
			compute.DataReady();
		}
	}

	if(start2)
	{
		ADC2->Start();
		start2 = false;
	}
	else
	{
		int m_nReady = 0;
		ADC2->Get(ADC_GET_READY, &m_nReady);
		if(timeGetTime() - timeBeforeStop > 2500) 
		{
			ptr = NULL;//&Lan::Error;
			return;
		}
		if(m_nReady)
		{
			start2 = true;
			short *data = primaryData.CurrentData();
			ADC2->GetData(ADC_DATA_MODE_DATACH0 | ADC_DATA_MODE_CONVERT2INT16, data, bufSize, 0);
			timeBeforeStop = timeGetTime();
			primaryData.IncrementOffset();
			compute.DataReady();
		}
	}
}
#else
short data[50 * 484];
void Lan::DataCollection()
{
	switch(WaitForMultipleObjects(2, hEvent, FALSE, 10))
	{
	case WAIT_OBJECT_0:
		{
		    missedCycles = 0;
			ADC1->GetData(ADC_DATA_MODE_DATACH0 | ADC_DATA_MODE_CONVERT2INT16, data, bufSize, 0);
			ADC1->Start();
		}
		break;
	case WAIT_OBJECT_0 + 1:
		{
	    	missedCycles = 0;
			ADC2->GetData(ADC_DATA_MODE_DATACH0 | ADC_DATA_MODE_CONVERT2INT16, data, bufSize, 0);
			ADC2->Start();
		}
		break;
	case WAIT_TIMEOUT:
		if(++missedCycles > 100)
		{
			ptr = NULL;
		}
		break;
	}
}
#endif
//---------------------------------------------------------------------
bool Lan::Init()
{
	IFactory *LAxFactory = (IFactory *)LAxClient.LoadRegistered(_T(CompanyNameDRV),
		_T(ProductNameDRV), _T(VersionNameDRV), L"LAn10M8PCI");
	if(LAxFactory == 0) return false;

	ADC1 = (IADCDevice *)LAxFactory->Create((wchar_t *)"IADCDevice", 0);
	if(ADC1 == 0) return false;
	ADC2 = (IADCDevice *)LAxFactory->Create((wchar_t *)"IADCDevice", 0);
	if(ADC2 == 0) return false;
#ifdef POLLING
	ADC1->Setup(1, 1, 0, 0);
	ADC2->Setup(2, 1, 0, 0);
#else
	hEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	hEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	ADC1->Setup(1, 1, 1, hEvent[0]);
	ADC2->Setup(2, 1, 1, hEvent[1]);
#endif
	
	return true;
}
//---------------------------------------------------------------------
void Lan::Destroy()
{
  if(ADC1){ ADC1->Release(); ADC1 = 0;}
  if(ADC2){ ADC2->Release(); ADC2 = 0;}
#ifndef POLLING
  CloseHandle(hEvent[0]);
  CloseHandle(hEvent[1]);
#endif
  LAxClient.Free();
}
//---------------------------------------------------------------------
bool Lan::Do()	
{
	if(ptr) 
	{
	   (this->*ptr)();
	   return false;
	}
	return true;
}
//---------------------------------------------------------------------
void Lan::Start()
{
#ifdef POLLING
	start1 = true;
	start2 = true;
	timeBeforeStop = timeGetTime();
#else
	missedCycles = 0;
	ADC1->Start();
	ADC2->Start();
#endif
	ptr = &Lan::DataCollection;
}
//---------------------------------------------------------------------
void Lan::Stop()
{
	ptr = 0;
	ADC1->Stop();
	ADC2->Stop();
}
//-----------------------------------------------------------------------
void Lan::InitParameters()
{
	ADCParametersMEMORY1 a;
	a.m_nControl = ADC_CONTROL_SYNC_TTL;
	if(Singleton<LanParametersTable>::Instance().items.get<SynchronizationEdge>().value) a.m_nControl |= ADC_CONTROL_SYNC_FRONT;  
	if(Singleton<LanParametersTable>::Instance().items.get<SyncInput>().value)a.m_nControl |= ADC_CONTROL_DIFS; 
	if(Singleton<LanParametersTable>::Instance().items.get<MeasurementInput>().value)a.m_nControl |= ADC_CONTROL_DIF0;
	a.m_nStartOf = ADCPARAM_START_COMP;
	a.m_fFreq = 1000000.0f * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value;
	a.m_nPacketSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	a.m_nPacketNumber = count_sensors * Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	a.m_nBeforeHistory = 0;
	a.m_nDelta[0] = 0;
	a.m_nGain[0] = Singleton<LanParametersTable>::Instance().items.get<Gain0>().value;
	a.m_nDelta[1] = 0;
	a.m_nGain[1] = 0;
	a.m_nSyncLevel = Singleton<LanParametersTable>::Instance().items.get<SyncLevel>().value;
	a.m_nSyncGain = Singleton<LanParametersTable>::Instance().items.get<SyncGain>().value;
	a.m_nStartDelay = Singleton<LanParametersTable>::Instance().items.get<StartDelay>().value;
	float freq; 
    ADC1->Init(ADC_INIT_MODE_INIT, &a, &freq);
	ADC2->Init(ADC_INIT_MODE_INIT, &a, &freq);
	bufSize_ = a.m_nPacketSize * a.m_nPacketNumber;
}
//----------------------------------------------------------------------------
Lan lan;
#endif