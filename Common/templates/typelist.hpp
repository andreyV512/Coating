#pragma once

//template<int N, typename T>constexpr int dimention_of(T(&)[N])
//{
//	return N;
//}

template<class T, int N>char(&__length_array_of_T_type_same_as_length_array_of_char__(T(&)[N]))[N];
#define dimention_of(x) sizeof(__length_array_of_T_type_same_as_length_array_of_char__(x))

class NullType {};
template<typename ...List>struct Vlst {};

namespace VL
{
	template<class>struct ToVlst;
	template<template<typename ...>class W, typename ...List>struct ToVlst<W<List...>>
	{
		typedef Vlst<List...> Result;
	};

	template<typename List>struct Factory;

	template<typename Head>struct Factory<Vlst<Head>> : Head
	{
		Factory() : Head() {}
		template<typename T>Factory(T &t) : Head(t) {}
		template<typename T>Factory(T *t) : Head(t) {}
		template<typename T>T &get() { return *this; }
	};

	template<typename Head, typename ...Tail>struct Factory<Vlst<Head, Tail...>> : Head, Factory<Vlst<Tail...>>
	{
		Factory() : Head(), Factory<Vlst<Tail...>>() {}
		template<typename T>Factory(T &t) : Head(t), Factory<Vlst<Tail...>>(t) {}
		template<typename T>Factory(T *t) : Head(t), Factory<Vlst<Tail...>>(t) {}
		template<typename T>T &get() { return *this; }
	};

	template<typename List, template<typename, typename>typename proc>struct foreach;

	template<template<typename, typename>typename proc, typename Head, typename ...Tail>struct foreach<Vlst<Head, Tail...>, proc>
	{
		template<class O, class P>void operator()(O& o, P& p)
		{
			(proc<Head, P>()(o, p), foreach< Vlst<Tail...>, proc>()(o, p));
		}
		template<class P>void operator()(P& p)
		{
			(proc<Head, P>()(p), foreach< Vlst<Tail...>, proc>()(p));
		}
		void operator()(Vlst<Head, Tail...>& o)
		{
			(proc<Head, int>()(o), foreach< Vlst<Tail...>, proc>()(o));
		}
		void operator()()
		{
			(proc<Head, int>()(), foreach< Vlst<Tail...>, proc>()());
		}
	};

	template<template<typename, typename>typename proc, typename Head>struct foreach<Vlst<Head>, proc>
	{
		template<class O, class P>void operator()(O& o, P& p)
		{
			proc<Head, P>()(o, p);
		}
		template<class P>void operator()(P& p)
		{
			proc<Head, P>()(p);
		}
		void operator()(Vlst<Head>& o)
		{
			proc<Head, int>()(o);
		}
		void operator()()
		{
			proc<Head, int>()();
		}
	};

	template< typename List, template<typename, typename>typename proc>struct find;

	template<template<typename, typename>typename proc, typename Head, typename ...Tail>struct find<Vlst<Head, Tail...>, proc>
	{
		template<class O, class P>bool operator()(O& o, P& p)
		{
			return (proc<Head, P>()(o, p) && find<Vlst<Tail...>, proc>()(o, p));
		}
		template<class P>bool operator()(P &p)
		{
			return (proc<Head, P>()(p) && find<Vlst<Tail...>, proc>()(p));
		}
		bool operator()(Vlst<Head, Tail...>& o)
		{
			return (proc<Head, int>()(o) && find<Vlst<Tail...>, proc>()(o));
		}
		bool operator()()
		{
			return (proc<Head, int>()() && find<Vlst<Tail...>, proc>()());
		}
	};

	template<template<typename, typename>typename proc, typename Head>struct find<Vlst<Head>, proc>
	{
		template<class O, class P>bool operator()(O& o, P& p)
		{
			return proc<Head, P>()(o, p);
		}
		template<class P>bool operator()(P& p)
		{
			return proc<Head, P>()(p);
		}
		bool operator()(Vlst<Head> &o)
		{
			return proc<Head, int>()(o);
		}
		bool operator()()
		{
			return proc<Head, int>()();
		}
	};

	template<typename List>struct Length;

	template<typename ...T>struct Length<Vlst<T...>>
	{
		static const int value = sizeof...(T);
	};

	template<typename List, typename T>struct IndexOf;

	template<typename T, typename Head, typename ...Tail>struct IndexOf<Vlst<Head, Tail...>, T>
	{
		static const int value = 1 + IndexOf<Vlst<Tail...>, T>::value;
	};

	template<typename Head, typename ...Tail>struct IndexOf<Vlst<Head, Tail...>, Head>
	{
		static const int value = 0;
	};

