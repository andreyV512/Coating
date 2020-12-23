#include "AppKeyHandler.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "templates/typelist.hpp"
#include "MainWindow.h"
#include "Resource.h"
#include "tools_debug/DebugMess.h"
#include "window_tool/Emptywindow.h"

namespace AppKeyHandler
{
	typedef Vlst<
		VL::IntToType<IDB_CycleBtn>
		, VL::IntToType<IDB_Reset>
		, VL::IntToType<IDB_QueryBtn>
		//, TL::IntToType<IDB_DownArrow>
		//, TL::IntToType<IDB_UpArrow>
		//, TL::IntToType<IDB_LeftArrow>
		//, TL::IntToType<IDB_RightArrow>
		//, TL::IntToType<IDB_ExitTubeBtn>
		//, VL::IntToType<IDB_Ok>
	> button_list;

	template<int ID>struct On {static const int ID = ID; static const bool state = true;};
	template<int ID>struct Off{static const int ID = ID; static const bool state = false;};
	template<class List>struct __btn__;
	template<class Head, class ...Tail>struct __btn__<Vlst<Head, Tail...> >
	{
		void operator()()
		{
			SendMessage(Singleton<MainWindow>::Instance().toolBar.hWnd, TB_ENABLEBUTTON, Head::ID, MAKELONG(Head::state, 0));
			__btn__<Tail...>()();
		}
	};
	
	template<>struct __btn__<Vlst<>>{void operator()(){}};

	bool IsEnabled(int id)
	{
		return 0 != SendMessage(Singleton<MainWindow>::Instance().toolBar.hWnd, TB_ISBUTTONENABLED, id, 0);
	}

	template<class List, template<int>class W>struct __all_button_xx__;
	template<class Head, class ...Tail, template<int>class W>struct __all_button_xx__<Vlst<Head, Tail...>, W>
	{
		typedef typename Vlst<W<Head::value>, typename __all_button_xx__<Tail..., W>::Result> Result;
	};
	template<template<int>class W>struct __all_button_xx__<Vlst<>, W>
	{
		typedef Vlst<> Result;
	};


	template<class List, class T>struct InList;
	template<class Head, class ...Tail, class T>struct InList<Vlst<Head, Tail...>, T>
	{
		static const bool value = InList<Vlst<Tail...>, T>::value;
	};
	template<class ...Tail, class T>struct InList<Vlst<T, Tail...>, T>
	{
		static const bool value = true;
	};
	template<class T>struct InList<Vlst<>, T>
	{
		static const bool value = false;
	};

	template<class List, class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__;
	template<class Head, class ...Tail, class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__<Vlst<Head, Tail...>, SubList, on, off>
	{
		typedef typename Vlst<
			typename VL::_if< InList<SubList, Head>::value
			  , on<Head::value>
			  , off<Head::value>
			>::Result
			, typename __all_button_OnOff__<Vlst<Tail...>, SubList, on, off>::Result
		> Result;
	};
	template<class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__<Vlst<>, SubList, on, off>
	{
		typedef Vlst<> Result;
	};

	void Init()
	{		
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , Vlst<VL::IntToType<IDB_Reset> >
			  , Off, On
			>::Result
		>()();
	}

	void Run()
	{		
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , Vlst<VL::IntToType<IDB_Reset> >
			  , On, Off
			>::Result
		>()();
		MainWindow::EnableMenu(false);
	}
	
	void Continue()
	{
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , Vlst<VL::IntToType<IDB_Reset>, VL::IntToType<IDB_CycleBtn>>
			  , On, Off
			>::Result
		>()();
	}
	
	void Stop()
	{
		Init();
		MainWindow::EnableMenu(true);
	}

	void DisableAll()
	{
		__btn__<
			__all_button_OnOff__<
			button_list
			, Vlst<>
			, On, Off
			>::Result
		>()();
	}

	void VK_(unsigned id)
	{
		MainWindow &mw = Singleton<MainWindow>::Instance();
		TCommand c(mw.hWnd, id, 0, mw.toolBar.hWnd);
		SendMessage(MESSAGE(c));
	}

	template<int id>struct __is_key__{static const unsigned value = 0;};

	template<>struct __is_key__<IDB_CycleBtn   >{static const unsigned value = VK_F4;};
	template<>struct __is_key__<IDB_Reset      >{static const unsigned value = VK_ESCAPE;};
	template<>struct __is_key__<IDB_DownArrow >{static const unsigned value = VK_F5;};
	template<>struct __is_key__<IDB_UpArrow   >{static const unsigned value = VK_F6;};
	template<>struct __is_key__<IDB_LeftArrow >{static const unsigned value = VK_F7;};
	template<>struct __is_key__<IDB_Ok>{static const unsigned value = VK_F8;};

	template<>struct __is_key__<IDB_ExitTubeBtn >{static const unsigned value = VK_F9;};

	template<class O>struct __sub_test_key__
	{
		bool operator()(unsigned k)
		{
			if(__is_key__<O::value>::value == k)
			{
				if(IsEnabled(O::value))VK_(O::value);
				return false;
			}
			return true;
		}
	};

	template<class O, class P>struct __test_key__
	{
		bool operator()(P &p)
		{
			return __sub_test_key__<O>()(p);
		}
	};
//------------------------------------------
	HANDLE handleTimer = INVALID_HANDLE_VALUE;
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		// HWND h = FindWindow(WindowClass<TestSignalWindow>()(), 0);
		// if(NULL != h)
		// {
		// 	TestSignalWindow *w = (TestSignalWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		// 	SendMessage(w->toolBar.hWnd, TB_ENABLEBUTTON, TL::IntToType<IDB_Ok>::value, MAKELONG(TRUE, 0));
		// }
		// DeleteTimerQueueTimer(NULL, handleTimer, NULL);
	}


	template<class P>struct __test_key__<VL::IntToType<IDB_Ok>, P>
	{
		typedef VL::IntToType<IDB_Ok> O;
		bool operator()(P &id)
		{
			if(__is_key__<O::value>::value == id)
			{
				// HWND h = FindWindow(WindowClass<TestSignalWindow>()(), 0);
				// if(NULL != h)
				// {
				// 	// TestSignalWindow *w = (TestSignalWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
				// 	// if(0 != SendMessage(w->toolBar.hWnd, TB_ISBUTTONENABLED, O::value, 0))
				// 	// {
				// 	// 	TCommand c(h, O::value, 0, w->toolBar.hWnd);
				// 	// 	SendMessage(MESSAGE(c));
				// 	// 	SendMessage(w->toolBar.hWnd, TB_ENABLEBUTTON, O::value, MAKELONG(FALSE, 0));
				// 	// 	CreateTimerQueueTimer(&handleTimer, NULL, WaitOrTimerCallback, NULL, 3000, 0, WT_EXECUTEONLYONCE);
				// 	// }
				// }
				return false;
			}
			return true;
		}
	};


//--------------------------------------------
	void KeyPressed(unsigned key)
	{
		VL::find<button_list, __test_key__>()(key);
	}
}