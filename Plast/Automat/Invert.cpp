#include "Invert.h"
#include "Units/FR_E700/FR_E700.h"
#include "App/AppBase.h"

FR_E700::Mode m;
FR_E700::Reset r;
FR_E700::GetFrequency gf;
FR_E700::SetFrequency sf;
FR_E700::SetState ss;
FR_E700::GetState gs;

void(*ptr)();

void reset();
void get_frequency();
void set_state_frequency();
void set_state();

void mode()
{
	m.Init();
	ptr = reset;
}
void reset()
{
	if (ComPortHandler::status == FR_E700::inverter_mode_did_not_work)throw Invert::Alarm();
	if (ComPortHandler::status >= FR_E700::max_alarm)throw Invert::Alarm();
	if (ComPortHandler::status != FR_E700::start_query)
	{
		r.Init();
		ptr = get_frequency;
	}
}
void get_frequency()
{
	if (ComPortHandler::status == FR_E700::inverter_reset_request_did_not_work)throw Invert::Alarm();
	if (ComPortHandler::status >= FR_E700::max_alarm)throw Invert::Alarm();
	if (ComPortHandler::status != FR_E700::start_query)
	{
		gf.Init();
		ptr = set_state_frequency;
	}
}
void set_state_frequency()
{
	if (ComPortHandler::status == FR_E700::get_frequency_did_not_work)throw Invert::Alarm();
	if (ComPortHandler::status >= FR_E700::max_alarm)throw Invert::Alarm();
	if (ComPortHandler::status != FR_E700::start_query)
	{
		if (gf.frequency != Singleton<ComPortTable>::Instance().items.get<InverterFrequency>().value)
		{
			sf.Init();
			ptr = set_state;
		}
		else
		{
			ss.Init(FR_E700::WriteState::STF | FR_E700::WriteState::RH);
			ptr = NULL;
		}
	}
}
void set_state()
{
	if (ComPortHandler::status == FR_E700::speed_​​setting_request_did_not_work)throw Invert::Alarm();
	if(ComPortHandler::status >= FR_E700::max_alarm)throw Invert::Alarm();
	if (ComPortHandler::status != FR_E700::start_query)
	{
		ss.Init(FR_E700::WriteState::STF | FR_E700::WriteState::RH);
		ptr = NULL;
	}
}

void Invert::Init()
{
	ptr = mode;
}

void Invert::Start()
{
	if(ptr)(ptr)();
}

void Invert::Run()
{
	gs.Init();
}

void Invert::Stop()
{
	ss.Init(FR_E700::WriteState::none);
	ss.Init(FR_E700::WriteState::none);
}
