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
	template<typename Head>struct Factory;

	template<typename Head>struct Factory<Vlst<Head>> : Head
	{
		Factory() : Head() {}
		template<typename T>Factory(T& t) : Head(t) {}
		template<typename T>Factory(T* t) : Head(t) {}
		template<typename T>T& get() { return *this; }
	};

	template<typename Head, typename ...Tail>struct Factory<Vlst<Head, Tail...>> : Head, Factory<Vlst<Tail...>>
	{
		Factory() : Head(), Factory<Vlst<Tail...>>() {}
		template<typename T>Factory(T& t) : Head(), Factory<Vlst<Tail...>>(t) {}
		template<typename T>Factory(T* t) : Head(), Factory<Vlst<Tail...>>(t) {}
		template<typename T>T& get() { return *this; }
	};

	template<typename List, template<typename, typename>typename proc>struct for_each;

	template<template<typename, typename>typename proc, typename Head, typename ...Tail>struct for_each<Vlst<Head, Tail...>, proc>
	{
		template<class O, class P>void operator()(O& o, P& p)
		{
			(proc<Head, P>()(o, p), for_each< Vlst<Tail...>, proc>()(o, p));
		}
		template<class P>void operator()(P& p)
		{
			(proc<Head, P>()(p), for_each< Vlst<Tail...>, proc>()(p));
		}
		void operator()(Vlst<Head, Tail...>& o)
		{
			(proc<Head, int>()(o), for_each< Vlst<Tail...>, proc>()(o));
		}
		void operator()()
		{
			(proc<Head, int>()(), for_each< Vlst<Tail...>, proc>()());
		}
	};

	template<template<typename, typename>typename proc, typename Head>struct for_each<Vlst<Head>, proc>
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
			return (proc<Head, int>()(o, p) && find<Vlst<Tail...>, proc>()(o, p));
		}
		template<class P>bool operator()(P &p)
		{
			return (proc<Head, P>()(p) && find<Vlst<Tail...>, proc>()(p));
		}
		bool operator()(Vlst<Head, Tail...>& o)
		{
			return (proc<Head, int>()(o) && find<Vlst<Tail...>, proc>()(o));
		}
		template<class O>bool operator()()
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
		bool operator()(Vlst<Head>& o)
		{
			return proc<Head, int>()(o);
		}
		template<class O>bool operator()()
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
