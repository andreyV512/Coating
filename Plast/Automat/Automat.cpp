#include "Automat.h"
#include "Automat.hpp"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Windows/MainWindow/MainWindow.h"
#include "tools_debug/DebugMess.h"
#include "Compute/Compute.h"
#include "Devices/LanDevice.h"
#include "AdditionalAutomat.h"
#include "App/Config.h"
#include "Compute/Emulator/Emulator.h"

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

Proc<iStrobe>::Proc()
	: data(Singleton<Data::InputData>::Instance())
	, bit(Singleton<InputBitsTable>::Instance().items.get<iStrobe>().value)
{}
bool Proc<iStrobe>::pred = false;
unsigned Proc<iStrobe>::count = 0;

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
					Bits<Key<StartBtn>>();
					App::IsRun() = true;
					MainWindow::EnableMenu(false);
					AppKeyHandler::Run();
					startLoop = false;
				}
				Proc<iStrobe>::pred = false;
				Proc<iStrobe>::count = 1;
#ifdef EMULATOR
				Emulator emulator;
#endif
				// Bits<TstOn<iCU> >(); //проверка цепей управления
				Log::Mess <LogMess::On_iIn>();
				Bits<On<iIn>, Key<StopBtn>, Proc<iStrobe>>();
				////Singleton<Compute>::Instance().Start();
				{
					Log::Mess <LogMess::Collection>();

					CollectionData collection; 
					Singleton<Compute>::Instance().Start();


					Bits<On<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					Bits<Off<iIn>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					Bits<Off<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(20 * 1000);
				}

				Singleton<Compute>::Instance().Update();
			    Done();

				Log::Mess <LogMess::CollectionDone>();
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
		device1730.Write(0);
		return 0;
	}
}