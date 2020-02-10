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
	MESS_WRAP(On<iCU    	>, void, "������� ������ \"���� ����������\"", red, white);
	MESS_WRAP(On<iKM2_DC >, void, "������� ������ \"��������� (��2 DC)\"", red, white);
	MESS_WRAP(On<iKM3_AC >, void, "������� ������ \"��������� (��3 AC)\"", red, white);
	MESS_WRAP(On<iCycle 	>, void, "������� ������ \"����\"", red, white);
	MESS_WRAP(On<iP1     >, void, "������� ������ \"������������ ������� (�1)\"", red, white);
	MESS_WRAP(On<iP2     >, void, "������� ������ \"������������ ������� (�2)\"", red, white);
	MESS_WRAP(On<iCOPT   >, void, "������� ������ \"����\"", red, white);
	MESS_WRAP(On<iControl>, void, "������� ������ \"��������\"", red, white);
	
	MESS_WRAP(On<oDC_ON1 >, void, "������� ������ \"DC ON1\"", red, white);
	MESS_WRAP(On<oAC_ON  >, void, "������� ������ \"AC ON\"", red, white);
	MESS_WRAP(On<oDC_ON2 >, void, "������� ������ \"DC ON2\"", red, white);
	MESS_WRAP(On<oWork   >, void, "������� ������ \"������\"", red, white);
	MESS_WRAP(On<oStart  >, void, "������� ������ \"����\"", red, white);
	MESS_WRAP(On<oToShift>, void, "������� ������ \"���������\"", red, white);
	MESS_WRAP(On<oC1     >, void, "������� ������ \"�1\"", red, white);
	MESS_WRAP(On<oC2  	>, void, "������� ������ \"�2\"", red, white);

	MESS_WRAP(Off<iCU    	>, void, "�������� ������ \"���� ����������\"", blue, white);
	MESS_WRAP(Off<iKM2_DC >, void, "�������� ������ \"��������� (��2 DC)\"", blue, white);
	MESS_WRAP(Off<iKM3_AC >, void, "�������� ������ \"��������� (��3 AC)\"", blue, white);
	MESS_WRAP(Off<iCycle 	>, void, "�������� ������ \"����\"", blue, white);
	MESS_WRAP(Off<iP1     >, void, "�������� ������ \"������������ ������� (�1)\"", blue, white);
	MESS_WRAP(Off<iP2     >, void, "�������� ������ \"������������ ������� (�2)\"", blue, white);
	MESS_WRAP(Off<iCOPT   >, void, "�������� ������ \"����\"", blue, white);
	MESS_WRAP(Off<iControl>, void, "�������� ������ \"��������\"", blue, white);
	
	MESS_WRAP(Off<oDC_ON1 >, void, "�������� ������ \"DC ON1\"", blue, white);
	MESS_WRAP(Off<oAC_ON  >, void, "�������� ������ \"AC ON\"", blue, white);
	MESS_WRAP(Off<oDC_ON2 >, void, "�������� ������ \"DC ON2\"", blue, white);
	MESS_WRAP(Off<oWork   >, void, "�������� ������ \"������\"", blue, white);
	MESS_WRAP(Off<oStart  >, void, "�������� ������ \"����\"", blue, white);
	MESS_WRAP(Off<oToShift>, void, "�������� ������ \"���������\"", blue, white);
	MESS_WRAP(Off<oC1     >, void, "�������� ������ \"�1\"", blue, white);
	MESS_WRAP(Off<oC2  	>, void, "�������� ������ \"�2\"", blue, white);

	MESS_WRAP(TimeOutExteption, void, "��������� ����� ��������", red, yellow);
	MESS_WRAP(AlarmBitsExteption, void, "����� �� ���������� ����", red, yellow);
	MESS_WRAP(ExitLoopExteption, void, "����� �� ���������", red, yellow);

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