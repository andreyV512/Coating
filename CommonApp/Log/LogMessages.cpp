#include "LogMessages.h"
#include <Windows.h>
#include "templates/typelist.hpp"
#include <stdio.h>
#include "PerformanceCounter/PerformanceCounter.h"

#pragma warning(disable : 4996)

namespace LogMess
{
	struct Loc
	{
		unsigned backColor, textColor;
		void(*proc)(char*, double);
	};
	template<class T, class C>struct GenFunc;
	template<class C>struct GenFunc<void, C>
	{
		static void Func(char* buf, double)
		{
			sprintf(buf, C::mess());
		}
	};

	template<class C>struct GenFunc<int, C>
	{
		static void Func(char* buf, double val)
		{
			sprintf(buf, C::mess(), (int)val);
		}
	};
	template<class C>struct GenFunc<double, C>
	{
		static void Func(char* buf, double val)
		{
			sprintf(buf, C::mess(), val);
		}
	};
	template<class C>struct GenFunc<bool, C>
	{
		static void Func(char* buf, double val)
		{
			sprintf(buf, C::mess(), 0.0 != val ? "¬ À" : "Œ“ À");
		}
	};
	template<class C>struct GenFunc<const char*, C>
	{
		static void Func(char* buf, double val)
		{
			sprintf(buf, C::mess(), (char*)*(unsigned*)&val);
		}
	};

	class Items
	{
		Loc (&arr)[MAX_MESS_ID];
		template<class T>void Proc()
		{
			static const int i = T::value;
			typedef typename IDtoMess<i>::Result O;
			arr[i].backColor = O::backColor;
			arr[i].textColor = O::textColor;
			arr[i].proc = GenFunc<O::type, O >::Func;
			Proc<VL::IntToType<1 + i>>();
		}
		template<>void Proc<VL::IntToType<MAX_MESS_ID - 1>>()
		{
			static const int i = MAX_MESS_ID - 1;
			typedef typename IDtoMess<MAX_MESS_ID - 1>::Result O;
			arr[i].backColor = O::backColor;
			arr[i].textColor = O::textColor;
			arr[i].proc = GenFunc<O::type, O >::Func;
		}
	public:
		Items(Loc(&arr)[MAX_MESS_ID]): arr(arr){}
	};
	class FactoryMessages::Inner
	{
		Items items;
	public:
		unsigned start;
		Loc arr[MAX_MESS_ID];
		Inner() : items(arr) {}
		static Inner &Instance(){static Inner x; return x;}
	};
	FactoryMessages::FactoryMessages() : inner(Inner::Instance()) {}
	void FactoryMessages::StartTime()
	{
		inner.start = Performance::Counter();
	}
	bool FactoryMessages::Color(int i, unsigned &backColor, unsigned &textColor)
	{
		if(i < MAX_MESS_ID) 
		{
			backColor = inner.arr[i].backColor;
			textColor = inner.arr[i].textColor;
			return true;
		}
		return false;
	}
	bool FactoryMessages::Text(int i, char *buf, double val)
	{
		if(i < MAX_MESS_ID) 
		{
			inner.arr[i].proc(buf, val);
			return true;
		}
		return false;
	}
	FactoryMessages &FactoryMessages::Instance()
	{
		static FactoryMessages x; return x;
	}
}
