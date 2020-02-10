#include "Automat.h"
#include "Automat.hpp"

HANDLE Key<StartBtn>::Event;
HANDLE Key<StopBtn>::Event;
HANDLE Key<ContineBtn>::Event;

namespace Automat
{	
	HANDLE WaitFor<Vlst<>>::Event;

	void Init() 
	{
		Key<StartBtn>::Event   = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<StopBtn>::Event	 = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<ContineBtn>::Event = CreateEvent(NULL, FALSE, FALSE, NULL);
		WaitFor<Vlst<>>::Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	void Destroy() 
	{
		CloseHandle(Key<StartBtn>::Event	 );
		CloseHandle(Key<StopBtn>::Event	 );
		CloseHandle(Key<ContineBtn>::Event );
		CloseHandle(WaitFor<Vlst<>>::Event);
	}

	void Run()
	{
		Log::Mess<MessWrap<Off<iCU>>>();
		
		while (true)
		{
			try
			{
				Bits<Off<iCU>>();
			}
			catch (TimeOutExteption)
			{
				Log::Mess<MessWrap<TimeOutExteption>>();
			}
			catch (AlarmBitsExteption)
			{
				Log::Mess<MessWrap<AlarmBitsExteption>>();
			}
			catch (ExitLoopExteption)
			{
				Log::Mess<MessWrap<ExitLoopExteption>>();
				break;
			}
		}
	}
}