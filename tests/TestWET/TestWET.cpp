// TestWET.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <templates/typelist.hpp>
#include "window_tool/WindowsEventTemplate.hpp"
#include "window_tool/message.h"

class TestWnd
{
public:
	
	
	void operator()(TClose&) {}
	void operator()(TMouseWell&) {}
	void operator()(TUser&) {}

	void operator()(TGetMinMaxInfo&) {}
	void operator()(TSize&) { }
	
};

class TestW : public TestWnd
{
public:
	typedef TestWnd Parent;
	void operator()(TGetMinMaxInfo&) {}
	void operator()(TSize&) { }
	LRESULT operator()(TCreate&) {}
	void operator()(TDestroy&) {}

	

};


//typedef WET::AddTypeEvent<WET::type_events_all_list, TestW>::Result list;
//typedef WET::ReTypeDelete<list0>::Result list;

//typedef typename list0::xxx ccc;




struct A { void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct B { void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct C { void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct D { void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };

typedef Vlst<A, B, C, D> listA;

struct _1 { void Do(int& i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _2 { void Do(int& i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _3 { void Do(int& i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _4 { void Do(int& i) { std::cout << __FUNCTION__ << ++i << std::endl; } };

typedef Vlst<_1, _2, _3, _4> listN;

template<class O, class P>struct proc
{
	bool  operator()(P &p)
	{
		O().Do(p);
		std::cout << typeid(O).name() << ++p << std::endl;
		return true;
	}
};

#define TST(t, l){\
   typedef VL::Append<t, l>::Result alist;\
	std::cout << typeid(alist).name() << std::endl;\
}

template<class T>struct no_IDB_SEP
{
	static const bool value = true;
};
template<class W>struct no_IDB_SEP<WET::Couple<W, TGetMinMaxInfo>>
{
	static const bool value = false;
};

int main()
{
	int p = 0;
	VL::find<listA, proc>()(p);
	//typedef XAppend<Vlst<>, A>::Result alist;
	//std::cout << VL::IndexOf<list, C>::value << std::endl;
	//std::cout << typeid(alist).name() << std::endl;
    //std::cout << "Hello World!\n";

	TST(A, _1)
		TST(A, listN)
		TST(listA, _1)
		TST(listA, listN)


		TST(A, Vlst<>)
		TST(listA, Vlst<>)
		TST(Vlst<>, _1)
		TST(Vlst<>, listN)

		typedef WET::AddTypeEvent<WET::type_events_all_list, TestW>::Result handlers;

	typedef WET::AddSubClass<TestW>::Result listsAll;

	
	std::cout << typeid(handlers).name() << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << typeid(listsAll).name() << std::endl;


	typedef WET::ReTypeDelete<listsAll>::Result listDel;

		std::cout << "-------------------------------------" << std::endl;
	std::cout << typeid(listDel).name() << std::endl;


	std::cout << "----------oooooo---------------------------" << std::endl;
	typedef typename VL::EraseAllParam<listsAll, no_IDB_SEP>::Result only_buttons_list;
	std::cout << typeid(only_buttons_list).name() << std::endl;
}

