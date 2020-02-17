#include <iostream>
#include <templates/typelist.hpp>

template<class T>struct NoButton;

template<class List>struct __only_buttons__;
template<class Head, class ...Tail>struct __only_buttons__ < Vlst <Head, Tail... >>
{
	typedef typename VL::Append<Head, typename __only_buttons__<Vlst<Tail...>>::Result>::Result Result;
};
template<class Head, class ...Tail>struct __only_buttons__<Vlst<NoButton<Head>, Tail...>>
{
	typedef typename __only_buttons__<Vlst<Tail...>>::Result Result;
};
template<>struct __only_buttons__<Vlst<>>
{
	typedef Vlst<> Result;
};

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

typedef Vlst<A, B, C, NoButton<D>, E> lst;

int main()
{
	printf("%s\n", typeid(lst).name());
	printf("%s\n", typeid(__only_buttons__<lst>::Result).name());
	getchar();
}

