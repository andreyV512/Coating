#include "AScanKeyHandler.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "templates/typelist.hpp"
#include "AScanWindow.h"
#include "Resource.h"
#include "tools_debug/DebugMess.h"
#include "window_tool/Emptywindow.h"

namespace AScanKeyHandler
{
	typedef Vlst<
		VL::IntToType<IDB_CycleBtn>
		, VL::IntToType<IDB_Reset>
		//, VL::IntToType<IDB_QueryBtn>
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
			HWND h = FindWindow(WindowClass<AScanWindow>()(), 0);
			AScanWindow(&w) = *(AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			SendMessage(w.toolBar.hWnd, TB_ENABLEBUTTON, Head::ID, MAKELONG(Head::state, 0));
			__btn__<Tail...>()();
		}
	};
	
	template<>struct __btn__<Vlst<>>{void operator()(){}};

	//bool IsEnabled(int id)
	//{
	//	HWND h = FindWindow(WindowClass<AScanWindow>()(), 0);
	//	AScanWindow (&w) = *(AScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	//	return 0 != SendMessage(w.toolBar.hWnd, TB_ISBUTTONENABLED, id, 0);
	//}

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

	void Run()
	{		
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , Vlst<VL::IntToType<IDB_Reset> >
			  , On, Off
			>::Result
		>()();
	}

	void Stop()
	{
		__btn__<
			__all_button_OnOff__<
			button_list
			, Vlst<VL::IntToType<IDB_Reset> >
			, Off, On
			>::Result
		>()();
	}
}