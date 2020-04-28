#pragma once
#include <Windows.h>
#include "templates/typelist.hpp"
#include "PerformanceCounter/PerformanceCounter.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "Log/LogBuffer.h"
#include "Log/LogMessages.h"
#include "Automat.h"


namespace Automat
{
	//struct Result
	//{
	//	Status status;
	//};

	template<class List>struct WaitFor
	{
		static const int  count = VL::Length<List>::value;
		HANDLE h[count];
		template<class O, class P>struct __init__
		{
			void operator()(P& p)
			{
				p[VL::IndexOf<List, O>::value] = O::hEvent;
			}
		};
		unsigned operator()()
		{
			VL::foreach<List, __init__>()(h);
			return WaitForMultipleObjects(count, h, FALSE, 5);
		}
	};
	template<>struct WaitFor<Vlst<>>
	{
		static HANDLE hEvent;
		unsigned operator()()
		{
			return WaitForSingleObject(hEvent, 5);
		}
	};

	template<class O, class P>struct __bits__
	{
		void operator()(O& o, P& p)
		{
			p |= o.value;
		}
	};

	template<class O, class P>struct __proc__
	{
		void operator()(P& p)
		{
			Proc<O>()(p);
		}
	};

	template<class T>struct __key_data__
	{
		typedef T list;
		Status status;
		int numKey;
		__key_data__(int numKey) : status(Status::contine), numKey(numKey) {}
	};

	template<class O, class P>struct __key__
	{
		bool operator()(P& p)
		{
			if (VL::IndexOf<P::list, O>::value == p.numKey)
			{
				p.status = O::value;
				return false;
			}
			return true;
		}
	};

	template<class P>struct __key__<StopBtn, P>
	{
		bool operator()(P &p)
		{
			if (VL::IndexOf<P::list, StopBtn>::value == p.numKey)
			{
				throw StopBtn();
			}
			return true;
		}
	};


	template<class O, class P>struct __test_bits_xxx__
	{
		void operator()(O& o, P& p)
		{
			if (0 != (o.value & p.changed))
			{
				if (0 != (o.value & p.bits))
				{
					 Log::Mess<MessBit<On<O> > >();
				}
				else
				{
					Log::Mess<MessBit<Off<O>>>();
				}
			}
		}
	};

	template<class T>struct __test_bits__
	{
		unsigned bits, changed;
		void operator()(unsigned bits)
		{
			static unsigned last = 0;
			changed = last ^ bits;
			if (changed)
			{
				VL::foreach<typename T::items_list, __test_bits_xxx__>()(Singleton<T>::Instance().items, *this);
				last = bits;
			}
		}
	};
//------------------------
	template<class List>struct __filtr_tst_bits__;
	template<class Head, class ...Tail>struct __filtr_tst_bits__<Vlst<Head, Tail...>>
	{
		typedef typename __filtr_tst_bits__<Vlst<Tail...>>::Result Result;
	};
	template<class Head, class ...Tail>struct __filtr_tst_bits__<Vlst<TstOn<Head>, Tail...>>
	{
		typedef typename VL::Append< TstOn<Head>, typename __filtr_tst_bits__<Vlst<Tail...>>::Result>::Result Result;
	};
	template<class Head, class ...Tail>struct __filtr_tst_bits__<Vlst<TstOff<Head>, Tail...>>
	{
		typedef typename VL::Append< TstOff<Head>, typename __filtr_tst_bits__<Vlst<Tail...>>::Result>::Result Result;
	};
	template<>struct __filtr_tst_bits__<Vlst<>>
	{
		typedef Vlst<> Result;
	};

	struct __alarm_bits_data__
	{
		InputBitsTable::TItems &items;
		unsigned inputBits;
		__alarm_bits_data__(unsigned inputBits)
			: items(Singleton<InputBitsTable>::Instance().items)
			, inputBits(inputBits)
		{}
	};
	template<class O, class P>struct __alarm_bits_xxx__;
	template<class X, class P>struct __alarm_bits_xxx__<TstOn<X>, P>
	{
		void operator()(P &p)
		{
			if (0 == (p.inputBits & p.items.get<X>().value))
			{
				Log::Mess<AlarmBit<On<X> > >();
				/*INFO
				* Необходимо переопределить макрос 
				* ALARM_BIT(On<X>, void, "подписать название сигнала", red, yellow);
				* X - тип сигнала (см. файл LogMessages.h)
				*/
			}
		}
	};
	template<class X, class P>struct __alarm_bits_xxx__<TstOff<X>, P>
	{
		void operator()(P &p)
		{
			if (0 != (p.inputBits & p.items.get<X>().value))
			{
				Log::Mess<AlarmBit<Off<X> > >();
				/*INFO
				* Необходимо переопределить макрос
				* ALARM_BIT(Off<X>, void, "подписать название сигнала", red, yellow);
				* X - тип сигнала (см. файл LogMessages.h)
				*/
			}
		}
	};

