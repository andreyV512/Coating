#pragma once
#include <windows.h>
#include "window_tool/message.h"
#include "templates/typelist.hpp"

namespace WET{
	///\brief в типе Т присутствует член-тип "Parent"
	template<class T>struct IsParent
	{
		template<bool, class Z>struct Ret
		{
			typedef Vlst<> Result;
			static const bool value = false;
		};
		template<class Z>struct Ret<true, Z>
		{
			typedef typename Z::Parent Result;
			static const bool value = true;
		};
		template<class Z>static char Is(Z *, typename Z::Parent * = 0);
		static double Is(...);
		typedef typename Ret<sizeof(char) == sizeof(Is((T *)0)), T>::Result Result;
		static const bool value = Ret<sizeof(char) == sizeof(Is((T *)0)), T>::value;
	};
	template<class T>struct TypeToEvent
	{
		///\brief необходимо для типа Т сопоставить сообщение windows
		typedef typename T::_must_match_the_class_of_message_windows nonexist;
	};

	template<>struct TypeToEvent<TMouseMove>{static const int value = WM_MOUSEMOVE;};///<-------см. ниже комментарий
	template<>struct TypeToEvent<TSize>{static const int value = WM_SIZE;};
	template<>struct TypeToEvent<TPaint>{static const int value = WM_PAINT;};
	template<>struct TypeToEvent<TActivate>{static const int value = WM_ACTIVATE;};
	template<>struct TypeToEvent<TUser>{static const int value = WM_USER;};
	template<>struct TypeToEvent<TDestroy>{static const int value = WM_DESTROY;};
	template<>struct TypeToEvent<TLButtonDown>{static const int value = WM_LBUTTONDOWN;};
	template<>struct TypeToEvent<TRButtonDown>{static const int value = WM_RBUTTONDOWN;};
	template<>struct TypeToEvent<TLButtonUp>{static const int value = WM_LBUTTONUP;};
	template<>struct TypeToEvent<TCommand>{static const int value = WM_COMMAND;};
	template<>struct TypeToEvent<TNotify>{static const int value = WM_NOTIFY;};
	template<>struct TypeToEvent<TGetMinMaxInfo>{static const int value = WM_GETMINMAXINFO;};
	template<>struct TypeToEvent<TLButtonDbClk>{static const int value = WM_LBUTTONDBLCLK;};
	template<>struct TypeToEvent<TKeyDown>{static const int value = WM_KEYDOWN;};
	template<>struct TypeToEvent<TMouseWell>{static const int value =  WM_MOUSEWHEEL;};
	template<>struct TypeToEvent<TMoving>{static const int value =  WM_MOVING;};
	template<>struct TypeToEvent<TSizing>{static const int value =  WM_SIZING;};
	template<>struct TypeToEvent<TMessage>{static const int value =  WM_MESSAGE;};
	template<>struct TypeToEvent<TTimer>{static const int value =  WM_TIMER;};
	template<>struct TypeToEvent<TClose>{static const int value =  WM_CLOSE;};
	template<>struct TypeToEvent<TCopyData>{static const int value =  WM_COPYDATA;};
	template<>struct TypeToEvent<TDropFiles>{static const int value =  WM_DROPFILES;};
	template<>struct TypeToEvent<TSysKeyDown>{static const int value = WM_SYSKEYDOWN;};

	template<class O, class P>class IsFuncExist
	{
		template<class T, T>struct Helper{};
		///\bug Необходимо определить в классе unsigned operator()(XXX &){}, где XXX - TMouseMove, TSize ... TTimer(см выше) 
		///\bug или необходимо определить в классе void operator()(XXX &){}, где XXX - TMouseMove, TSize ... TTimer(см выше) 
		template<class Z>static double Is(O *, Helper<void(O::*)(Z &), &O::operator()> * = NULL);
		template<class Z>static double Is(O *, Helper<LRESULT(O::*)(Z &), &O::operator()> * = NULL);
		template<class Z>static char Is(...);
	public:
		static const bool value = (sizeof(Is<P>((O *)0)) == sizeof(double) );
	};

	template<class O, class P>struct IsCreateExist;
	template<class O, class P, bool>struct isExist
	{
		typedef typename IsCreateExist<typename O::Parent, P>::Result Result;
	};
	template<class O, class P>struct isExist<O, P, false>
	{
		typedef Vlst<> Result;
	};

	template<class O, class P>struct IsCreateExist
	{
		template<class T, T>struct Helper {};
		template<class Z>static double Is(O *, Helper<LRESULT(O:: *)(Z &), &O::operator()> * = NULL);
		template<class Z>static char Is(...);
		typedef typename VL::_if<sizeof(Is<P>((O *)0)) == sizeof(double)
			, O
			, typename isExist<O, P, IsParent<O>::value>::Result
		>::Result Result;
	};

	template<class O, class P>struct Couple
	{
		typedef O OBJ;
		typedef P PAR;
	};

	template<class O, class P>struct RES
	{
		typedef Couple<O, P> Result;
	};
	
	template<class O>struct RES<O, NullType>
	{
		typedef NullType Result;
	};
	
	typedef Vlst <
		TMouseMove, TSize, TPaint, TActivate, TNotify
		, TLButtonDown, TLButtonUp, TLButtonDbClk, TKeyDown 
		, TUser, TCommand, TDestroy
		, TGetMinMaxInfo, TMouseWell, TRButtonDown, TMoving, TSizing
		, TMessage, TTimer, TClose
		, TCopyData, TDropFiles	, TSysKeyDown
	> type_events_all_list; //<-------см. ниже комментарий

