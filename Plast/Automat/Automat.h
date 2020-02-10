#pragma once
#include <Windows.h>

template<class T>struct Key;
template<class T>struct On;
template<class T>struct Off;
template<class T>struct TstOn;
template<class T>struct TstOff;
template<class T>struct Proc;

struct TimeOutExteption {};
struct ExitLoopExteption {};
struct AlarmBitsExteption {};

struct StartBtn  { static const int value = __COUNTER__; };
struct StopBtn   { static const int value = __COUNTER__; };
struct ContineBtn{ static const int value = __COUNTER__; };

namespace Automat
{
	
	template<>struct Key<StartBtn>
	{
		static HANDLE Event;
	};
	template<>struct Key<StopBtn>
	{
		static HANDLE Event;
	};
	template<>struct Key<ContineBtn>
	{
		static HANDLE Event;
	};
	void Init();
	void Destroy();
	void Run();
}