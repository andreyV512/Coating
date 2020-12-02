#pragma once

#include "templates/typelist.hpp"
#include "App/App.h"
#include "Devices/1730Parameters.h"
#include "tools_debug/DebugMess.h"

template<int N>struct IDtoMess;
template<class>struct On;
template<class>struct Off;

#define __double_1 L" %s" 
#define __double_2 L" %s"
#define __int L" %d"
#define __void L""
#define __bool L" %s"
#define __const_char_ptr L" %s"
#define _cat(a, b) a b

#define MESS(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
	static const unsigned textColor = tc;\
	static const wchar_t *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

#define MESS1(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
	static const unsigned textColor = tc;\
	static const wchar_t *mess(){return _cat(txt, __##tpe##_1);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

template<class T>struct MessBit;			

#define MESS_BIT(name, tpe, txt, bc, tc)template<>struct MessBit<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
	static const unsigned textColor = tc;\
	static const wchar_t *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<MessBit<name>::ID>{typedef MessBit<name> Result;};

template<class T>struct AlarmBit;
#define ALARM_BIT(name, tpe, txt, bc, tc)template<>struct AlarmBit<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
	static const unsigned textColor = tc;\
	static const wchar_t *mess(){return _cat(txt, __##tpe);}\
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
	MESS(StartCycle, void, L"Пуск цикла", blue, white)
		//MESS(AirTest		          , void  , "Проверка воздуха"              , blue, white)
	MESS(ProgramOpen, void, L"Программа открыта", black, white)
	//							          
	MESS(TimeoutPipe, void, L"Превышенно время ожидания", red, yellow)

	MESS(ProgramClosed, void, L"Программа закрыта", red, yellow)

	MESS(ExitMeshuringCycle, void, L"Оператор вышел из цикла", red, yellow)
	MESS(ExitTubeInZoneSensors, void, L"Труба в рабочей зоне установки", red, yellow)

	MESS(DataCollectionDEF, void, L"сбор данных", blue, white)

	MESS(Alarm502, int, L"Ошибка платы L502  ", red, yellow)

	MESS(DataCollectionCompleted, void, L"Цикл сбора данных закончен", green, white)

	MESS(Copt, int, L"Сорт", green, white)
	MESS(Brak, void, L"Брак", red, yellow)

	MESS(ErrorSpline, void, L"Ошибка сплайн", red, yellow)

	MESS(StopBtn, void, L"Оператор вышел из цикла", blue, white)
	MESS(On_iIn, void, L"Ожидание прутка", blue, white)
	MESS(Collection, void, L"Сбор данных", blue, white)
	MESS(CollectionDone, void, L"Сбор данных закончен", blue, white)

	MESS_BIT(On<iCU  >, void, L"Включен сигнал \"Цепи управления\"", red, white);
	MESS_BIT(On<iIn  >, void, L"Включен сигнал \"Труба на входе\"", red, white);
	MESS_BIT(On<iOut >, void, L"Включен сигнал \"Труба на выходе\"", red, white);
	MESS_BIT(On<iStrobe >, void, L"Включен сигнал \"Строб\"", red, white);

	MESS_BIT(On<oAutomat>, void, L"On<oAutomat>", red, white);
	MESS_BIT(On<oSupply >, void, L"On<oSupply >", red, white);
	MESS_BIT(On<oMark>, void, "Включен сигнал \"Отметка\"", red, white);
	MESS_BIT(On<oGenerator>, void, "Включен сигнал \"Генератор\"", red, white);

	MESS_BIT(Off<iCU  >, void, L"Отключен сигнал \"Цепи управления\"", blue, white);
	MESS_BIT(Off<iIn  >, void, L"Отключен сигнал \"Труба на входе\"", blue, white);
	MESS_BIT(Off<iOut >, void, L"Отключен сигнал \"Труба на выходе\"", blue, white);
	MESS_BIT(Off<iStrobe >, void, L"Отключен сигнал \"Строб\"", blue, white);

	MESS_BIT(Off<oAutomat>, void, L"Отключен сигнал \"Автомат\"", blue, white);
	MESS_BIT(Off<oSupply >, void, L"Off<oSupply >", blue, white);
	MESS_BIT(Off<oMark>, void, L"Отключен сигнал \"Отметка\"", blue, white);
	MESS_BIT(Off<oGenerator>, void, L"Отключен сигнал \"Генератор\"", blue, white);

	ALARM_BIT(On<iCU  >, void, L"Нет сигнала \"Цепи управления\"", red, yellow);
	
	MESS(TimeOutExteption, void, L"Превышено время ожидания", red, yellow);
	MESS(AlarmBitsExteption, void, L"Выход по аварийному биту", red, yellow);
	MESS(ExitLoopExteption, void, L"Выход из программы", red, yellow);

	MESS(FormatDownloadedFileNoSupported, void, L"Формат загруженного файла не поддерживается", red, yellow);
	MESS(FileDownloaded, void, L"Файл загружен", blue, white);
	MESS(FileSaved, void, L"Файл сохранен", blue, white);
	MESS(FileDownloading, void, L"Загрузка файла", blue, white);
	MESS(Unziping, void, L"Разархивирование файла", blue, white);
	MESS(StopControl, void, L"Прерывание на просмотр", green, white);

	template<class T>struct Bits;
	template<>struct Bits<Vlst<>> {};
#define BITS_XX(color, txt, ...)template<>struct Bits<Vlst<__VA_ARGS__> >\
	{\
		static const int ID = __COUNTER__;\
		typedef int type;\
		static const unsigned backColor = color;\
		static const unsigned textColor = black;\
		static const wchar_t *mess(){return txt;}\
	};\
	template<>struct IDtoMess<Bits<Vlst<__VA_ARGS__> >::ID>\
	{\
		typedef Bits<Vlst<__VA_ARGS__> > Result;\
	};

	BITS_XX(sea, L"Ожидание сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", On<iCU>)
	BITS_XX(sea, L"Выставлен сигнал \"Автомат\"", On<oAutomat>)
	BITS_XX(sea, L"Снят сигнал \"Автомат\"", Off<oAutomat>)
				 
	BITS_XX(sea, L"Ожидание cнятия сигнала \"Труба на входе\"", Off<iIn>)
	BITS_XX(sea, L"Ожидание cнятия сигнала \"Труба на выходе\"", Off<iOut>)
	BITS_XX(sea, L"Ожидание сигнала \"Труба на входе\"", On<iIn>)
	BITS_XX(sea, L"Ожидание сигнала \"Труба на выходе\"", On<iOut>)
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

	MESS(BaseLengthError, void, L"Необходимо уменьшить параметр \"Базовое расстояние\"", red, yellow)

	MESS(Recalculation, void, L"Расчёт", blue, white)
	MESS(RecalculationStop, void, L"Расчёт завершён", blue, white)


	static const int MAX_MESS_ID = __COUNTER__;

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, wchar_t *buf, double val);
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