#include "status.h"
#include "status.hpp"

namespace ZoneStatus
{
	static const int size = VL::Length<status_list>::value;
	char st[size][size];
	template<int N>struct data
	{
		static const int id = N;
		char(&data)[size][size];
		data(char(&data)[size][size]) : data(data) {}
	};
	template<class O, class P>struct inner
	{
		void operator()(P &p)
		{
			typedef attach<typename VL::TypeAt<P::id, status_list>::Result, O>::Result att;
			static const int i = VL::IndexOf<status_list, O>::value;
			p.data[P::id][i] = VL::IndexOf<status_list, att>::value;
		}
	};
	template<class O, class P>struct outer
	{
		void operator()(P &p)
		{
			static const int id = VL::IndexOf<status_list, O>::value;
			data< id > d(p);
			VL::foreach<status_list, inner>()(d);
		}
	};
}

Status::Status()
{
	VL::foreach<ZoneStatus::status_list, ZoneStatus::outer>()(ZoneStatus::st);
}
unsigned char Status::operator()(unsigned char s0, unsigned char s1)
{
	return ZoneStatus::st[s0][s1];
}

void Status::Text(unsigned char id, unsigned &color, wchar_t *&txt)
{

}



