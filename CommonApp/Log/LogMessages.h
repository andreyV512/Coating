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
	///\brief ������������� ����� ����������
	///\param ��� ���������
	///\param ��� ��������������� ���������
	///\param ���������
	///\param ���� ����
	///\param ���� ������
	MESS(StartCycle, void, "���� �����", blue, white)
		//MESS(AirTest		          , void  , "�������� �������"              , blue, white)
	MESS(ProgramOpen, void, "��������� �������", black, white)
	//							          
	MESS(TimeoutPipe, void, "���������� ����� ��������", red, yellow)

	MESS(ProgramClosed, void, "��������� �������", red, yellow)

	MESS(ExitMeshuringCycle, void, "�������� ����� �� �����", red, yellow)
	MESS(ExitTubeInZoneSensors, void, "����� � ������� ���� ���������", red, yellow)

	MESS(DataCollectionDEF, void, "���� ������", blue, white)

	MESS(Alarm502, int, "������ ����� L502  ", red, yellow)

	MESS(DataCollectionCompleted, void, "���� ����� ������ ��������", green, white)

	MESS(Copt, int, "����", green, white)
	MESS(Brak, void, "����", red, yellow)

	MESS(ErrorSpline, void, "������ ������", red, yellow)

	MESS(StopBtn, void, "�������� ����� �� �����", blue, white)
	MESS(On_iIn, void, "�������� ������", blue, white)
	MESS(Collection, void, "���� ������", blue, white)
	MESS(CollectionDone, void, "���� ������ ��������", blue, white)

	MESS_BIT(On<iCU  >, void, "������� ������ \"���� ����������\"", red, white);
	MESS_BIT(On<iIn  >, void, "������� ������ \"����� �� �����\"", red, white);
	MESS_BIT(On<iOut >, void, "������� ������ \"����� �� ������\"", red, white);
	MESS_BIT(On<iStrobe >, void, "������� ������ \"�����\"", red, white);

	MESS_BIT(On<oAutomat>, void, "On<oAutomat>", red, white);
	MESS_BIT(On<oSupply >, void, "On<oSupply >", red, white);
	MESS_BIT(On<oMark>, void, "������� ������ \"�������\"", red, white);

	MESS_BIT(Off<iCU  >, void, "�������� ������ \"���� ����������\"", blue, white);
	MESS_BIT(Off<iIn  >, void, "�������� ������ \"����� �� �����\"", blue, white);
	MESS_BIT(Off<iOut >, void, "�������� ������ \"����� �� ������\"", blue, white);
	MESS_BIT(Off<iStrobe >, void, "�������� ������ \"�����\"", blue, white);

	MESS_BIT(Off<oAutomat>, void, "Off<oAutomat>", blue, white);
	MESS_BIT(Off<oSupply >, void, "Off<oSupply >", blue, white);
	MESS_BIT(Off<oMark>, void, "�������� ������ \"�������\"", blue, white);

	ALARM_BIT(On<iCU  >, void, "��� ������� \"���� ����������\"", red, yellow);
	
	MESS(TimeOutExteption, void, "��������� ����� ��������", red, yellow);
	MESS(AlarmBitsExteption, void, "����� �� ���������� ����", red, yellow);
	MESS(ExitLoopExteption, void, "����� �� ���������", red, yellow);

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

	BITS_XX(sea, "�������� ������� \"���� ����������\"", On<iCU>)
	BITS_XX(sea, "��������� ������ \"�������\"", On<oAutomat>)
	BITS_XX(sea, "���� ������ \"�������\"", Off<oAutomat>)

	BITS_XX(sea, "�������� c����� ������� \"����� �� �����\"", Off<iIn>)
	BITS_XX(sea, "�������� c����� ������� \"����� �� ������\"", Off<iOut>)
	BITS_XX(sea, "�������� ������� \"����� �� �����\"", On<iIn>)
	BITS_XX(sea, "�������� ������� \"����� �� ������\"", On<iOut>)
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
//	//ERR_BITS(red, "��� ������� \"���� ����������\"", On<iCU>)
//#undef ERR_BITS

	MESS(BaseLengthError, void, "���������� ��������� �������� \"������� ����������\"", red, yellow)

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