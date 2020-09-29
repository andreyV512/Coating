#include <stdio.h>
#include <typeinfo>
#include <string>
#include "templates/impl.hpp"

struct A
{
	virtual void print() { printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"); }
	virtual ~A() { printf("%s\n", __FUNCTION__); };
};


struct B: A
{
	char buf[128];
	B(char *b) 
	{ strcpy_s(buf, b); }
	void print()override { printf("%s\n", buf); }
	~B()override { printf("%s\n", __FUNCTION__); }
};

struct C : B
{
	int k;
	C(char *b, int &k): B(b),k(k) {}
	void print()override { printf("%s  %d\n", buf, k); }
	~C()override { printf("%s\n", __FUNCTION__); }
};



int main()
{
	int k = 0;
	{
		Impl<A, 272> impl;
		A &a = impl.Init<C>((char *)"Ok obi", k);
		a.print();
		impl.Init<B>((char *)"HI Ok obi");
		a.print();
		printf("   %d\n", (int)sizeof(B));
		impl->print();
		(*impl).print();


	}
}