	template<typename T>struct IndexOf<Vlst<>, T>
	{
		static const int value = 0;
	};

	template<int N, typename List>struct TypeAt;

	template<int N, typename Head, typename ...Tail>struct TypeAt<N, Vlst<Head, Tail...>>
	{
		typedef typename TypeAt<N - 1, Vlst<Tail...>>::Result Result;
	};

	template<typename Head, typename ...Tail>struct TypeAt<0, Vlst<Head, Tail...>>
	{
		typedef Head Result;
	};

	template<class T, class L>struct Append
	{
		typedef Vlst<T, L> Result;
	};
	template<class T, class ...L>struct Append<T, Vlst<L...>>
	{
		typedef Vlst<T, L...> Result;
	};
	template<class ...T, class L>struct Append<Vlst<T...>, L>
	{
		typedef Vlst<T..., L> Result;
	};
	template<class ...T, class ...L>struct Append<Vlst<T...>, Vlst<L...>>
	{
		typedef Vlst<T..., L...> Result;
	};

	template<bool, typename A, typename B>struct _if
	{
		typedef A Result;
	};
	template<typename A, typename B>struct _if<false, A, B>
	{
		typedef B Result;
	};

	template<typename List, template<class>class T>struct EraseAllParam;
	template<typename Head, typename ...Tail, template<class>class T>struct EraseAllParam<Vlst<Head, Tail...>, T>
	{
		typedef typename Append<
			typename _if<T<Head>::value
				, Head
				, Vlst<>
			>::Result
			, typename EraseAllParam<Vlst<Tail...>, T>::Result
		>::Result Result;
	};
	template<template<class>class T>struct EraseAllParam<Vlst<>, T> 
	{
		typedef Vlst<> Result;
	};

	template<int N>struct IntToType { static const int value = N; };

	template<class A, class B>struct CompareTypes { typedef A Result; };
	template<class A>struct CompareTypes<A, A> { typedef Vlst<> Result; };

	template<class List, class T>struct EraseItem;
	template<class T, class Head, class ...Tail>struct EraseItem<Vlst<Head, Tail...>, T>
	{
		typedef typename Append<
			typename CompareTypes<Head, T>::Result
			, typename EraseItem<Vlst<Tail...>, T>::Result
		>::Result Result;
	};
	template<class T>struct EraseItem<Vlst<>, T>
	{
		typedef Vlst<> Result;
	};

	template<class T>struct Inner;
	template<template<class>class W, class T>struct Inner<W<T> >
	{
		typedef T Result;
	};
	template<template<class, class>class W, class T, class X>struct Inner<W<T, X> >
	{
		typedef T Result;
	};

	template<class T, template<class>class W>struct CmpW { typedef Vlst<> Result; };
	template<class T, template<class>class W>struct CmpW<W<T>, W> { typedef W<T> Result; };
	template<class List, template<class>class Wrap>struct SelectWrap;
	template<template<class>class Wrap, class Head, class ...Tail>struct SelectWrap<Vlst<Head, Tail...>, Wrap>
	{
		typedef typename Append<typename CmpW<Head, Wrap>::Result, typename SelectWrap<Vlst<Tail...>, Wrap>::Result>::Result Result;
	};
	template<template<class>class Wrap>struct SelectWrap<Vlst<>, Wrap>
	{
		typedef Vlst<> Result;
	};

	template<class List>struct SubMultyList;
	template<class Head, class ...Tail>struct SubMultyList<Vlst<Head, Tail...> >
	{
		typedef typename Append<Head, typename SubMultyList<Vlst<Tail...> >::Result>::Result Result;
	};
	template<>struct SubMultyList<Vlst<> >
	{
		typedef Vlst<> Result;
	};
	template<class ...List>struct MultyList
	{
		typedef typename SubMultyList<Vlst<List...> >::Result Result;
	};

	template<class List, class T>struct InList;
	template<class Head, class ...Tail, class T>struct InList<Vlst<Head, Tail...>, T>
	{
		static const bool value = InList<Vlst<Tail...>, T>::value;
	};
	template<class T, class ...Tail>struct InList<Vlst<T, Tail...>, T>
	{
		static const bool value = true;
	};
	template<class T>struct InList<Vlst<>, T>
	{
		static const bool value = false;
	};
	template<class List, class T>struct SubListFromMultyList;
	template<class Head, class ...Tail, class T>struct SubListFromMultyList<Vlst<Head, Tail...>, T>
	{
		typedef typename VL::_if<
			InList<Head, T>::value
			, Head
			, typename SubListFromMultyList<Vlst<Tail...>, T>::Result
		>::Result Result;
	};

