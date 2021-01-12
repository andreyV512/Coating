#include <stdio.h>
#include <typeinfo>
#include <string>
#include "templates/impl.hpp"
#include "templates/typelist.hpp"


struct A { };
struct B { typedef Vlst<> list; };

template<class T>struct IsExistList
{
	template<class Z>static double tst(Z *, typename Z::list * = NULL);
	template<class Z>static char tst(...);
	static const bool  value = sizeof(double) == sizeof(tst<T>(NULL));
};

int main()
{
	printf("A %d\n", IsExistList<A>::value);
	printf("B %d\n", IsExistList<B>::value);
	return 0;
}
