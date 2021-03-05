#pragma once
template<class T, int size>struct Impl
{
	template<int size, bool b> struct the_buffer_size_must_be_larger_than_the_type_size;
	template<int size> struct the_buffer_size_must_be_larger_than_the_type_size<size, true> {};
	char buf[size] = {};
	template<class Z, class ...Params>Z &Init(Params... p)
	{
		the_buffer_size_must_be_larger_than_the_type_size<sizeof(Z), sizeof(Z) <= size>();
		((T *)buf)->~T();
		return *new(buf)Z(p...);
	}
	~Impl() { 
		((T *)buf)->~T();
	}
	operator T *()const { return (T *)buf;}
};
