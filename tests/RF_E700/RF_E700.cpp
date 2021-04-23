﻿// RF_E700.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "App/AppBase.h"
#include "tools_debug/DebugMess.h"
#include "templates/templates.hpp"
#include "Units/FR_E700/FR_E700.h"

template<int >struct ParityX;
#define PARITY(n)template<>struct ParityX<n>\
{\
	static const int value = n;\
	wchar_t *Name(){return (wchar_t *)L#n;}\
};

PARITY(EVENPARITY)
PARITY(MARKPARITY)
PARITY(NOPARITY)
PARITY(ODDPARITY)
PARITY(SPACEPARITY)

typedef Vlst<
    ParityX<EVENPARITY 	 >
    , ParityX<MARKPARITY >
    , ParityX<NOPARITY	 >
    , ParityX<ODDPARITY  >
    , ParityX<SPACEPARITY>
>baud_parity_list;

template<int >struct StopBitsX;
#define STOPBITS(n)template<>struct StopBitsX<n>\
{\
	static const int value = n;\
	wchar_t *Name(){return (wchar_t *)L#n;}\
};

STOPBITS(ONESTOPBIT)
STOPBITS(ONE5STOPBITS)
STOPBITS(TWOSTOPBITS)

typedef Vlst<
    StopBitsX<ONESTOPBIT	>
    , StopBitsX<ONE5STOPBITS	>
    , StopBitsX<TWOSTOPBITS 	>
>baud_stop_bits_list;

template<class T>struct com_params_data { typedef T list; int num; wchar_t* res; };
template<class O, class P>struct com_params
{
    bool operator()(P &p)
    {
        if (VL::IndexOf<P::list, O>::value == p.num)
        {
            p.res = (wchar_t *)O().Name();
            return false;
        }
        return true;
    }
};

template<class O, class P>struct PrintParam
{
    void operator()(O &o)
    {
        Wchar_from<O::type_value>t(o.value);
        dprint("%s %S\n", typeid(O).name(), t());
    }
};
template<class P>struct PrintParam<Parity, P>
{
    typedef Parity O;
    void operator()(O& o)
    {
        com_params_data<baud_parity_list> data{o.value};
        VL::find<baud_parity_list, com_params>()(data);
        dprint("%s %S\n", typeid(O).name(), data.res);
    }
};
template<class P>struct PrintParam<StopBits, P>
{
    typedef StopBits O;
    void operator()(O& o)
    {
        com_params_data<baud_stop_bits_list> data{ o.value };
        VL::find<baud_stop_bits_list, com_params>()(data);
        dprint("%s %S\n", typeid(O).name(), data.res);
    }
};

int main()
{
    auto& comParam = Singleton<ComPortTable>::Instance().items;
    //comParam.get<ComPortAddr>().value = 9;
    dprint("-------------------------\n");
    VL::foreach<ComPortTable::items_list, PrintParam>()(comParam);
    
    bool com_port_open = FR_E700::Init();
    const char* mess_com_port = com_port_open ? "Open" : "Closed";
    dprint("%s\n", mess_com_port);

    FR_E700::Reset r;
    r.Init();

    FR_E700::SetFrequency sf;
    sf.Init();

    FR_E700::SetState ss;
    ss.Init(FR_E700::WriteState::STF | FR_E700::WriteState::RH);

    FR_E700::GetState gs;
    gs.Init();

    Sleep(20000);

    ss.Init(FR_E700::WriteState::none);

    getchar();
}
