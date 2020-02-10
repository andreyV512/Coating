#pragma once
#include <Windows.h>
#include "templates/typelist.hpp"
#include "PerformanceCounter/PerformanceCounter.h"
#include "App/AppBase.h"
#include "App/App.h"
#include "Log/LogBuffer.h"
#include "Log/LogMessages.h"


namespace Automat
{
	struct Result
	{
	};

	template<class List>struct WaitFor
	{
		static const int  count = VL::Length<List>::value;
		HANDLE h[count];
		template<class O, class P>struct __init__
		{
			void operator()(P& p)
			{
				p[VL::IndexOf<List, O>::value] = O::Event;
			}
		};
		unsigned operator()()
		{
			VL::for_each<List, __init__>()(h);
			return WaitForMultipleObjects(count, h, FALSE, 5);
		}
	};
	template<>struct WaitFor<Vlst<>>
	{
		static HANDLE Event;
		unsigned operator()()
		{
			return WaitForSingleObject(Event, 5);
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
			Proc<O>(p);
		}
	};

	template<class T>struct __key_data__
	{
		typedef T list;
		DWORD value;
	};

	template<class O, class P>struct __key__
	{
		bool operator()(P& p)
		{
			if (VL::IndexOf<P::list, O>::value == p.value)
			{
				p.value = O::value;
				return false;
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
					 Log::Mess<MessWrap<On<O> > >();
				}
				else
				{
					Log::Mess<MessWrap<Off<O>>>();
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
				VL::for_each<typename T::items_list, __test_bits_xxx__>()(Singleton<T>::Instance().items, *this);
				last = bits;
			}
		}
	};

	template<class List, template<class, class>class Proc>struct WrapFor
	{
		template<class Items, class Data>void operator()(Items &items, Data &data)
		{
			VL::for_each<List, Proc>()(items, data);
		}
		template<class Data>void operator()(Data& data)
		{
			VL::for_each<List, Proc>()(data);
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
			return VL::for_each<List, Proc>()(data);
		}
	};
	template<template<class, class>class Proc>struct WrapFind<Vlst<>, Proc>
	{
		template<class Items, class Data>bool operator()(Items& items, Data& data) { return true; }
		template<class Data>bool operator()(Data& data) { return true; }
	};

	template<typename ...Args>unsigned Bits(unsigned delay = -1)
	{
		if (-1 != delay) delay += Performance::Counter();

		Result result;

		InputBitsTable::TItems(&items) = Singleton<InputBitsTable>::Instance().items;
		unsigned bitsOn = 0;
		WrapFor<typename VL::WrapFilter<On, Vlst<Args...> >::Result, __bits__>()(items, bitsOn);
		unsigned bitsOff = 0;
		WrapFor<typename VL::WrapFilter<Off, Vlst<Args...> >::Result, __bits__>()(items, bitsOff);
		//
		unsigned tstBitsOn = 0;
		WrapFor<typename VL::WrapFilter<TstOn, Vlst<Args...> >::Result, __bits__>()(items, tstBitsOn);
		unsigned tstBitsOff = 0;
		WrapFor<typename VL::WrapFilter<TstOff, Vlst<Args...> >::Result, __bits__>()(items, tstBitsOff);
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
				if (bitsOr && bitsOn == (inputs & bitsOr)) return 0;
		
				WrapFor<proc_list, __proc__>()(result);
		
				unsigned tstBitsOr = tstBitsOn | tstBitsOff;
				if (tstBitsOr)
				{
					if (tstBitsOn == (inputs & tstBitsOr))
					{
						if (--alarmTick < 0) throw AlarmBitsExteption();
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
				__key_data__<key_list> data = { ev - WAIT_OBJECT_0 };
				if (!WrapFind<key_list, __key__>()(data)) return data.value;
			}
		}

		return 0;
	}
}