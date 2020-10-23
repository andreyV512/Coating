#include "Automat.h"
#include "Automat.hpp"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Windows/MainWindow/MainWindow.h"
#include "tools_debug/DebugMess.h"
#include "Compute/Compute.h"
#include "Devices/LanDevice.h"

template<> struct Proc<Compute>
{
	template<class P>void operator()(P &p)
	{
		static unsigned last = 0;
		unsigned first = Performance::Counter();
		if (first - last > 1000)
		{
			last = first;
			Singleton<Compute>::Instance().Update();
		}
	}
};

template<> struct Proc<iStrobe>
{
	static bool pred;
	Data::InputData &data;
	unsigned bit;
	Proc() 
		: data(Singleton<Data::InputData>::Instance())
		, bit(Singleton<InputBitsTable>::Instance().items.get<iStrobe>().value)
	{}

	template<class P>void operator()(P &p) 
	{
		bool b = 0 != (p.bits & bit);
		if (!pred && b)
		{
			if (++data.strobesTickCount > dimention_of(data.strobesTick))  data.strobesTickCount = dimention_of(data.strobesTick) - 1;
			data.strobesTick[data.strobesTickCount] = Performance::Counter();
		}
		pred = b;
	}
};
bool Proc<iStrobe>::pred = false;

namespace Automat
{	
	HANDLE Key<StartBtn>::hEvent;
	HANDLE Key<StopBtn>::hEvent;
	HANDLE Key<ContineBtn>::hEvent;

	HANDLE WaitFor<Vlst<>>::hEvent;
	HANDLE hThread;
	
	DWORD WINAPI Loop(PVOID);
	void Init() 
	{
		Key<StartBtn>::hEvent   = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<StopBtn>::hEvent	 = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<ContineBtn>::hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		WaitFor<Vlst<>>::hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = CreateThread(NULL, 0, Loop, NULL, 0, NULL);
	}
	void Destroy() 
	{
		CloseHandle(Key<StartBtn>::hEvent	 );
		CloseHandle(Key<StopBtn>::hEvent	 );
		CloseHandle(Key<ContineBtn>::hEvent );
		CloseHandle(WaitFor<Vlst<>>::hEvent);
		CloseHandle(hThread);
	}

	void Start()
	{
		SetEvent(Key<StartBtn>::hEvent);
	}

	void Stop()
	{
		SetEvent(Key<StopBtn>::hEvent);
	}
	

	DWORD WINAPI Loop(PVOID)
	{
		Compute &compute = Singleton<Compute>::Instance();
		bool startLoop = true;
		while (true)
		{
			try
			{
				device1730.Write(0);
				if (startLoop)
				{
					App::IsRun() = false;
					MainWindow::EnableMenu(true);
					AppKeyHandler::Stop();
					auto x = Bits<Key<StartBtn>>();
					App::IsRun() = true;
					MainWindow::EnableMenu(false);
					AppKeyHandler::Run();
					startLoop = false;
				}
				Proc<iStrobe>::pred = false;

				//TODO Bits<TstOn<iCU> >(); //проверка цепей управления
				Log::Mess <LogMess::On_iIn>();
				Bits<On<iIn>, Key<StopBtn>, Proc<iStrobe>>();
				{
					Log::Mess <LogMess::Collection>();

					CollectionData collection; 

					Bits<On<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					Bits<Off<iIn>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					Bits<Off<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(20 * 1000);
				}
				Log::Mess <LogMess::CollectionDone>();
				compute.Done();
				dprint("loop\n");
				Sleep(5000);
			}
			catch (TimeOutExteption)
			{
				Log::Mess<LogMess::TimeOutExteption>();
				startLoop = true;
			}
			catch (AlarmBitsExteption)
			{
			//	Log::Mess<LogMess::AlarmBitsExteption>();
				startLoop = true;
			}
			catch (ExitLoopExteption)
			{
				Log::Mess<LogMess::ExitLoopExteption>();
				break;
			}
			catch (StopBtn)
			{
				Log::Mess<LogMess::StopBtn>();
				startLoop = true;
			}
			
		}
		zprint(" exit loop\n");
		return 0;
	}
}