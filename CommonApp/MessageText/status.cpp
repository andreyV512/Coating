#include "status.h"
#include "status.hpp"
#include <typeinfo>
#include <stdio.h>
#include "DlgTemplates/ParamDlg.hpp"

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

	PARAM_TITLE(Vlst<Norm>, L"Норма")
	PARAM_TITLE(Vlst<NoBottomReflection>, L"Нет донного отражения")
	PARAM_TITLE(Vlst<Defect>, L"Брак")
	PARAM_TITLE(Vlst<DeadZone>, L"Неизмеряемая зона")
	//PARAM_TITLE(Vlst<SensorOff>, L"Датчик отключен")
	PARAM_TITLE_LIST(Vlst, L"Брак, нет донного отражения", NoBottomReflection, Defect)

	wchar_t *mess[VL::Length<status_list>::value];
	unsigned color[VL::Length<status_list>::value];

	template<class O, class P>struct init_mess
	{
		void operator()(P &p) 
		{
			p[VL::IndexOf<status_list, O>::value] = ParamTitle<O>()();
		}
	};

	template<class O, class P>struct init_color;
	template<template<class ...>class W, class ...X, class P>struct init_color<W<X...>, P>
	{
		void operator()(P &p)
		{
			p[VL::IndexOf<status_list, W<X...>>::value] = Singleton<ColorTable>::Instance().items.get<Clr<X...>>().value;
		}
	};

	struct InitStatus
	{
		InitStatus()
		{
			VL::foreach<status_list, outer>()(st);
			VL::foreach<status_list, init_mess>()(mess);			
		}
	} init;
}

namespace StatusData
{
	unsigned char Compute(unsigned char s0, unsigned char s1)
	{
		static const int x = VL::Length<ZoneStatus::status_list>::value;
		if(s0 < x && s1 < x) return ZoneStatus::st[s0][s1];
		return VL::IndexOf<zone_status_list, DeadZone>::value;
	}

	void Text(unsigned char id, unsigned &color, const wchar_t *&txt)
	{
		if (id < VL::Length<ZoneStatus::status_list>::value)
		{
			color = ZoneStatus::color[id];
			txt = ZoneStatus::mess[id];
			return;
		}
		color = 0xffeeeeee;
		txt = L"";
	}

	unsigned Color(unsigned char id)
	{
		if (id < VL::Length<ZoneStatus::status_list>::value)
		{
			return ZoneStatus::color[id];
		}
		return 0xffeeeeee;
	}

	void UpdateColor()
	{
		VL::foreach<ZoneStatus::status_list, ZoneStatus::init_color>()(ZoneStatus::color);
	}
}



