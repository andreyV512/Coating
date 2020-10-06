#pragma once

#include "templates/typelist.hpp"
#include "App/App.h"
#include "Devices/1730Parameters.h"
#include "tools_debug/DebugMess.h"

template<int N>struct IDtoMess;
template<class>struct On;
template<class>struct Off;

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"
#define _cat(a, b) a b

#define MESS(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

#define MESS1(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

template<class T>struct MessBit;			

#define MESS_BIT(name, tpe, txt, bc, tc)template<>struct MessBit<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<MessBit<name>::ID>{typedef MessBit<name> Result;};

template<class T>struct AlarmBit;
#define ALARM_BIT(name, tpe, txt, bc, tc)template<>struct AlarmBit<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<AlarmBit<name>::ID>{typedef AlarmBit<name> Result;};

static const int green = 0xff00;
static const int blue = 0xff0000;
static const int black = 0x0;
static const int white = 0xffffff;
static const int red = 0xff;
static const int yellow = 0xffff;
static const int sea = 0xffa279;
static const int pink = 0xaBa0FF;
namespace LogMess
{
	///\brief сопоставление числу сообщениея
	///\param тип параметра
	///\param тип дополнительного параметра
	///\param сообщение
	///\param цвет фона
	///\param цвет шрифта
	MESS(StartCycle, void, "Пуск цикла", blue, white)
		//MESS(AirTest		          , void  , "Проверка воздуха"              , blue, white)
	MESS(ProgramOpen, void, "Программа открыта", black, white)
	//							          
	MESS(TimeoutPipe, void, "Превышенно время ожидания", red, yellow)

	MESS(ProgramClosed, void, "Программа закрыта", red, yellow)

	MESS(ExitMeshuringCycle, void, "Оператор вышел из цикла", red, yellow)
	MESS(ExitTubeInZoneSensors, void, "Труба в рабочей зоне установки", red, yellow)

	MESS(DataCollectionDEF, void, "сбор данных", blue, white)

	MESS(Alarm502, int, "Ошибка платы L502  ", red, yellow)

	MESS(DataCollectionCompleted, void, "Цикл сбора данных закончен", green, white)

	MESS(Copt, int, "Сорт", green, white)
	MESS(Brak, void, "Брак", red, yellow)

	MESS(ErrorSpline, void, "Ошибка сплайн", red, yellow)

	MESS(StopBtn, void, "Оператор вышел из цикла", blue, white)
	MESS(On_iIn, void, "Ожидание прутка", blue, white)
	MESS(Collection, void, "Сбор данных", blue, white)
	MESS(CollectionDone, void, "Сбор данных закончен", blue, white)

	MESS_BIT(On<iCU  >, void, "Включен сигнал \"Цепи управления\"", red, white);
	MESS_BIT(On<iIn  >, void, "Включен сигнал \"Труба на входе\"", red, white);
	MESS_BIT(On<iOut >, void, "Включен сигнал \"Труба на выходе\"", red, white);
	MESS_BIT(On<iStrobe >, void, "Включен сигнал \"Строб\"", red, white);

	MESS_BIT(On<oAutomat>, void, "On<oAutomat>", red, white);
	MESS_BIT(On<oSupply >, void, "On<oSupply >", red, white);
	MESS_BIT(On<oMark>, void, "Включен сигнал \"Отметка\"", red, white);

	MESS_BIT(Off<iCU  >, void, "Отключен сигнал \"Цепи управления\"", blue, white);
	MESS_BIT(Off<iIn  >, void, "Отключен сигнал \"Труба на входе\"", blue, white);
	MESS_BIT(Off<iOut >, void, "Отключен сигнал \"Труба на выходе\"", blue, white);
	MESS_BIT(Off<iStrobe >, void, "Отключен сигнал \"Строб\"", blue, white);

	MESS_BIT(Off<oAutomat>, void, "Off<oAutomat>", blue, white);
	MESS_BIT(Off<oSupply >, void, "Off<oSupply >", blue, white);
	MESS_BIT(Off<oMark>, void, "Отключен сигнал \"Отметка\"", blue, white);

	ALARM_BIT(On<iCU  >, void, "Нет сигнала \"Цепи управления\"", red, yellow);
	
	MESS(TimeOutExteption, void, "Превышено время ожидания", red, yellow);
	MESS(AlarmBitsExteption, void, "Выход по аварийному биту", red, yellow);
	MESS(ExitLoopExteption, void, "Выход из программы", red, yellow);

	template<class T>struct Bits;
	template<>struct Bits<Vlst<>> {};
#define BITS_XX(color, txt, ...)template<>struct Bits<Vlst<__VA_ARGS__> >\
	{\
		static const int ID = __COUNTER__;\
		typedef int type;\
		static const unsigned backColor = color;\
		static const unsigned textColor = black;\
		static const char *mess(){return txt;}\
	};\
	template<>struct IDtoMess<Bits<Vlst<__VA_ARGS__> >::ID>\
	{\
		typedef Bits<Vlst<__VA_ARGS__> > Result;\
	};

	BITS_XX(sea, "Ожидание сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", On<iCU>)
	BITS_XX(sea, "Выставлен сигнал \"Автомат\"", On<oAutomat>)
	BITS_XX(sea, "Снят сигнал \"Автомат\"", Off<oAutomat>)

	BITS_XX(sea, "Ожидание cнятия сигнала \"Труба на входе\"", Off<iIn>)
	BITS_XX(sea, "Ожидание cнятия сигнала \"Труба на выходе\"", Off<iOut>)
	BITS_XX(sea, "Ожидание сигнала \"Труба на входе\"", On<iIn>)
	BITS_XX(sea, "Ожидание сигнала \"Труба на выходе\"", On<iOut>)
#undef BITS_XX

//#define ERR_BITS(color, txt, ...)template<>struct Bits<Bits<Vlst<__VA_ARGS__> >>\
//	{\
//		static const int ID = __COUNTER__;\
//		typedef int type;\
//		static const unsigned backColor = color;\
//		static const unsigned textColor = black;\
//		static const int err = Bits<Vlst<__VA_ARGS__> >::ID;\
//		static const char *mess(){return txt;}\
//	};\
//	template<>struct IDtoMess<Bits<Bits<Vlst<__VA_ARGS__>> >::ID>\
//	{\
//		typedef Bits<Bits<Vlst<__VA_ARGS__> >> Result;\
//	};
//
//	//ERR_BITS(red, "Нет сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", On<iCU>)
//#undef ERR_BITS

	MESS(BaseLengthError, void, "Необходимо уменьшить параметр \"Базовое расстояние\"", red, yellow)

	MESS(Recalculation, void, "Перерасчёт", blue, white)
	MESS(RecalculationStop, void, "Расчёт окончен", blue, white)

		


    static const int MAX_MESS_ID = __COUNTER__;

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, char *buf, double val);
		static FactoryMessages &Instance();
	};

	//void Err();
}
#undef MESS
#undef MESS1
#undef MESS2
#undef __double_1 
#undef __double_2 
#undef __int 
#undef __void 
#undef _cat