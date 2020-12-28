#pragma once
#include "LogMessages.h"

class Log
{
public:
	static int ID;
	static void Insert(unsigned id, double val, bool);
private:
	template<class T>struct Param{typedef T Result;};
	template<>struct Param<void>{typedef int Result;};

	template<class T>static double Convert(T t){return (double)t;}
public:	
	struct TData
	{
		unsigned id;
		unsigned time;
		double value;
	};
#pragma warning(disable: 4101)
	static LONG lastMessage;
	
	template<class T>struct Filter{static const bool value = true;};
	template<class T>static void Mess(typename Param<typename T::type>::Result value = 0)
	{
		Insert(T::ID, Convert(value), Filter<T>::value);
	}
#pragma warning(default: 4101)
	static bool IsRow(unsigned , TData *&);
	static int LastMessageIndex();
	static bool LastMessage(TData *&d);
	static void TailMessage(TData *&d);
	static void SetSkip(unsigned);
	static void CleanSkip();
	static bool IsRowTest(unsigned row, TData *&d, unsigned &offset);
};

#define DROP_MESSAGE(N)template<>struct Log::Filter<N>{static const bool value = false;};
//DROP_MESSAGE(LogMess::ErrStop)
//DROP_MESSAGE(LogMess::ProgramOpen) 

//DROP_MESSAGE(MessBit<Off<oLampRed  		   >>)
//DROP_MESSAGE(MessBit<Off<oLampGreen		   >>)
//DROP_MESSAGE(MessBit<Off<oBuzzer   		   >>)
//DROP_MESSAGE(MessBit<Off<oOperatorLampRed  >>)
//DROP_MESSAGE(MessBit<Off<oOperatorLampGreen>>)
//DROP_MESSAGE(MessBit<Off<oOperatorBuzzer   >>)
//			
//DROP_MESSAGE(MessBit<On<oLampRed  		   >>)
//DROP_MESSAGE(MessBit<On<oLampGreen		   >>)
//DROP_MESSAGE(MessBit<On<oBuzzer   		   >>)
//DROP_MESSAGE(MessBit<On<oOperatorLampRed   >>)
//DROP_MESSAGE(MessBit<On<oOperatorLampGreen >>)
//DROP_MESSAGE(MessBit<On<oOperatorBuzzer    >>)

DROP_MESSAGE(MessBit<On<iStrobe >>)
DROP_MESSAGE(MessBit<Off<iStrobe>>)

#undef DROP_MESSAGE
