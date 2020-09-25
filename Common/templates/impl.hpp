#pragma once
template<class T, int size>struct impl
{
	template<int size, bool b> struct the_buffer_size_must_be_larger_than_the_type_size;
	template<int size> struct the_buffer_size_must_be_larger_than_the_type_size<size, true> {};
	union {
		T *o;
		char buf[size];
	}inner;
	template<class Z, class ...Params>Z &Set(Params... p)
	{
		the_buffer_size_must_be_larger_than_the_type_size<sizeof(Z), sizeof(Z) <= size>();
		return *new(inner.buf)Z(p...);
	}
};

/*
struct A
{
	virtual void print() = 0;
};


struct B: A
{
	char buf[128];
	B(char *b)
	{ strcpy_s(buf, b); }
	void print()override { printf("%s\n", buf); }
};

struct C : B
{
	int k;
	C(char *b, int &k): B(b),k(k) {}
	void print()override { printf("%s  %d\n", buf, k); }
};



int main()
{
	int k = 1234;
	A &a = impl<A, 272>().Set<C>((char *)"Ok obi", k);
	a.print();
}
*/