	template<class T>struct SubListFromMultyList<Vlst<>, T>
	{
		typedef Vlst<> Result;
	};

	template<typename List, template<typename>class Wrap>struct TypeToTypeLst;
	template<typename Head, typename ...Tail, template<typename>class Wrap>struct TypeToTypeLst<Vlst<Head, Tail...>, Wrap>
	{
		typedef typename Append<Wrap<Head>, typename TypeToTypeLst<Vlst<Tail...>, Wrap>::Result>::Result Result;
	};
	template<template<typename>class Wrap>struct TypeToTypeLst<Vlst<>, Wrap>
	{
		typedef Vlst<> Result;
	};

	template<typename List, template<typename, typename>class Wrap, class Param>struct TypeToTypeLstParam1;
	template<typename Head, typename ...Tail, template<typename, typename>class Wrap, class Param>struct TypeToTypeLstParam1<Vlst<Head, Tail...>, Wrap, Param>
	{
		typedef typename Append<Wrap<Head, Param>, typename TypeToTypeLstParam1<Vlst<Tail...>, Wrap, Param>::Result>::Result Result;
	};
	template<template<typename, typename>class Wrap, class Param>struct TypeToTypeLstParam1<Vlst<>, Wrap, Param>
	{
		typedef Vlst<> Result;
	};

	template<class T>struct TestType
	{
		template<class Q, Q>struct helper {};
		template<class Z>static double Is(
			typename Z *
			, helper<typename Inner<Z>::Result::type_value(Z:: *)(), &Z::operator()> * = NULL
		);
		template<class Z>static char   Is(...);
		static const bool value = sizeof(double) == sizeof(Is<T>((T *)0));
	};

	template<class List>struct TypeExist;
	template<class Head, class ...Tail>struct TypeExist<Vlst<Head, Tail...> >
	{
		typedef typename _if<
			TestType<Head>::value
			, Head
			, typename TypeExist<Vlst<Tail...> >::Result
		>::Result Result;
	};
	template<>struct TypeExist<Vlst<> >
	{
		typedef Vlst<> Result;
	};

	template<class O, class P>struct __copy__
	{
		void operator()(O &o, P &p)
		{
			p.get<O>().value = o.value;
		}
	};

	template<class From, class To>void CopyFromTo(From &from, To &to)
	{
		foreach<typename Inner<From>::Result, __copy__>()(from, to);
	}

	template<template<class>class W, class List, class tmp = Vlst<> >struct WrapFilter;
	template<template<class>class W, class tmp, class Head, class ...Tail>struct WrapFilter<W, Vlst<Head, Tail...>, tmp>
	{
		typedef typename WrapFilter<W, Vlst<Tail...>, tmp>::Result Result;
	};
	template<template<class>class W, class tmp, class Head, class ...Tail>struct WrapFilter<W, Vlst<W<Head>, Tail...>, tmp>
	{
		typedef typename WrapFilter<W, Vlst<Tail...>, typename Append<tmp, Head>::Result>::Result Result;
	};
	template<template<class>class W, class tmp>struct WrapFilter<W, Vlst<>, tmp>
	{
		typedef tmp Result;
	};

	template<template<int>class Wrap, int start, int max>struct CreateNumList
	{
		typedef typename Append<Wrap<start>, typename CreateNumList<Wrap, 1 + start, max>::Result>::Result Result;
	};
	template<template<int>class Wrap, int max>struct CreateNumList<Wrap, max, max>
	{
		typedef Vlst<Wrap<max> > Result;
	};

	template<class List, class T>struct TypeInList;
	template<class T, class Head, class ...Tail>struct TypeInList<Vlst<Head, Tail...>, T>
	{
		static const bool value = TypeInList<Tail..., T>::value;
	};
	template<class Head, class ...Tail>struct TypeInList<Vlst<Head, Tail...>, Head>
	{
		static const bool value = true;
	};
	template<class T>struct TypeInList<Vlst<>, T>
	{
		static const bool value = false;
	};
}

template<class T>struct Singleton
{
	static T& Instance() { return x; }
private:
	static inline  T x;
	Singleton() = delete;
	~Singleton() = delete;
	// необходимо также запретить копирование
	Singleton(Singleton const&) = delete; // реализаци¤ не нужна
	Singleton& operator= (Singleton const&) = delete;  // и тут
};

#define EXPAND(x) x
#define CONCATENATE(x,y) x##y

