#include "Automat.h"
#include "Automat.hpp"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Windows/MainWindow/MainWindow.h"
#include "tools_debug/DebugMess.h"

HANDLE Key<StartBtn>::hEvent;
HANDLE Key<StopBtn>::hEvent;
HANDLE Key<ContineBtn>::hEvent;

namespace Automat
{	
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

				Bits<On<iCU>>(500);
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
				Log::Mess<LogMess::AlarmBitsExteption>();
			}
			catch (ExitLoopExteption)
			{
				Log::Mess<LogMess::ExitLoopExteption>();
				break;
			}
		}
		return 0;
	}
}