	template<class List>struct __alarm_bits_message__
	{
		void operator()(unsigned bits)
		{
			__alarm_bits_data__ data(bits);
			VL::foreach<List, __alarm_bits_xxx__>()(data);
		}
	};
	template<>struct __alarm_bits_message__<Vlst<>>
	{
		void operator()(unsigned bits) {}
	};
//----------------------------------
	template<class List, template<class, class>class Proc>struct WrapFor
	{
		template<class Items, class Data>void operator()(Items &items, Data &data)
		{
			VL::foreach<List, Proc>()(items, data);
		}
		template<class Data>void operator()(Data& data)
		{
			VL::foreach<List, Proc>()(data);
		}
	};
	template<template<class, class>class Proc>struct WrapFor<Vlst<>, Proc>
	{
		template<class Items, class Data>void operator()(Items& items, Data& data) {}
		template<class Data>void operator()(Data& data)	{}
	};

	template<class List, template<class, class>class Proc>struct WrapFind
	{
		template<class Items, class Data>bool operator()(Items &items, Data &data)
		{
			return VL::find<List, Proc>()(items, data);
		}
		template<class Data>bool operator()(Data& data)
		{
			return VL::find<List, Proc>()(data);
		}
	};
	template<template<class, class>class Proc>struct WrapFind<Vlst<>, Proc>
	{
		template<class Items, class Data>bool operator()(Items& items, Data& data) { return true; }
		template<class Data>bool operator()(Data& data) { return true; }
	};

	template<typename... Args> Status Bits(unsigned delay = -1)
	{
		if (-1 != delay) delay += Performance::Counter();

		InputBitsTable::TItems(&items) = Singleton<InputBitsTable>::Instance().items;
		unsigned bitsOn = 0;
		WrapFor<typename VL::WrapFilter<On, Vlst<Args...> >::Result, __bits__>()(items, bitsOn);
		unsigned bitsOff = 0;
		WrapFor<typename VL::WrapFilter<Off, Vlst<Args...> >::Result, __bits__>()(items, bitsOff);
		//
		unsigned tstBitsOn = 0;
		typedef typename VL::WrapFilter<TstOn, Vlst<Args...> >::Result __tst_on_list;
		WrapFor<__tst_on_list, __bits__>()(items, tstBitsOn);
		unsigned tstBitsOff = 0;
		typedef typename VL::WrapFilter<TstOff, Vlst<Args...> >::Result __tst_off_list__;
		WrapFor<__tst_off_list__, __bits__>()(items, tstBitsOff);
		//
		typedef typename VL::WrapFilter<Proc, Vlst<Args...> >::Result proc_list;
		//
		int alarmTick = 5;
		
		while (true)
		{
			typedef typename VL::WrapFilter<Key, Vlst<Args...> >::Result key_list;
			DWORD ev = WaitFor<typename VL::TypeToTypeLst<key_list, Key>::Result>()();
			switch (ev)
			{
			case WAIT_TIMEOUT:
			{
				if (delay < Performance::Counter())	throw TimeOutExteption();
		
				unsigned inputs  = device1730.Read();
				__test_bits__<InputBitsTable>()(inputs);
				unsigned outputs = device1730.ReadOutput();
				__test_bits__<OutputBitsTable>()(outputs);
		
				unsigned bitsOr = bitsOn | bitsOff;
				if (bitsOr && bitsOn == (inputs & bitsOr)) return Status::contine;
		
				Status result = Status::contine;
				WrapFor<proc_list, __proc__>()(result);
				if (Status::exit_from_procedure == result) return Status::contine;
		
				unsigned tstBitsOr = tstBitsOn | tstBitsOff;
				if (tstBitsOr)
				{
					if (tstBitsOn != (inputs & tstBitsOr))
					{
						if (--alarmTick < 0)
						{
							__alarm_bits_message__<typename __filtr_tst_bits__<Vlst<Args...>>::Result>()(inputs);
							throw AlarmBitsExteption();
						}
					}
					else
					{
						alarmTick = 5;
					}
				}
				break;
			}
			case WAIT_FAILED:
				throw ExitLoopExteption();
			default:
			{
				__key_data__<key_list> data(ev - WAIT_OBJECT_0);
				if (!WrapFind<key_list, __key__>()(data)) return data.status;
			}
			}
		}
		return Status::contine;
	}
}