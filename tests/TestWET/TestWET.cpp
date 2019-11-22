// TestWET.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <templates/typelist.hpp>
#include "window_tool/WindowsEventTemplate.hpp"
#include "window_tool/message.h"
#include "C:\Users\USER\source\repos\Coating\Plast\App\AppBase.h"
#include "C:\Users\USER\source\repos\Coating\Common\DlgTemplates\ParamDlg.hpp"

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

class TestWnd
{
public:
	HWND hWnd;
	
	void operator()(TClose&) {}
	void operator()(TMouseWell&) {}
	void operator()(TUser&) {}

	void operator()(TGetMinMaxInfo&) {}
	void operator()(TSize&) { }
	LRESULT operator()(TCreate &) {return 50;}
	
};

class TestW : public TestWnd
{
public:
	typedef TestWnd Parent;
	void operator()(TGetMinMaxInfo&) {}
	void operator()(TSize&) { }
	
	void operator()(TDestroy&) {}

	//LRESULT operator()(TCreate &) { return 100; }

};


//typedef WET::AddTypeEvent<WET::type_events_all_list, TestW>::Result list;
//typedef WET::ReTypeDelete<list0>::Result list;

//typedef typename list0::xxx ccc;




struct A { int val; A(int & val):val(val){} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct B { int val; B(int & val):val(val){} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct C { int val; C(int & val):val(val){} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct D { int val; D(int & val):val(val){} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };

typedef Vlst<A, B, C, D> listA;

struct _1 { int val; _1(int &val) :val(val) {} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _2 { int val; _2(int &val) :val(val) {} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _3 { int val; _3(int &val) :val(val) {} void Do(int &i) { std::cout << __FUNCTION__ << ++i << std::endl; } };
struct _4 { int val; _4(int &val) :val(val) {} void Do(int& i) { std::cout << __FUNCTION__ << ++i << std::endl; } };

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

template<class O, class P>struct Print
{
	void operator()(O &o, P &p)
	{
		o.Do(p);
	}
};


template<int N>struct expX
{
	static const int value = 10 * expX<N - 1>::value;
};
template<>struct expX<0>
{
	static const int value = 1;
};

template<int N>wchar_t *dtow(double v, wchar_t *buf)
{
	wchar_t *s = buf;
	_itow((int)v, s, 10);
	s = &buf[wcslen(s)];
	*s = '.';
	++s;
	_itow(int((v - (int)v) * expX<N>::value), s, 10);
	return buf;
}

struct TestVal
{
	typedef  int type_value;
	type_value value;
};

MIN_VALUE(TestVal, 0)
MAX_VALUE(TestVal, 9999999)

int main()
{
#if 0
	int p = 0;
	VL::find<listA, proc>()(p);
	

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
	std::cout << "----------EraseItem---------------------------" << std::endl;
	typename VL::EraseItem<listA, A>::Result lA;
	std::cout << typeid(lA).name() << " A " << std::endl;

	typename VL::EraseItem<listA, B>::Result lB;
	std::cout << typeid(lB).name() << " B " << std::endl;

	typename VL::EraseItem<listA, C>::Result lC;
	std::cout << typeid(lC).name() << " C " << std::endl;

	typename VL::EraseItem<listA, D>::Result lD;
	std::cout << typeid(lD).name() << " D " << std::endl;
	//---test Factory=-----------------------------
	VL::Factory<listA> obj;
	int iii = 0;

	VL::for_each<listA, Print>()(obj, iii);
	wchar_t buf[32];
	std::cout << (wchar_t *)dtow<3>(123.456, buf) << std::endl;
	wprintf(L"%s\n", dtow<3>(123.456, buf));
#else

	int val = 88;
	HWND h = 0;
	allowable_limit_on_the_parameter<TestVal>()(val, h);

	typedef VL::MultyList<
		listA
		, listN
	>::Result type_list;

	int xxxx;
	VL::Factory<type_list> items(xxxx);

	std::cout << typeid(items).name() << "FActory" << std::endl;

	std::wcout << typeid(SubListFromMultyList<Vlst<listA, listN>, TestW>::Result).name() << std::endl;

	std::cout << typeid(ParametersBase::type_list).name() << std::endl;
	//std::cout << typeid(WET::IsCreateExist<TestWnd, TCreate>::Result).name() << std::endl;
  //TestWnd
	//Create<WET::IsFuncExist<TestWnd, TCreate>::value>()(o, m);
	//Create<WET::IsFuncExist<TestW, TCreate>::value>()(o, m)
	auto x = Viewer<TestW>::Proc;// (HWND(), WM_CREATE, 0, 0);
	std::cout << x << std::endl;
#endif
}