	template<class List, class T, class tmp = Vlst<> >struct AddTypeEvent;
	template<class T, class tmp, class Head, class ...Tail>struct AddTypeEvent<Vlst<Head, Tail...>, T, tmp>
	{
		typedef typename VL::_if<IsFuncExist<T, Head>::value, Couple<T, Head>, Vlst<> >::Result Item;
		typedef typename AddTypeEvent<Vlst<Tail...>, T, typename VL::Append<tmp, Item>::Result>::Result Result;
	};
	template<class T, class tmp>struct AddTypeEvent<Vlst<>, T, tmp>
	{
		typedef tmp Result;
	};

	template<class T>struct AddSubClass
	{
		typedef typename VL::Append<
			typename AddTypeEvent<type_events_all_list, T, Vlst<> >::Result
			, typename AddSubClass<typename IsParent<T>::Result>::Result
		>::Result Result;
	};
	template<>struct AddSubClass<Vlst<> >
	{
		typedef Vlst<> Result;
	};

	template<bool >struct Wrap
	{
		template<class O, class P>LRESULT operator()(O &o, P &p)
		{
			p.obj(o);
			return 0;
		}
	};

	template<>struct Wrap<false>
	{
		template<class O, class P>LRESULT operator()(O &o, P &p)
		{
			return p.obj(o);
		}
	};

	template<class A, class B, class T>struct SubReTypeTest
	{
		typedef Couple<A, B> Result;
	};
	template<class A, class B>struct SubReTypeTest<A, B, B>
	{
		typedef Vlst<> Result;
	};

	template<class List, class T>struct SubReTypeDelete;
	template<class A, class B, class T, class ...Tail>struct SubReTypeDelete<Vlst<Couple<A, B>, Tail...>, T>
	{
		typedef typename VL::Append<
			typename SubReTypeTest<A, B, T>::Result
			, typename SubReTypeDelete<Vlst<Tail...>, T>::Result
		>::Result Result;
	};
	template<class T>struct SubReTypeDelete<Vlst<>, T>
	{
		typedef Vlst<> Result;
	};

	template<class List>struct ReTypeDelete;
	template<class Head, class ...Tail>struct ReTypeDelete<Vlst<Head, Tail...> >
	{
		typedef typename SubReTypeDelete<Vlst<Tail...>, typename Head::PAR>::Result tail;
		typedef typename VL::Append<Head, typename ReTypeDelete<tail>::Result>::Result Result;
	};
	template<>struct ReTypeDelete<Vlst<>>
	{
		typedef Vlst<> Result;
	};

	template<class T>struct EventHandler
	{
		template<class O, class P>struct loc
		{
			template<class T, T>struct Helper{};
			template<class X, class Z>static double Is(X *, Helper<void(X::*)(Z &), &X::operator()> * = NULL);
			template<class X, class Z>static char Is(...);
			bool operator()(P &p)
			{
				if(TypeToEvent<typename O::PAR>::value != p.mess.uMsg) return true;
				static const bool b = sizeof(double) == sizeof(Is<typename O::OBJ, typename O::PAR>((typename O::OBJ *)0));
				p.result = Wrap<b>()((typename O::PAR &)p.mess, (EventHandler<typename O::OBJ> &)p);
				return false;
			}
		};
		template<class List, class T>struct TestNotNullType
		{
			typedef List Result;
		};
		template<class T>struct TestNotNullType<Vlst<>, T>
		{
			///\brief класс должен иметь обработчик в виде- LRESULT T::operator()(XXX &) или void T::operator()(XXX &);
			///где: XXX - TMouseMove, TSize, TPaint, TActivate(добавить при необходимости в список type_events_all_list, см. выше)
			typedef typename T::__class_does_not_have_any_handler__ nonexist;
		};
		
		template<typename List, template<class, class>class proc>struct XWapper
		{
			template<class P>bool operator()(P &p) 
			{ 
				return !VL::find<List, loc>()(p); 
			}
		};
		template<template<class, class>class proc>struct XWapper<Vlst<>, proc>
		{
			template<class P>bool operator()(P& p) { return false; }
		};
		TMessage &mess;
		T &obj;
		LRESULT result;
	public:
		typedef
			typename TestNotNullType<
			typename ReTypeDelete<
			typename AddSubClass<T>::Result
			>::Result
			, T>::Result list;
		EventHandler(TMessage &mess, T &obj)
			: mess(mess)
			, obj(obj)
			, result(0)
		{}
		LRESULT operator()()
		{			
			if (XWapper<list, loc>()(*this))return result;
			return DefWindowProc(MESSAGE(mess));
		}
	};
}									
//------------------------------------------------------------------------------------------------
template<class T>struct Viewer
{
	template<class T>struct Create
	{
		template<class O, class P>LRESULT operator()(O *o, P &p) 
		{ 
			return (*(T *)o)((TCreate &)p); 
		}
	};
	template<>struct Create<Vlst<> >
	{
		template<class O, class P>LRESULT operator()(O *o, P &p) 
		{ 
			return 0; 
		}
	};
	static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{		
		T *o = (T *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		TMessage m = {hWnd, message, wParam, lParam};
		if(NULL != o)
		{
			return  WET::EventHandler<T>(m, *o)();
		}	
		else if(WM_CREATE == message)
		{
			o = (T *)(*(TCreate *)&m).create->lpCreateParams;
			o->hWnd = hWnd;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)o);
			return Create<typename WET::IsCreateExist<T, TCreate>::Result>()(o, m);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
};
//--------------------------------------------------------------------------------------------------------------