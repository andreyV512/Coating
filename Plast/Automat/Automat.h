#pragma once
#include <Windows.h>

template<class T>struct On;
template<class T>struct Off;
template<class T>struct TstOn;
template<class T>struct TstOff;
template<class T>struct Proc;

struct TimeOutExteption {};
struct ExitLoopExteption {};
struct AlarmBitsExteption {};

namespace Automat
{
	template<class T>struct Key;
	enum class Status {
		contine
		, exit_loop
		, time_out
		, start
		, stop
		, alarm_bits
		, alarm_l502
		, tubeInWorkZone
		, exit_from_procedure
		, undefined
	};
	struct StartBtn { static const Status value = Status::start; };
	struct StopBtn { static const Status value = Status::stop; };
	struct ContineBtn { static const Status value = Status::contine; };
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