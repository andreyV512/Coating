#pragma once
#include "Units\ComPort\ComPort.h"
namespace FR_E700
{
	static const int inverter_reset_request_did_not_work = 1;
	static const int speed_​​setting_request_did_not_work = 2;
	static const int request_to_set_the_state_did_not_work = 3;
	static const int status_check_request_failed = 4;
	static const int ABC_bit_off = 5;
	static const int inverter_ok = 6;
	static const int start_query = 7;
	static const int inverter_mode_did_not_work = 8;
	enum class WriteState
	{
		none  = 0
		, AU  = 1 << 0
		, STF = 1 << 1
		, STR = 1 << 2
		, RL  = 1 << 3
		, RM  = 1 << 4
		, RH  = 1 << 5
		, RT  = 1 << 6
		, MRS = 1 << 7
	};

	inline WriteState operator|(WriteState l, WriteState r)
	{
		return WriteState((unsigned)l | (unsigned)r);
	}

	enum class ReadState
	{
		none     = 0
		, RUN	 = 1 << 0
		, STF	 = 1 << 1
		, STR	 = 1 << 2
		, SU	 = 1 << 3
		, OL	 = 1 << 4
		, unused = 1 << 5
		, TU	 = 1 << 6
		, ABS	 = 1 << 7
	};

	static const char STX = 0x02;
	static const char ETX = 0x03;
	static const char ENQ = 0x05;
	static const char ACK = 0x06;
	static const char LF  = 0x0A;
	static const char CR  = 0x0D;
	static const char NAK = 0x15;

	static const char timeout = '0';

	bool Init();
	void Destroy();
	bool UpdateComPort();

	class Mode : public ComPortHandler
	{
		static const int maxLoopCount = 3;
		ComPort& port;
		int delay;
		int loopCount;
		unsigned currentTime;
		void Send();
	public:
		Mode();
		void operator()(unsigned char(&input)[1024], int) override;
		void Init();
	};

	class Reset : public ComPortHandler
	{
		static const int maxLoopCount = 3;
		ComPort& port;
		int delay;
		int loopCount;
		unsigned currentTime;
		void Send();
	public:
		Reset();
		void operator()(unsigned char(&input)[1024], int) override;
		void Init();
	};

	class SetFrequency : public ComPortHandler
	{
		static const int maxLoopCount = 2;
		ComPort& port;
		int delay;
		int loopCount;
		unsigned currentTime;
		void Send();
	public:
		SetFrequency();
		void operator()(unsigned char(&input)[1024], int) override;
		void Init();
	};

	class SetState : public ComPortHandler
	{
		static const int maxLoopCount = 3;
		ComPort& port;
		int delay;
		int loopCount;
		unsigned currentTime;
		WriteState state;
		void Send();
	public:
		SetState();
		void operator()(unsigned char(&input)[1024], int) override;
		void Init(WriteState);
	};

	class GetState : public ComPortHandler
	{
		static const int maxLoopCount = 3;
		ComPort& port;
		int delay;
		int loopCount;
		unsigned currentTime;
		ReadState state;
		void Send();
	public:
		GetState();
		void operator()(unsigned char(&input)[1024], int) override;
		void Init();
	};
};

