#pragma once
#include "window_tool\message.h"
class TEvent
{
public:
	virtual void Do(TCommand &)
	{
	}
	virtual LRESULT Do(TNotify &)
	{
		return 0; 
	}
};

//class TEventCommand
//{
//public:
//	virtual void Do(TCommand &) = 0;// {}
//};
//class TEventNotify
//{
//public:
//	virtual LRESULT Do(TNotify &) { return 0; }
//};