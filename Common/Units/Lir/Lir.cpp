#include "Lir.h"
#include "App/Config.h"
#include <Windows.h>
#include <MMSystem.h>
#include "App/AppBase.h"
#include "inc/bdaqctrl.h"
#include "templates/typelist.hpp"
//#include "AnalogBoard.h"
#include "tools_debug/DebugMess.h"
using namespace Automation::BDaq;

#define cnt0 ((UdCounterCtrl *)cnt0_)
#define cnt1 ((UdCounterCtrl *)cnt1_)

Lir::Lir()
	: dx0(Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir0>().value)
	, dx1(Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir1>().value)
{ }

bool Lir::Init(int id)
{
#ifndef DEBUG_ITEMS
	cnt0_  = (void *)AdxUdCounterCtrlCreate();
	cnt1_  = (void *)AdxUdCounterCtrlCreate();

	DeviceInformation devInfo(id);
	ErrorCode ret = cnt0->setSelectedDevice(devInfo);
	if(Success == ret)ret = cnt1->setSelectedDevice(devInfo);
	if(Success == ret)ret = cnt0->setChannel(0);
	if(Success == ret)ret = cnt1->setChannel(1);
	if(Success == ret)ret = cnt0->setCountingType(PulseDirection);
	if(Success == ret)ret = cnt1->setCountingType(PulseDirection);
	if(Success == ret)ret= cnt0->setEnabled(true);
	if(Success == ret)ret= cnt1->setEnabled(true);
	return Success == ret;
#else
	return true;
#endif
}
void Lir::Clear()   
{
	cnt0->setInitialValue(0);
	cnt1->setInitialValue(0);
	ptrTick = &Lir::Tick0;
}

double Lir::Tick()
{
	return (this->*ptrTick)();
}

void Lir::Change()
{
	ptrTick = &Lir::Tick1;
}

double Lir::Tick0()
{
	tmpTick = cnt1->getValue();
	return dx0 * (tick = cnt0->getValue());	
}
double Lir::Tick1()
{
	return dx0 * tick + dx1 * (cnt1->getValue() - tmpTick);
}

#undef cnt0
#undef cnt1