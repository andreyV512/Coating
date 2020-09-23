#include <stdio.h>
#include <typeinfo>
#include "templates/typelist.hpp"
#include "status.h"

template<class O, class P>struct __print__
{
	void operator()(P &p)
	{
		printf("%d %s\n\n", p++, typeid(O).name());
	}
};

//#define DEFINE_PARAM_WRAP_LIST(Wrap, type, def_val, ...) template<> struct Wrap<__VA_ARGS__>\
//{\
//	typedef type type_value;\
//	type_value value;\
//	const type_value default_value;\
//	const wchar_t *name(){return L#Wrap###z;}\
//	Wrap() : value(def_val), default_value(def_val) {}\
//};


#define STICK__(n) L ## n ## "X"
#define STICK_(n) STICK__ (n)
#define STICK(n) #n##"_"
#define STICK_TOGETHER(...)  FOR_EACH(STICK, __VA_ARGS__)
//const wchar_t *x = STICK_(STICK_TOGETHER(a,b,c,d));

//#define zzz(WRAP, ...)const wchar_t *WRAP() {return L#WRAP ## (STICK_TOGETHER(__VA_ARGS__));}
#define zzz(WRAP, ...)const wchar_t *WRAP() {return L#WRAP STICK_TOGETHER(__VA_ARGS__);}

zzz(tst, a, b, c)

int main()
{
	//int num = 0;
	//VL::foreach<ZoneStatus::status_list, __print__>()(num);

	

	wprintf(L"%s \n", tst());

	Status st;
	int t = st(1, 2);
	printf("result %d\n", t);


}
