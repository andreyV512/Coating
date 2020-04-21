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
	MESS_WRAP(On<iCU >, void, "Включен сигнал \"Цепи управления\"", red, white);
	MESS_WRAP(On<iIn >, void, "Включен сигнал \"Труба на входе\"", red, white);
	MESS_WRAP(On<iOut>, void, "Включен сигнал \"Труба на выходе\"", red, white);
		
	MESS_WRAP(On<oAutomat >, void, "Включен сигнал \"Автомат\"", red, white);
	MESS_WRAP(On<oSupply  >, void, "Включен сигнал \"Подвод\"", red, white);
	MESS_WRAP(On<oMark >, void   , "Включен сигнал \"Отметка\"", red, white);

	MESS_WRAP(Off<iCU >, void, "Включен сигнал \"Цепи управления\"", blue, white);
	MESS_WRAP(Off<iIn >, void, "Включен сигнал \"Труба на входе\"" , blue, white);
	MESS_WRAP(Off<iOut>, void, "Включен сигнал \"Труба на выходе\"", blue, white);

	MESS_WRAP(Off<oAutomat >, void, "Включен сигнал \"Автомат\""   , blue, white);
	MESS_WRAP(Off<oSupply  >, void, "Включен сигнал \"Подвод\""    , blue, white);
	MESS_WRAP(Off<oMark >, void, "Включен сигнал \"Отметка\""      , blue, white);

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