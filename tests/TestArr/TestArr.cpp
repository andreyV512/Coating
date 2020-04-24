// TestArr.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "templates\typelist.hpp"

template<class O, class P>struct tst
{
    bool operator()()
    {
        printf("%s\n", typeid(O).name());
        return true;
    }
};

template<class >struct TstOn {};
template<class >struct TstOff {};

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

struct A {};
struct B {};
struct C {};
struct D {};
struct F {};
struct Z {};

typedef Vlst<TstOn<Z>, A, TstOn<B>, C, TstOff<D>, F> list;
VL::Factory<list> f;

int main()
{
    typedef __filtr_tst_bits__<list>::Result xxx;
    VL::find<xxx, tst>()();
}
