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

struct A {};
struct B {};
struct C {};
struct D {};

typedef Vlst<A, B, C, D> list;
VL::Factory<list> f;

int main()
{
    VL::find<list, tst>()();
}
