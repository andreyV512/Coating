#include "LogMessages.h"
#include <Windows.h>
#include "templates/typelist.hpp"
#include "templates/templates.hpp"
#include <stdio.h>
#include "PerformanceCounter\PerformanceCounter.h"
#include "LogBuffer.h"

#pragma warning(disable : 4996)

namespace LogMess
{
	struct Loc
	{
		unsigned backColor, textColor;
		void(*proc)(wchar_t *, double);
	};
	template<class T, class C>struct GenFunc;
	template<class C>struct GenFunc<void, C>
	{
		static void Func(wchar_t *buf, double)
		{
			wsprintf(buf, C::mess());
		}
	};
	
	template<class C>struct GenFunc<int, C>
	{
		static void Func(wchar_t *buf, double val)
		{
			wsprintf(buf, C::mess(), (int)val);			
		}
	};
	template<class C>struct GenFunc<double, C>
	{
		static void Func(wchar_t *buf, double val)
		{
			Wchar_from x;
			wsprintf(buf, C::mess(), x(val));			
		}
	};
	template<class C>struct GenFunc<bool, C>
	{
		static void Func(wchar_t *buf, double val)
		{
			wsprintf(buf, C::mess(), 0.0 != val ? L"ВКЛ" : L"ОТКЛ");			
		}
	};
	template<class C>struct GenFunc<const wchar_t *, C>
	{
		static void Func(wchar_t *buf, double val)
		{
			wsprintf(buf, C::mess(), (wchar_t *)*(unsigned *)&val);			
		}
	};
	
	template<int i = 0, bool = true>struct Item
	{
		typedef typename IDtoMess<i>::Result O;
		void operator()(Loc(&arr)[MAX_MESS_ID])
		{
			arr[i].backColor = O::backColor;
			arr[i].textColor = O::textColor;
			arr[i].proc = GenFunc<O::type, O >::Func;
			Item < i + 1, i + 1 < MAX_MESS_ID > ()(arr);
		}
	};
	template<int i>struct Item<i, false>
	{
		void operator()(Loc(&arr)[MAX_MESS_ID]) {}
	};
	struct FactoryMessages::Inner
	{
	public:
		unsigned start = 0;
		
		Loc arr[MAX_MESS_ID];
		Inner()
		{
			Item<>()(arr);
		}
		static Inner &Instance(){static Inner x; return x;}
	};
	FactoryMessages::FactoryMessages()
		: inner(Inner::Instance())
	{
	}
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
	bool FactoryMessages::Text(int i, wchar_t *buf, double val)
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

////////////////////////////////////////////////////////////////////////////////////////
//	template<class T>struct __exist_err__
//	{
//	private:
//		template<class E, E>struct Helper;
//		template<class Z>static char Tst(...);
//		template<class Z>static double Tst(Z *, Helper<int, Z::err> * = NULL);
//		static const bool value = sizeof(double) == sizeof(Tst<T>((T *)NULL));
//	public:
//		typedef typename VL::_if<value, T, Vlst<>>::Result Result;
//	};
//	
//	template<int N, class tmp = Vlst<>>struct __get_err_list__
//	{
//		typedef typename IDtoMess<N>::Result Head;
//		typedef typename __get_err_list__<N - 1, typename VL::Append<tmp, typename __exist_err__<Head>::Result>::Result>::Result Result;
//	};
//
//	template<class tmp>struct __get_err_list__<-1, tmp>
//	{
//		typedef tmp Result;
//	};
//
//	typedef __get_err_list__<MAX_MESS_ID - 1>::Result __error_list__;
//
//	template<class O, class P>struct __get_err__
//	{
//		bool operator()()
//		{
//			if(Log::ID == O::err){Log::Mess<O>(); return false;}
//			return true;
//		}
//	};
//////////////////////////////////////////////////////////////////////////////////////////
//
//	void Err()
//	{
//		VL::find<__error_list__, __get_err__>()();
//	}
}
