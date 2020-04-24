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

struct StartBtn  { static const int value = 1; };
struct StopBtn   { static const int value = 2; };
struct ContineBtn{ static const int value = 3; };

namespace Automat
{
	enum class Status {
		contine
		, exit_loop
		, time_out
		, start
		, stop
		, alarm_bits
		, alarm_l502
		, tubeInWorkZone
		, undefined
	};
	template<>struct Key<StartBtn>
	{
		static HANDLE hEvent;
	};
	template<>struct Key<StopBtn>
	{
		static HANDLE hEvent;
	};
	template<>struct Key<ContineBtn>
	{
		static HANDLE hEvent;
	};
	void Init();
	void Destroy();
	void Start();
	void Stop();
}