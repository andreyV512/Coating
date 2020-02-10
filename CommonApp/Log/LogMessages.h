#pragma once
#include "App/AppBase.h"
#include "Automat/Automat.h"

template<class T>struct MessWrap;
template<int N>struct IDtoMess;

static const int green = 0xff00;
static const int blue = 0xff0000;
static const int black = 0x0;
static const int white = 0xffffff;
static const int red = 0xff;
static const int yellow = 0xffff;
static const int sea = 0xffa279;
static const int pink = 0xaBa0FF;

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"

#define MESS_WRAP(name, tpe, txt, bc, tc)template<>struct MessWrap<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return txt __ ## tpe;}\
};\
template<>struct IDtoMess<MessWrap<name>::ID>{typedef MessWrap<name> Result;};

#define MESS(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return txt __ ## tpe;);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

namespace LogMess
{
	MESS_WRAP(On<iCU    	>, void, "Включен сигнал \"Цепи управления\"", red, white);
	MESS_WRAP(On<iKM2_DC >, void, "Включен сигнал \"Пускатель (КМ2 DC)\"", red, white);
	MESS_WRAP(On<iKM3_AC >, void, "Включен сигнал \"Пускатель (КМ3 AC)\"", red, white);
	MESS_WRAP(On<iCycle 	>, void, "Включен сигнал \"ЦИКЛ\"", red, white);
	MESS_WRAP(On<iP1     >, void, "Включен сигнал \"транспортной системы (П1)\"", red, white);
	MESS_WRAP(On<iP2     >, void, "Включен сигнал \"транспортной системы (П2)\"", red, white);
	MESS_WRAP(On<iCOPT   >, void, "Включен сигнал \"СОРТ\"", red, white);
	MESS_WRAP(On<iControl>, void, "Включен сигнал \"КОНТРОЛЬ\"", red, white);
	
	MESS_WRAP(On<oDC_ON1 >, void, "Включен сигнал \"DC ON1\"", red, white);
	MESS_WRAP(On<oAC_ON  >, void, "Включен сигнал \"AC ON\"", red, white);
	MESS_WRAP(On<oDC_ON2 >, void, "Включен сигнал \"DC ON2\"", red, white);
	MESS_WRAP(On<oWork   >, void, "Включен сигнал \"РАБОТА\"", red, white);
	MESS_WRAP(On<oStart  >, void, "Включен сигнал \"ПУСК\"", red, white);
	MESS_WRAP(On<oToShift>, void, "Включен сигнал \"РЕЗУЛЬТАТ\"", red, white);
	MESS_WRAP(On<oC1     >, void, "Включен сигнал \"С1\"", red, white);
	MESS_WRAP(On<oC2  	>, void, "Включен сигнал \"С2\"", red, white);

	MESS_WRAP(Off<iCU    	>, void, "Отключен сигнал \"Цепи управления\"", blue, white);
	MESS_WRAP(Off<iKM2_DC >, void, "Отключен сигнал \"Пускатель (КМ2 DC)\"", blue, white);
	MESS_WRAP(Off<iKM3_AC >, void, "Отключен сигнал \"Пускатель (КМ3 AC)\"", blue, white);
	MESS_WRAP(Off<iCycle 	>, void, "Отключен сигнал \"ЦИКЛ\"", blue, white);
	MESS_WRAP(Off<iP1     >, void, "Отключен сигнал \"транспортной системы (П1)\"", blue, white);
	MESS_WRAP(Off<iP2     >, void, "Отключен сигнал \"транспортной системы (П2)\"", blue, white);
	MESS_WRAP(Off<iCOPT   >, void, "Отключен сигнал \"СОРТ\"", blue, white);
	MESS_WRAP(Off<iControl>, void, "Отключен сигнал \"КОНТРОЛЬ\"", blue, white);
	
	MESS_WRAP(Off<oDC_ON1 >, void, "Отключен сигнал \"DC ON1\"", blue, white);
	MESS_WRAP(Off<oAC_ON  >, void, "Отключен сигнал \"AC ON\"", blue, white);
	MESS_WRAP(Off<oDC_ON2 >, void, "Отключен сигнал \"DC ON2\"", blue, white);
	MESS_WRAP(Off<oWork   >, void, "Отключен сигнал \"РАБОТА\"", blue, white);
	MESS_WRAP(Off<oStart  >, void, "Отключен сигнал \"ПУСК\"", blue, white);
	MESS_WRAP(Off<oToShift>, void, "Отключен сигнал \"РЕЗУЛЬТАТ\"", blue, white);
	MESS_WRAP(Off<oC1     >, void, "Отключен сигнал \"С1\"", blue, white);
	MESS_WRAP(Off<oC2  	>, void, "Отключен сигнал \"С2\"", blue, white);

	MESS_WRAP(TimeOutExteption, void, "Превышено время ожидания", red, yellow);
	MESS_WRAP(AlarmBitsExteption, void, "Выход по аварийному биту", red, yellow);
	MESS_WRAP(ExitLoopExteption, void, "Выход из программы", red, yellow);

	static const int MAX_MESS_ID = __COUNTER__;

	class FactoryMessages
	{
		class Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned& backColor, unsigned& textColor);
		bool Text(int i, char* buf, double val);
		static FactoryMessages& Instance();
	};
}