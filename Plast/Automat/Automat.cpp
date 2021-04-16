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
				device1730.Write(0);	  //очистка выходных бит 1730
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
				Bits<TstOn<iCU> >(); //проверка цепей управления (если iCU = 0 - выход из цикла )
				Log::Mess <LogMess::On_iIn>();	   //вывод сообщения
				Bits<On<iIn>, Key<StopBtn>, Proc<iStrobe>>(); //ожидает пока iIn = 0, выход из цикла кнопке стоп, записывает время появления строба
				{
					Log::Mess <LogMess::Collection>(); //вывод сообщения - сбор данных

					CollectionData collection; //сбор кадров платой Lan10
					Singleton<Compute>::Instance().Start();//начало расчёта собранных данных

					/*
					* ждёт пока iOut = 0, выход из цикла по кнопке "Стоп", расчёт собранных кадров,
					* записывает время появления строба, если время превышено 120 сек - выход по ошибке
					* вывод на экран собранных зон
					*/
					Bits<On<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					/*
					* ждёт пока iIn = 1, выход из цикла по кнопке "Стоп", расчёт собранных кадров,
					* записывает время появления строба, если время превышено 120 сек - выход по ошибке
					* вывод на экран собранных зон
					*/
					Bits<Off<iIn>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(120 * 1000);
					/*
					* ждёт пока iOut = 1, выход из цикла по кнопке "Стоп", расчёт собранных кадров,
					* записывает время появления строба, если время превышено 20 сек - выход по ошибке
					* вывод на экран собранных зон
					*/
					Bits<Off<iOut>, Key<StopBtn>, Proc<Compute>, Proc<iStrobe>>(20 * 1000);
				}

				Singleton<Compute>::Instance().Update();  //вывод на экран собранных зон
			    Done();	//сохранение в базе данных результата контроля, сохранение первичного сигнала в файле

				Log::Mess <LogMess::CollectionDone>();
				dprint("loop\n");
				Sleep(5000);
			}
			catch (TimeOutExteption)
			{
				//выход по превышению времени ожидания
				Log::Mess<LogMess::TimeOutExteption>();
				startLoop = true;
			}
			catch (AlarmBitsExteption)
			{
				//выход по авари
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