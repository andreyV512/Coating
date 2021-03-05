#pragma once
template<class T, int size>struct Impl
{
	template<int size, bool b> struct the_buffer_size_must_be_larger_than_the_type_size;
	template<int size> struct the_buffer_size_must_be_larger_than_the_type_size<size, true> {};
	char buf[size];
	Impl() { *(unsigned *)buf = 0; }
	template<class Z, class ...Params>Z &Init(Params... p)
	{
		the_buffer_size_must_be_larger_than_the_type_size<sizeof(Z), sizeof(Z) <= size>();
		if (0 != *(unsigned *)buf)((T *)buf)->~T();
		return *new(buf)Z(p...);
	}
	~Impl() { 
		if (0 != *(unsigned *)buf)((T *)buf)->~T();
	}
	T &operator *()const { return *(T *)buf; }
	T *operator ->()const { return (T *)buf; }
	operator T *()const { return (T *)buf;}
	template<class Z>Z &get()const { return *(Z *)buf; }
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
	int k = 0;
	{
		Impl<A, 272> impl;
		A &a = impl.Init<C>((char *)"Ok obi", k);
		a.print();
		impl.Init<B>((char *)"HI Ok obi");
		a.print();
		printf("   %d\n", (int)sizeof(B));
	}
}
*/