#define FOR_EACH_1(what, x, ...)what(x)
#define FOR_EACH_2(what, x, ...)what(x)EXPAND(FOR_EACH_1(what, __VA_ARGS__))
#define FOR_EACH_3(what, x, ...)what(x)EXPAND(FOR_EACH_2(what, __VA_ARGS__))
#define FOR_EACH_4(what, x, ...)what(x)EXPAND(FOR_EACH_3(what, __VA_ARGS__))
#define FOR_EACH_5(what, x, ...)what(x)EXPAND(FOR_EACH_4(what, __VA_ARGS__))
#define FOR_EACH_6(what, x, ...)what(x)EXPAND(FOR_EACH_5(what, __VA_ARGS__))
#define FOR_EACH_7(what, x, ...)what(x)EXPAND(FOR_EACH_6(what, __VA_ARGS__))
#define FOR_EACH_8(what, x, ...)what(x)EXPAND(FOR_EACH_7(what, __VA_ARGS__))
#define FOR_EACH_9(what, x, ...)what(x)EXPAND(FOR_EACH_8(what, __VA_ARGS__))
#define FOR_EACH_10(what, x, ...)what(x)EXPAND(FOR_EACH_9(what, __VA_ARGS__))
#define FOR_EACH_11(what, x, ...)what(x)EXPAND(FOR_EACH_10(what, __VA_ARGS__))
#define FOR_EACH_12(what, x, ...)what(x)EXPAND(FOR_EACH_11(what, __VA_ARGS__))
#define FOR_EACH_13(what, x, ...)what(x)EXPAND(FOR_EACH_12(what, __VA_ARGS__))
#define FOR_EACH_14(what, x, ...)what(x)EXPAND(FOR_EACH_13(what, __VA_ARGS__))
#define FOR_EACH_15(what, x, ...)what(x)EXPAND(FOR_EACH_14(what, __VA_ARGS__))
#define FOR_EACH_16(what, x, ...)what(x)EXPAND(FOR_EACH_15(what, __VA_ARGS__))
#define FOR_EACH_17(what, x, ...)what(x)EXPAND(FOR_EACH_16(what, __VA_ARGS__))
#define FOR_EACH_18(what, x, ...)what(x)EXPAND(FOR_EACH_17(what, __VA_ARGS__))
#define FOR_EACH_19(what, x, ...)what(x)EXPAND(FOR_EACH_18(what, __VA_ARGS__))
#define FOR_EACH_20(what, x, ...)what(x)EXPAND(FOR_EACH_19(what, __VA_ARGS__))
#define FOR_EACH_21(what, x, ...)what(x)EXPAND(FOR_EACH_20(what, __VA_ARGS__))
#define FOR_EACH_22(what, x, ...)what(x)EXPAND(FOR_EACH_21(what, __VA_ARGS__))
#define FOR_EACH_23(what, x, ...)what(x)EXPAND(FOR_EACH_22(what, __VA_ARGS__))
#define FOR_EACH_24(what, x, ...)what(x)EXPAND(FOR_EACH_23(what, __VA_ARGS__))
#define FOR_EACH_25(what, x, ...)what(x)EXPAND(FOR_EACH_24(what, __VA_ARGS__))
#define FOR_EACH_26(what, x, ...)what(x)EXPAND(FOR_EACH_25(what, __VA_ARGS__))
#define FOR_EACH_27(what, x, ...)what(x)EXPAND(FOR_EACH_26(what, __VA_ARGS__))
#define FOR_EACH_28(what, x, ...)what(x)EXPAND(FOR_EACH_27(what, __VA_ARGS__))
#define FOR_EACH_29(what, x, ...)what(x)EXPAND(FOR_EACH_28(what, __VA_ARGS__))
#define FOR_EACH_30(what, x, ...)what(x)EXPAND(FOR_EACH_29(what, __VA_ARGS__))
#define FOR_EACH_31(what, x, ...)what(x)EXPAND(FOR_EACH_30(what, __VA_ARGS__))
#define FOR_EACH_32(what, x, ...)what(x)EXPAND(FOR_EACH_31(what, __VA_ARGS__))
#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) EXPAND(FOR_EACH_ARG_N(__VA_ARGS__))
#define FOR_EACH_ARG_N(_32,_31,_30,_29,_28,_27,_26,_25,_24,_23,_22,_21,_20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1, N, ...) N
#define FOR_EACH_RSEQ_N() 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1

#define FOR_EACH_(N, what, ...) EXPAND(CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__))
#define FOR_EACH(what, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)

//---------------------------------------------------тест генератора
//#define INT_TO_TYPE(n) TL::IntToType<n>,
//#define INT_TO_TYPE_LIST(...)  TL::MkTlst<FOR_EACH(INT_TO_TYPE, __VA_ARGS__)NullType>::Result
//typedef INT_TO_TYPE_LIST(1,3, 7, 8, 9) int_to_type_list ;
